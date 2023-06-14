/* 
   For fitness calcualte in pso algorithm 
   the parallel version.
 */


#include "nsga_config.h"
#include <iomanip>
//#include <cuda.h>

using namespace std;


void cal_particle_distance_binary(float *apsc, float *exetime, float *disapsc, float *disexet, 
        float *distance, float *avgapsc, float *avgexetime, float *avgdistance)
{
    *avgapsc = 0.0f;
    *avgexetime = 0.0f;
    *avgdistance = 0.0f;

    float amax = (float)MIN;
    float amin = (float)MAX;
    float tmax = (float)MIN;
    float tmin = (float)MAX;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        *avgapsc += apsc[i];
        *avgexetime += exetime[i];

        if (amax < apsc[i]) amax = apsc[i];
        if (amin > apsc[i]) amin = apsc[i];
        if (tmax < exetime[i]) tmax = exetime[i];
        if (tmin > exetime[i]) tmin = exetime[i];
    }
    
    *avgapsc = *avgapsc / SWARMSIZE;
    *avgexetime = *avgexetime / SWARMSIZE;
    
    float ap = amax - amin;
    float ti = tmax - tmin;
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        
        disapsc[i] = (apsc[i] - amin) / ap;  
        disexet[i] = (exetime[i] - tmin) / ti;
        
        distance[i] = disapsc[i] + disexet[i]; 
        
        *avgdistance += distance[i];
    }
    
    *avgdistance = *avgdistance / SWARMSIZE;
}


void cal_apsc_serial_binary(bool *CMatrix, int *particle, float *apsc, int *eff_num, 
        int code_line, bool *max_cover, int num_testcase)
{
    int *max_line = NULL;
    max_line = (int*)malloc(sizeof(int) * code_line);

    for (int i = 0; i < code_line; i++) max_line[i] = -1;

    int line_count = 0;
    for (int i = 0; i < code_line; i++)
    {
        if (max_cover[i] == 1)
        {
            max_line[line_count] = i;
            line_count++;
        }
    }
        
    float eff_code_line = 0.0f;
    for (int j = 0; j < code_line; j++)
    {
        if (max_cover[j] == 1) eff_code_line += 1;
        else continue;
    }


    for (int i = 0; i < SWARMSIZE; i++)
    {
        bool *code_cover = NULL;
        code_cover = (bool*)malloc(sizeof(bool) * code_line);

        int count_tc = 0;

        for (int j = 0; j < code_line; j++) code_cover[j] = 0;

        for (int j = 0; j < num_testcase; j++)
        {
            if (particle[i * num_testcase + j] == 1)
            {
                count_tc += 1;

                for (int k = 0; k < line_count; k++)
                {
                    code_cover[max_line[k]] |= CMatrix[j * code_line + max_line[k]]; 
                }
            }
        }
       
        eff_num[i] = count_tc;
        
        count_tc = 0;
        for (int j = 0; j < code_line; j++) 
        {
            if (code_cover[j] == 1)
            {
                count_tc ++;
            }
        }

        apsc[i] = (float)(count_tc) / eff_code_line;

        free(code_cover);
    }

    free(max_line);
}


void cal_efftime_serial_binary(int *particle, float *TVector, float *exetime, 
        int num_testcase)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        exetime[i] = 0;

        for (int j = 0; j < num_testcase; j++)
        {
            if (particle[i * num_testcase + j] == 1)
            {
                exetime[i] += TVector[j];
            }
        }
    }
}


void Cal_fitness_binary(Swarms* sw_pt, int num_testcase, int code_line, bool* CMatrix, float* TVector, 
        bool* max_cover, int start)
{
    cal_apsc_serial_binary(CMatrix, &(sw_pt->particle[start * num_testcase]), &(sw_pt->apsc[start]),
            &(sw_pt->effnum[start]), code_line, max_cover, num_testcase);
    
    cal_efftime_serial_binary(&(sw_pt->particle[start * num_testcase]), TVector, 
            &(sw_pt->exetime[start]), num_testcase);
    
    cal_particle_distance_binary(&(sw_pt->apsc[start]), &(sw_pt->exetime[start]), 
            &(sw_pt->disapsc[start]), &(sw_pt->disexet[start]), &(sw_pt->distance[start]),
            &(sw_pt->avgapsc), &(sw_pt->avgexetime), &(sw_pt->avgdistance));

    return;
}

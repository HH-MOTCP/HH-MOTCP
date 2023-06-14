/* 
   For fitness calcualte in pso algorithm 
   the parallel version.
 */


#include "nsga_config.h"
#include <iomanip>
//#include <cuda.h>

using namespace std;


void cal_particle_distance(float *apsc, float *exetime, float *disapsc, float *disexet, 
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


void cal_apsc_serial(bool *CMatrix, int *particle, float *apsc, int *eff_num, 
        int code_line, bool *max_cover, int num_testcase)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        int sum_testnum = 0;
        int max_testnum = 0;

        for (int j = 0; j < code_line; j++)
        {
            if (max_cover[j] == 0) continue;

            for (int k = 0; k < num_testcase; k++)
            {
                bool temp = CMatrix[particle[i * num_testcase + k] * 
                    code_line + j]; 
                
                if (temp == 1)
                {
                    k += 1;
                    sum_testnum += k; 
                    if (k > max_testnum) max_testnum = k;
                    break;
                }
            }
        }
     
        float eff_code_line = 0.0f;
        for (int j = 0; j < code_line; j++)
        {
            if (max_cover[j] == 1) eff_code_line += 1;
            else continue;
        }

        eff_num[i] = max_testnum;
        apsc[i] = 1 - ((float)sum_testnum / (float)(eff_code_line * num_testcase))
            + 1.0f / (float)(2 * num_testcase);
    }
}


void cal_efftime_serial(int *eff_num, int *particle, float *TVector, float *exetime, 
        int num_testcase)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        exetime[i] = 0;
        for (int j = 0; j < eff_num[i]; j++)
        {
            exetime[i] += TVector[particle[i * num_testcase + j]];
        }
    }
}

void cal_front_serial(float *apsc, float *exetime, bool *efficient, 
        int *eff_mark)
{
    for (int i = 0; i < SWARMSIZE; i++) efficient[i] = 1;
    //for (int i = 0; i < SWARMSIZE; i++) eff_mark[i] = 0;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = i + 1; j < SWARMSIZE; j++)
        {
            if (efficient[j] == 0) continue;

            if (apsc[i] < apsc[j] && exetime[i] > exetime[j])
            {
                efficient[i] = 0;
                break;
            }
            else if (apsc[i] > apsc[j] && exetime[i] < exetime[j])
            {
                efficient[j] = 0;
            }
        }
    }
}




void Cal_fitness_tcp(Swarms* sw_pt, int num_testcase, int code_line, bool* CMatrix, float* TVector, 
        bool* max_cover, int start)
{
    cal_apsc_serial(CMatrix, &(sw_pt->particle[start * num_testcase]), &(sw_pt->apsc[start]),
            &(sw_pt->effnum[start]), code_line, max_cover, num_testcase);
    
    
    cal_efftime_serial(&(sw_pt->effnum[start]), &(sw_pt->particle[start *
                num_testcase]), TVector, &(sw_pt->exetime[start]), num_testcase);
    
    //cal_front<<<1, SWARMSIZE>>>(d_apsc, d_exetime, d_efficient);
    
    //CUDA_CALL(cudaMemcpy(&(sw_pt->efficient[start]), d_efficient, sizeof(bool) *
    //            SWARMSIZE, cudaMemcpyDeviceToHost));
    
    cal_particle_distance(&(sw_pt->apsc[start]), &(sw_pt->exetime[start]), 
            &(sw_pt->disapsc[start]), &(sw_pt->disexet[start]), &(sw_pt->distance[start]),
            &(sw_pt->avgapsc), &(sw_pt->avgexetime), &(sw_pt->avgdistance));

    return;
}

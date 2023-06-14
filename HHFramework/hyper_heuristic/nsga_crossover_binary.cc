/*
 * =====================================================================================
 *
 *       Filename:  nsga_crossover.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/22/14 10:40:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

//#include "nsga_crossover.h"
#include "nsga_config.h"
#include "stdlib.h"
#include <iostream>
#include "domain_judge.h"

using namespace std;

void Nsga_crossover_singlepoint_binary(Swarms* sw_pt, int *temp_ind, int num_testcase)
{
    int *cross_num = NULL;
    cross_num = (int*) malloc(sizeof(int) * SWARMSIZE);
    
    int min_pos = sw_pt->effnum[0];
    for (int i = 1; i < SWARMSIZE; i++) 
    {
        if (sw_pt->effnum[i] < min_pos)
        {
            min_pos = sw_pt->effnum[i];
        }
    }

    srand((unsigned int)time(0) * 3);
    for (int i = 0; i < SWARMSIZE; i++) 
    {
        cross_num[i] = rand() % num_testcase;
    }

    
    int POS = SWARMSIZE * num_testcase;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < cross_num[i]; j++)
        {
        
            int temp = sw_pt->particle[temp_ind[2 * i] * num_testcase + j];
            sw_pt->particle[POS + i * num_testcase + j] = temp;
        }

        int start_point = 0;
        for (int j = cross_num[i]; j < num_testcase; j++)
        {
            int temp = sw_pt->particle[temp_ind[2 * i + 1] * num_testcase + j];
            sw_pt->particle[POS + i * num_testcase + j] = temp;
        }
    }

    free(cross_num);

    return;
}


void Nsga_crossover_serial_binary(Swarms* sw_pt, int *temp_ind, int num_testcase)
{
    int *cross_num = NULL;
    cross_num = (int*)malloc(sizeof(int) * SWARMSIZE * 2);

    srand((unsigned int)time(0) * 2);
    for (int i = 0; i < SWARMSIZE * 2; i++) cross_num[i] = rand() % num_testcase;

    int POS = SWARMSIZE * num_testcase;
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        int cro_p1 = cross_num[loop * 2];
        int cro_p2 = cross_num[loop * 2 + 1];
        if (cro_p1 > cro_p2)
        {
            int temp = cro_p2;
            cro_p2 = cro_p1;
            cro_p1 = temp;
        }

        for (int i = cro_p1; i < cro_p2; i++)
        {
            int temp = sw_pt->particle[temp_ind[2 * loop] * num_testcase + i];
            sw_pt->particle[POS + loop * num_testcase + i] = temp;
        }

        for (int i = 0; i < cro_p1; i++)
        {
            int temp = sw_pt->particle[temp_ind[2 * loop + 1] * num_testcase + i];
            sw_pt->particle[POS + loop * num_testcase + i] = temp;
        }

        for (int i = cro_p2; i < num_testcase; i++)
        {
            int temp = sw_pt->particle[temp_ind[2 * loop + 1] * num_testcase + i];
            sw_pt->particle[POS + loop * num_testcase + i] = temp;
        }

    }

    free(cross_num);
    return;
}

void Nsga_crossover_uniform_binary(Swarms* sw_pt, int *temp_ind, bool *temp_pos, int num_testcase)
{
    int POS = SWARMSIZE * num_testcase;
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        for (int i = 0; i < num_testcase; i++)
        {
            if (temp_pos[loop * num_testcase + i] == 0) continue;
            else
            {
                int temp = sw_pt->particle[temp_ind[2 * loop] * num_testcase + i];
                sw_pt->particle[POS + loop * num_testcase + i] = temp;
            }
        }

        int start_pos = 0;
        for (int i = 0; i < num_testcase; i++)
        {
            if (temp_pos[loop * num_testcase + i] == 1) continue;
            else
            {
                int temp = sw_pt->particle[temp_ind[2 * loop + 1] * num_testcase + i];
                sw_pt->particle[POS + loop * num_testcase + i] = temp;
            }
        }

    }

    return;
}


void Pso_crossover_uniform_binary(Swarms* sw_pt, Swarms* pso_sw_pt, bool *temp_pos, int num_testcase)
{
    int POS = SWARMSIZE * num_testcase;
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        for (int i = 0; i < num_testcase; i++)
        {
            if (temp_pos[loop * num_testcase + i] == 0) continue;
            else
            {
                int temp = pso_sw_pt->particle[POS + loop * num_testcase + i];
                sw_pt->particle[POS + loop * num_testcase + i] = temp;
            }
        }

        int start_pos = 0;
        for (int i = 0; i < num_testcase; i++)
        {
            if (temp_pos[loop * num_testcase + i] == 1) continue;
            else
            {
                int temp = sw_pt->particle[loop * num_testcase + i];
                sw_pt->particle[POS + loop * num_testcase + i] = temp;
            }
        }

    }

    return;
}

void Nsga_copy_binary(Swarms* sw_pt, int num_testcase)
{
    int start = SWARMSIZE * num_testcase;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++)
        {
            int temp = sw_pt->particle[i * num_testcase + j];
            sw_pt->particle[start + i * num_testcase + j] = temp;
        }
    }
}

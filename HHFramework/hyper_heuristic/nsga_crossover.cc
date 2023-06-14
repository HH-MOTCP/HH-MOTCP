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

void Nsga_crossover_singlepoint(Swarms* sw_pt, int *temp_ind, int num_testcase)
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
        //cross_num[i] = rand() % min_pos;
    }

    bool *mark = NULL;
    mark = (bool*) malloc(sizeof(bool) * num_testcase);
    
    int POS = SWARMSIZE * num_testcase;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++) mark[j] = 0;

        for (int j = 0; j < cross_num[i]; j++)
        {
        
            int temp = sw_pt->particle[temp_ind[2 * i] * num_testcase + j];
            sw_pt->particle[POS + i * num_testcase + j] = temp;
            mark[temp] += 1;
        }

        int start_point = 0;
        for (int j = cross_num[i]; j < num_testcase; j++)
        {
            for (int k = start_point; k < num_testcase; k++)
            {
                int temp = sw_pt->particle[temp_ind[2 * i + 1] * num_testcase + k];
                if (mark[temp] == 0)
                {
                    sw_pt->particle[POS + i * num_testcase + j] = temp;
                    start_point = k + 1;
                    break;
                }
                else continue;
            }
        }
    }

    free(mark);
    free(cross_num);

    return;
}


void Nsga_crossover_antsinglepoint(Swarms* sw_pt, int *temp_ind, int num_testcase)
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
        //cross_num[i] = rand() % min_pos;
    }

    bool *mark = NULL;
    mark = (bool*) malloc(sizeof(bool) * num_testcase);
    
    int POS = SWARMSIZE * num_testcase;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++) mark[j] = 0;

        //for (int j = 0; j < cross_num[i]; j++)
        for (int j = cross_num[i]; j < num_testcase; j++)
        {
        
            int temp = sw_pt->particle[temp_ind[2 * i] * num_testcase + j];
            sw_pt->particle[POS + i * num_testcase + j] = temp;
            mark[temp] += 1;
        }

        int start_point = 0;
        //for (int j = cross_num[i]; j < num_testcase; j++)
        for (int j = 0; j < cross_num[i]; j++)
        {
            for (int k = start_point; k < num_testcase; k++)
            {
                int temp = sw_pt->particle[temp_ind[2 * i + 1] * num_testcase + k];
                if (mark[temp] == 0)
                {
                    sw_pt->particle[POS + i * num_testcase + j] = temp;
                    start_point = k + 1;
                    break;
                }
                else continue;
            }
        }
    }

    free(mark);
    free(cross_num);

    return;
}

void Nsga_crossover_antiscan(Swarms* sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase)
{
    int *cross_num = NULL;
    int *t_num = NULL;
    cross_num = (int*)malloc(sizeof(int) * (num_testcase + 1));
    t_num = (int*)malloc(sizeof(int) * num_testcase);
    
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        cross_num[num_testcase] = 0;
        cross_num[0] = 0;
        for (int i = 1; i < num_testcase + 1; i++) 
            cross_num[i] = cross_num[i - 1] + (int)temp_pos[loop * num_testcase + i - 1];
        for (int i = 0; i < num_testcase; i++) 
            t_num[i] = i - cross_num[i];
        for (int i = 0; i < num_testcase; i++) 
        {
            if (temp_pos[loop * num_testcase + i] == 1)
            {
                t_num[i] = num_testcase - 1 - cross_num[i];
            }
        }
        
        int POS = SWARMSIZE * num_testcase;
        for (int i = 0; i < num_testcase; i++)
        {
            sw_pt->particle[POS + loop * num_testcase + t_num[i]] =
                sw_pt->particle[temp_ind[loop] * num_testcase + i];
        }

    }
    
    free(cross_num);
    free(t_num);
    return;
}

void Nsga_crossover_scan(Swarms* sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase)
{
    int *cross_num = NULL;
    int *t_num = NULL;
    cross_num = (int*)malloc(sizeof(int) * (num_testcase + 1));
    t_num = (int*)malloc(sizeof(int) * num_testcase);
    
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        cross_num[num_testcase] = 0;
        cross_num[0] = 0;
        for (int i = 1; i < num_testcase + 1; i++) 
            cross_num[i] = cross_num[i - 1] + (int)temp_pos[loop * num_testcase + i - 1];
        for (int i = 0; i < num_testcase; i++) 
            t_num[i] = cross_num[num_testcase] + i - cross_num[i];
        for (int i = 0; i < num_testcase; i++) 
        {
            if (temp_pos[loop * num_testcase + i] == 1)
            {
                t_num[i] = cross_num[i];
            }
        }
    
        int POS = SWARMSIZE * num_testcase;
        for (int i = 0; i < num_testcase; i++)
        {
            sw_pt->particle[POS + loop * num_testcase + t_num[i]] =
                sw_pt->particle[temp_ind[loop] * num_testcase + i];
        }

    }

    free(cross_num);
    free(t_num);
    return;
}

void Nsga_crossover_serial(Swarms* sw_pt, int *temp_ind, int num_testcase)
{
    int *cross_num = NULL;
    cross_num = (int*)malloc(sizeof(int) * SWARMSIZE * 2);

    srand((unsigned int)time(0) * 2);
    for (int i = 0; i < SWARMSIZE * 2; i++) cross_num[i] = rand() % num_testcase;

    bool *mark = NULL;
    mark = (bool*) malloc(sizeof(bool) * num_testcase);

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
        
        for (int i = 0; i < num_testcase; i++) mark[i] = 0;

        for (int i = cro_p1; i < cro_p2; i++)
        {
            int temp = sw_pt->particle[temp_ind[2 * loop] * num_testcase + i];
            sw_pt->particle[POS + loop * num_testcase + i] = temp;
            mark[temp] = 1;
        }

        int start_pos = 0;
        for (int i = 0; i < cro_p1; i++)
        {
            for (int j = start_pos; j < num_testcase; j++)
            {
                int temp = sw_pt->particle[temp_ind[2 * loop + 1] * num_testcase + j];
                if (mark[temp] == 0)
                {
                    sw_pt->particle[POS + loop * num_testcase + i] = temp;
                    start_pos = j + 1;
                    break;
                }
                else continue;
            }
        }

        for (int i = cro_p2; i < num_testcase; i++)
        {
            for (int j = start_pos; j < num_testcase; j++)
            {
                int temp = sw_pt->particle[temp_ind[2 * loop + 1] * num_testcase + j];
                if (mark[temp] == 0)
                {
                    sw_pt->particle[POS + loop * num_testcase + i] = temp;
                    start_pos = j + 1;
                    break;
                }
                else continue;
            }
        }

    }

    free(mark);
    free(cross_num);
    return;
}

void Nsga_crossover_uniform(Swarms* sw_pt, int *temp_ind, bool *temp_pos, int num_testcase)
{
    bool *mark = NULL;
    mark = (bool*) malloc(sizeof(bool) * num_testcase);

    int POS = SWARMSIZE * num_testcase;
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        for (int i = 0; i < num_testcase; i++) mark[i] = 0;

        for (int i = 0; i < num_testcase; i++)
        {
            if (temp_pos[loop * num_testcase + i] == 0) continue;
            else
            {
                int temp = sw_pt->particle[temp_ind[2 * loop] * num_testcase + i];
                sw_pt->particle[POS + loop * num_testcase + i] = temp;
                mark[temp] = 1;
            }
        }

        int start_pos = 0;
        for (int i = 0; i < num_testcase; i++)
        {
            for (int j = start_pos; j < num_testcase; j++)
            {

                if (temp_pos[loop * num_testcase + i] == 1) continue;
                else
                {
                    int temp = sw_pt->particle[temp_ind[2 * loop + 1] * num_testcase + j];
                    if (mark[temp] == 0)
                    {
                        sw_pt->particle[POS + loop * num_testcase + i] = temp;
                        start_pos = j + 1;
                        break;
                    }
                }
            }
        }

    }

    free(mark);
    return;
}


void Pso_crossover_uniform(Swarms* sw_pt, Swarms* pso_sw_pt, bool *temp_pos, int num_testcase)
{
    bool *mark = NULL;
    mark = (bool*) malloc(sizeof(bool) * num_testcase);

    int POS = SWARMSIZE * num_testcase;
    for (int loop = 0; loop < SWARMSIZE; loop++)
    {
        for (int i = 0; i < num_testcase; i++) mark[i] = 0;

        for (int i = 0; i < num_testcase; i++)
        {
            if (temp_pos[loop * num_testcase + i] == 0) continue;
            else
            {
                int temp = pso_sw_pt->particle[POS + loop * num_testcase + i];
                sw_pt->particle[POS + loop * num_testcase + i] = temp;
                mark[temp] = 1;
            }
        }

        int start_pos = 0;
        for (int i = 0; i < num_testcase; i++)
        {
            for (int j = start_pos; j < num_testcase; j++)
            {

                if (temp_pos[loop * num_testcase + i] == 1) continue;
                else
                {
                    int temp = sw_pt->particle[loop * num_testcase + j];
                    if (mark[temp] == 0)
                    {
                        sw_pt->particle[POS + loop * num_testcase + i] = temp;
                        start_pos = j + 1;
                        break;
                    }
                }
            }
        }

    }

    free(mark);
    return;
}



void Nsga_copy(Swarms* sw_pt, int num_testcase)
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

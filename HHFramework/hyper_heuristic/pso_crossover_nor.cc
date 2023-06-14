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


void Pso_crossover_nor(Swarms* sw_pt, int *temp_ind, int num_testcase, int count)
{
    int CRO_POS = (int)((float)num_testcase * CRORATE);
    int loop_num = (int)((float)SWARMSIZE * CRORATE);

    int *cross_num = NULL;
    cross_num = (int*) malloc(sizeof(int) * num_testcase * loop_num);
    
    int *front_num = NULL;
    front_num = (int*) malloc(sizeof(int) * SWARMSIZE);
    
    /*
    int min_pos = sw_pt->effnum[0];
    for (int i = 1; i < SWARMSIZE; i++) 
    {
        if (sw_pt->effnum[i] < min_pos)
        {
            min_pos = sw_pt->effnum[i];
        }
    }
    */

    for (int i = 0; i < loop_num; i++)
    {
        Shuffle(&(cross_num[i * num_testcase]), num_testcase, i);
    }

    srand((unsigned int)time(0) * 3);
    for (int i = 0; i < SWARMSIZE; i++) 
    {
        //cross_num[i] = rand() % num_testcase;
        //cross_num[i] = rand() % min_pos;

        front_num[i] = rand() % count;
    }
    
    bool *mark = NULL;
    mark = (bool*) malloc(sizeof(bool) * num_testcase);
    
    bool *pose = NULL;
    pose = (bool*) malloc(sizeof(bool) * num_testcase);
    
    int POS = SWARMSIZE * num_testcase;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++) 
        {
            mark[j] = 0;
            pose[j] = 0;
        }

        for (int j = 0; j < CRO_POS; j++)
        {
        
            int position = cross_num[i * CRO_POS + j];
            int temp = sw_pt->particle[temp_ind[front_num[i]] * num_testcase + position];
            
            sw_pt->particle[POS + i * num_testcase + position] = temp;
            mark[temp] += 1;
            pose[position] += 1;
        }

        int start_point = 0;
        for (int j = 0; j < num_testcase; j++)
        {
            if (pose[j] != 0) continue;

            for (int k = start_point; k < num_testcase; k++)
            {
                int temp = sw_pt->particle[i * num_testcase + k];
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
    
    free(pose);
    free(mark);
    free(front_num);
    free(cross_num);

    return;
}



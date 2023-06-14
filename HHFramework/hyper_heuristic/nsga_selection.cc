/*
 * =====================================================================================
 *
 *       Filename:  nsga_selection.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/15 16:13:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"
#include <iostream>
#include "domain_judge.h"

using namespace std;

void Nsga_selection_rand(Swarms *sw_pt, int *temp_ind)
{
    srand((unsigned int)time(0) * 2);
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        int rand_num1 = rand() % SWARMSIZE;
        int rand_num2 = rand() % SWARMSIZE;

        temp_ind[i] = Domain_judge(sw_pt, rand_num1, rand_num2);
    }

    return;
}

void Nsga_selection_rand_twodim(Swarms *sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase)
{
    srand((unsigned int)time(0) * 2);
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        int rand_num1 = rand() % SWARMSIZE;
        int rand_num2 = rand() % SWARMSIZE;

        temp_ind[i] = Domain_judge(sw_pt, rand_num1, rand_num2);
    }

    int rand_num = (int)((float)num_testcase * CRORATE);
    srand((unsigned int)time(0) * 2);
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++)
        {
            temp_pos[i * num_testcase + j] = 0;
        }

        for (int j = 0; j < rand_num; j++)
        {
            int pos = rand() % num_testcase;
            temp_pos[i * num_testcase + pos] = 1;
        }
    }
    
    return;
}

void Nsga_selection_repeat(Swarms *sw_pt, Schema *sch_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        temp_ind[i] = i;
        temp_ind[i + SWARMSIZE] = i;
    }

    bool* mark_rep = NULL;
    mark_rep = (bool*) malloc(sizeof(bool) * sch_pt->map_size);
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < sch_pt->map_size; j++)
            mark_rep[j] = 0;

        for (int j = 0; j < sw_pt->effnum[i]; j++)
        {
            int temp = sch_pt->matrix_map[sw_pt->particle[i * num_testcase + j]];
            if (mark_rep[temp] == 0)
            {
                mark_rep[temp] = 1;
            }
            else
            {
                temp_pos[i * num_testcase + j] = 1;
            }
        }
    }

    free(mark_rep);
}

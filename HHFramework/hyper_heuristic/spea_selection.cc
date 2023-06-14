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

void Spea_selection(Swarms *sw_pt, int *temp_ind, bool *temp_pos, int num_testcase)
{
    temp_ind[0] = 0;
    temp_ind[SWARMSIZE * 2 - 1] = 0;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        temp_ind[i * 2] = i;
        temp_ind[i * 2 - 1] = i;
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


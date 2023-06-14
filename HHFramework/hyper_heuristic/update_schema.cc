/*
 * =====================================================================================
 *
 *       Filename:  update_schema.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/03/15 15:16:51
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

using namespace std;

void Update_schema(Swarms* sw_pt, Schema* sch_pt, int range, int num_testcase)
{
    sch_pt->sch_size = 0;

    for (int i = 0; i < range; i++)
    {
        if (sw_pt->effnum[i] > sch_pt->sch_size)
        {
            sch_pt->sch_size = sw_pt->effnum[i];
        }
    }

    for (int i = 0; i < range; i++)
    {
        for (int j = 0; j < sch_pt->sch_size; j++)
        {
            int temp = sw_pt->particle[i * num_testcase + j];
            sch_pt->sch_table[i * num_testcase + j] = sch_pt->matrix_map[temp];
        }
    }

    int threshold = (range + 1) / 2;

    int *thre_rec = NULL;
    thre_rec = (int*) malloc(sizeof(int) * sch_pt->map_size);
    
    for (int i = 0; i < sch_pt->sch_size; i++)
    {
        for (int j = 0; j < sch_pt->map_size; j++)
        {
            thre_rec[j] = 0;
        }
        
        bool achive = 0;
        for (int j = 0; j < range; j++)
        {
            int num = sch_pt->sch_table[j * num_testcase + i];
            
            thre_rec[num] += 1;
            
            if (thre_rec[num] >= threshold)
            {
                sch_pt->schema[i] = num;
                achive = 1;
                break;
            }
        }
        
        if (achive == 0)
        {
            sch_pt->schema[i] = -1;
        }
    }
    
    free(thre_rec);

    return;
}



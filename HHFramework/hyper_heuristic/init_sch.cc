/*
 * =====================================================================================
 *
 *       Filename:  init_sch.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17/03/15 18:25:25
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

void Initial_schema(Schema* sch_pt, bool* CMatrix, int num_testcase, int code_line)
{
    sch_pt->matrix_map = (int*) malloc(sizeof(int) * num_testcase);
    sch_pt->schema     = (int*) malloc(sizeof(int) * num_testcase);
    sch_pt->sch_value  = (float*) malloc(sizeof(float) * SWARMSIZE);
    sch_pt->sch_table  = (int*) malloc(sizeof(int) * SWARMSIZE * num_testcase);
    sch_pt->sch_chose  = (bool*) malloc(sizeof(bool) * SWARMSIZE * num_testcase);
    
    for (int i = 0; i < num_testcase; i++)
    {
        sch_pt->matrix_map[i] = -1;
        sch_pt->schema[i] = -1;
    }

    for (int i = 0; i < SWARMSIZE; i++) sch_pt->sch_value[i] = 0.0f;

    for (int i = 0; i < SWARMSIZE * num_testcase; i++)
    {
        sch_pt->sch_table[i] = -1;
        sch_pt->sch_chose[i] = 0;
    }

    int count = 0;
    /* 
    for (int i = 0; i < num_testcase; i++)
    {
        if (sch_pt->matrix_map[i] != -1) continue;
        else
        {
            sch_pt->matrix_map[i] = count;
            count++;
        }

        for (int j = i + i; j < num_testcase; j++)
        {
            if(sch_pt->matrix_map[j] != -1) continue;
            
            bool mark_same = 0;
            for (int k = 0; k < code_line; k++)
            {
                if ((CMatrix[i * code_line + k] ^ CMatrix[j * code_line + k]) == 0)
                    continue;
                else
                {
                    mark_same = 1;
                    break;
                }
            }
            
            if (mark_same == 0)
            {
                sch_pt->matrix_map[j] = sch_pt->matrix_map[i];
            }
        }
    }
    */
    sch_pt->map_size = count;

    return;
}

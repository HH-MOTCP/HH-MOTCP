/*
 * =====================================================================================
 *
 *       Filename:  free_mem.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/03/15 16:56:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <iostream>
#include "nsga_config.h"

using namespace std;

void Free_memory(bool* CMatrix, float* TVector, bool* max_cover, float* learn_val, 
        int* learn_time)
{
    free(CMatrix);
    free(TVector);
    free(max_cover);
    free(learn_val);
    free(learn_time);
     
    return;
}

void Free_swarm(Swarms* sw_pt)
{
    free(sw_pt->particle);
    free(sw_pt->apsc);
    free(sw_pt->exetime);
    free(sw_pt->effnum);
    free(sw_pt->distance);
    free(sw_pt->disapsc);
    free(sw_pt->disexet);
    free(sw_pt->efficient);
    free(sw_pt->newgen);
    free(sw_pt->batch);
    
    return;
}

void Free_schema(Schema* sch_pt)
{
    free(sch_pt->matrix_map);
    free(sch_pt->schema);
    free(sch_pt->sch_value);
    free(sch_pt->sch_table);
    free(sch_pt->sch_chose);

    return;
}

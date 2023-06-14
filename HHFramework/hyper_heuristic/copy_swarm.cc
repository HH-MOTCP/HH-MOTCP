/*
 * =====================================================================================
 *
 *       Filename:  copy_swarm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  14/05/15 10:47:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include "nsga_config.h"

void Copy_swarm(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase)
{
    for (int i = 0; i < num_testcase; i++)
    {
        int pos1 = a * num_testcase + i;
        int pos2 = b * num_testcase + i;
        sw_new->particle[pos1] = sw_pt->particle[pos2];
    } 
    
    sw_new->apsc[a] =      sw_pt->apsc[b];
    sw_new->exetime[a] =   sw_pt->exetime[b];
    sw_new->distance[a] =  sw_pt->distance[b];
    sw_new->disapsc[a] =   sw_pt->disapsc[b];
    sw_new->disexet[a] =   sw_pt->disexet[b];
    sw_new->effnum[a] =    sw_pt->effnum[b];
    sw_new->efficient[a] = sw_pt->efficient[b];
    sw_new->newgen[a] =   sw_pt->newgen[b];
    sw_new->batch[a] =   sw_pt->batch[b];

    return;
}
void Copy_swarm_samebatch(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase)
{
    for (int i = 0; i < num_testcase; i++)
    {
        int pos1 = a * num_testcase + i;
        int pos2 = b * num_testcase + i;
        sw_new->particle[pos1] = sw_pt->particle[pos2];
    } 
    
    sw_new->apsc[a] =      sw_pt->apsc[b];
    sw_new->exetime[a] =   sw_pt->exetime[b];
    sw_new->distance[a] =  sw_pt->distance[b];
    sw_new->disapsc[a] =   sw_pt->disapsc[b];
    sw_new->disexet[a] =   sw_pt->disexet[b];
    sw_new->effnum[a] =    sw_pt->effnum[b];
    

    return;
}



/*
 * =====================================================================================
 *
 *       Filename:  eliter_selection.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/27/14 17:25:24
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
#include <algorithm>
#include "domain_judge.h"
#include "copy_swarm.h"

using namespace std;

void Copy_swarm_pso(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase)
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
void Switch_particle_pso(Swarms *sw_pt, int a, int b, int num_testcase)
{
    //if (a == b) return;
     
    for (int i = 0; i < num_testcase; i++)
    {
        int pos1 = a * num_testcase + i;
        int pos2 = b * num_testcase + i;
        swap(sw_pt->particle[pos1], sw_pt->particle[pos2]);
    } 
    
    swap(sw_pt->apsc[a],      sw_pt->apsc[b]);
    swap(sw_pt->exetime[a],   sw_pt->exetime[b]);
    swap(sw_pt->distance[a],  sw_pt->distance[b]);
    swap(sw_pt->disapsc[a],   sw_pt->disapsc[b]);
    swap(sw_pt->disexet[a],   sw_pt->disexet[b]);
    swap(sw_pt->effnum[a],    sw_pt->effnum[b]);
    swap(sw_pt->efficient[a], sw_pt->efficient[b]);
    swap(sw_pt->newgen[a],    sw_pt->newgen[b]);
    swap(sw_pt->batch[a],    sw_pt->batch[b]);

    return;
}

bool compare_pso(int a, int b)
{
    //return sw_pt->distance[a] > sw_pt->distance[b];
    return a > b;
}

void Maopao_pso(Swarms *sw_pt, int *record, int count)
{

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = count -2; j >= i; j--)
        {
            int dis1 = (int)(sw_pt->distance[record[j]] * 1000000);
            int dis2 = (int)(sw_pt->distance[record[j + 1]] * 1000000);
            
            if (dis1 < dis2)
                swap(record[j], record[j + 1]);
        }
    }
}

void Init_temp_swarm_pso(Swarms* sw_pt, int num_testcase)
{
    
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch  = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 
    
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->apsc[i]      = (float)MIN; 
        sw_pt->exetime[i]   = (float)MAX;
        sw_pt->distance[i]  = (float)MAX;
        sw_pt->disapsc[i]   = (float)MAX;
        sw_pt->disexet[i]   = (float)MAX;
        sw_pt->effnum[i]    = 0;
        sw_pt->efficient[i] = 0;
        sw_pt->newgen[i] = 0;
        sw_pt->batch[i] = 0;
    }

    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
}

void Free_pso(Swarms* sw_pt)
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
}

int Pso_eliter_selection(Swarms *sw_pt, Swarms *pso_sw_pt, int num_testcase)
{
    int front_num = 0;
    int set_count = 0;

    Swarms sw_new;
    Init_temp_swarm_pso(&(sw_new), num_testcase);

    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->efficient[i] = 0;
    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->batch[i] = 0;
    
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[i] = 0;
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[SWARMSIZE + i] = 1;
   
    int batch_num = 0;
    while (set_count < SWARMSIZE && batch_num < SWARMSIZE)
    {
        batch_num++;

        bool mark_dom[SWARMSIZE * 2];
        int mark_record[SWARMSIZE * 2];
        
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            mark_dom[i] = 0;
            mark_record[i] = -1;
        }

        //for (int i = set_count; i < SWARMSIZE * 2; i++)
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            if (sw_pt->efficient[i] == 1) 
            {
                mark_dom[i] = 1;
            }

            if (mark_dom[i] == 1) continue;

            for (int j = i + 1; j < SWARMSIZE * 2; j++)
            {
                if (mark_dom[j] == 1 || sw_pt->efficient[j] == 1) continue;

                int judge = Domain_compare(sw_pt, i, j);
                
                if (judge == -1) continue;
                else if (judge == -2)
                {
                    sw_pt->efficient[j] = 1;
                }
                else if (judge == i)
                {
                    mark_dom[j] = 1;
                }
                else
                {
                    mark_dom[i] = 1;
                    break;
                }
            }
        }
       
        int f_count = 0;
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            if (0 == mark_dom[i])
            {
                mark_record[f_count] = i;
                sw_pt->efficient[i] = 1;
                sw_pt->batch[i] = batch_num;
                f_count++;
            }
        }

        if (set_count == 0 || set_count + f_count >= SWARMSIZE)
        {
            Maopao_pso(sw_pt, &(mark_record[0]), f_count);
        }
        
        for (int i = 0; i < f_count; i++)
        {
            //Switch_particle_pso(sw_pt, i + set_count, mark_record[i], num_testcase);
            //Copy_swarm_pso(sw_pt, &(sw_new), i + set_count, mark_record[i], num_testcase);
            Copy_swarm(sw_pt, &(sw_new), i + set_count, mark_record[i], num_testcase);
        }

        if (0 == set_count) 
        {
            front_num = f_count;
        }

        set_count += f_count;
    }
    
    for (int i = 0; i < set_count; i++)
    {
        //Copy_swarm_pso(&(sw_new), sw_pt, i, i, num_testcase);
        Copy_swarm(&(sw_new), sw_pt, i, i, num_testcase);
    }

    Free_pso(&(sw_new));

    sw_pt->range = front_num;

    return front_num;
}

void Pso_update_pos(Swarms* sw_pt, Swarms* pso_sw_pt, int num_testcase)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        Copy_swarm_pso(sw_pt, pso_sw_pt, SWARMSIZE + i, SWARMSIZE + i, num_testcase);
    }
    for (int i = 0; i < SWARMSIZE; i++)
    {
        int temp = Domain_compare(pso_sw_pt, i, SWARMSIZE + i);
        if (temp == i) continue;
        else
        {
            Copy_swarm_pso(pso_sw_pt, pso_sw_pt, i, SWARMSIZE + i, num_testcase);
        }
    }
}

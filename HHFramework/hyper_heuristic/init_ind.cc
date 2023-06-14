/*
 * =====================================================================================
 *
 *       Filename:  init_ind.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/23/14 09:03:48
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
//#include <rand.h>

using namespace std;


void Revert_sequence(int *temp_sequence, int gen_num)
{//Revert the sequence
    int count = 0;
    for (int i = gen_num -1; i >= 0; i--)
    {
        temp_sequence[count] = i;
        count++;
    }
    return;
}

void Initial_individuals(Swarms* sw_pt, bool* CMatrix, float* learn_val ,int* learn_time,
        int num_testcase)
{
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen     = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch     = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 

    for (int i = 0; i < SWARMSIZE; i++)
    {//Initial the particle & speed  
        
        Init_sequence(&(sw_pt->particle[i * num_testcase]), num_testcase);
        //Revert_sequence(&(particle[i * num_testcase]), num_testcase);
        Shuffle(&(sw_pt->particle[i * num_testcase]), num_testcase, i);
    }
    
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->apsc[i]      = (float)MIN; 
        sw_pt->exetime[i]   = (float)MAX;
        sw_pt->distance[i]  = (float)MIN;
        sw_pt->disapsc[i]   = (float)MIN;
        sw_pt->disexet[i]   = (float)MAX;
        sw_pt->effnum[i]    = 0;
        sw_pt->efficient[i] = 0;
        sw_pt->newgen[i] = 0;
        sw_pt->batch[i] = 0;
    }

    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
    sw_pt->range = SWARMSIZE;


    for (int i = 0; i < STR; i++)
    {
        learn_val[i] = 0.0f;
        learn_time[i] = 0;
    }
    return;
}

void Initial_individuals_binary(Swarms* sw_pt, bool* CMatrix, float* learn_val, int* learn_time,
        int num_testcase)
{
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen     = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch     = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 

    for (int i = 0; i < SWARMSIZE; i++)
    {//Initial the particle & speed  
        
        Init_sequence_binary(&(sw_pt->particle[i * num_testcase]), num_testcase, i);
    }
    
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->apsc[i]      = (float)MIN; 
        sw_pt->exetime[i]   = (float)MAX;
        sw_pt->distance[i]  = (float)MIN;
        sw_pt->disapsc[i]   = (float)MIN;
        sw_pt->disexet[i]   = (float)MAX;
        sw_pt->effnum[i]    = 0;
        sw_pt->efficient[i] = 0;
        sw_pt->newgen[i] = 0;
        sw_pt->batch[i] = 0;
    }

    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
    sw_pt->range = SWARMSIZE;


    for (int i = 0; i < STR; i++)
    {
        learn_val[i] = 0.0f;
        learn_time[i] = 0;
    }
    return;
}


void Initial_individuals_pure(Swarms* sw_pt, Swarms* sw_pt_old, int num_testcase)
{
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen     = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch     = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 

    for (int i = 0; i < SWARMSIZE; i++)
    {//Initial the particle & speed  
        for (int j = 0; j < num_testcase; j++)
        {
            sw_pt->particle[i * num_testcase + j] =  
                sw_pt_old->particle[i * num_testcase + j];
        }
    }
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        sw_pt->apsc[i]      = sw_pt_old->apsc[i]; 
        sw_pt->exetime[i]   = sw_pt_old->exetime[i];
        sw_pt->distance[i]  = sw_pt_old->distance[i];
        sw_pt->disapsc[i]   = sw_pt_old->disapsc[i];
        sw_pt->disexet[i]   = sw_pt_old->disexet[i];
        sw_pt->effnum[i]    = sw_pt_old->effnum[i];
        sw_pt->efficient[i] = sw_pt_old->efficient[i];
        sw_pt->newgen[i]    = sw_pt_old->newgen[i]; 
        sw_pt->batch[i]     = sw_pt_old->batch[i];
    }

    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
    sw_pt->range = SWARMSIZE;

    return;
}


void Initial_individuals_pure_binary(Swarms* sw_pt, Swarms* sw_pt_old, int num_testcase)
{
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen     = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch     = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 

    for (int i = 0; i < SWARMSIZE; i++)
    {//Initial the particle & speed  
        for (int j = 0; j < num_testcase; j++)
        {
            sw_pt->particle[i * num_testcase + j] =  
                sw_pt_old->particle[i * num_testcase + j];
        }
    }
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        sw_pt->apsc[i]      = sw_pt_old->apsc[i]; 
        sw_pt->exetime[i]   = sw_pt_old->exetime[i];
        sw_pt->distance[i]  = sw_pt_old->distance[i];
        sw_pt->disapsc[i]   = sw_pt_old->disapsc[i];
        sw_pt->disexet[i]   = sw_pt_old->disexet[i];
        sw_pt->effnum[i]    = sw_pt_old->effnum[i];
        sw_pt->efficient[i] = sw_pt_old->efficient[i];
        sw_pt->newgen[i]    = sw_pt_old->newgen[i]; 
        sw_pt->batch[i]     = sw_pt_old->batch[i];
    }

    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
    sw_pt->range = SWARMSIZE;

    return;
}

void Initial_individuals_greedy(Swarms* sw_pt, int num_testcase)
{
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen     = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch     = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 

    for (int i = 0; i < SWARMSIZE * 2 * num_testcase; i++)
    {//Initial the particle & speed  
        
        sw_pt->particle[i] = 0;
    }
    
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->apsc[i]      = (float)MIN; 
        sw_pt->exetime[i]   = (float)MAX;
        sw_pt->distance[i]  = i;
        sw_pt->disapsc[i]   = (float)MIN;
        sw_pt->disexet[i]   = (float)MAX;
        sw_pt->effnum[i]    = 0;
        sw_pt->efficient[i] = 0;
        sw_pt->newgen[i] = 0;
        sw_pt->batch[i] = 0;
    }
    
    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
    sw_pt->range = SWARMSIZE;

    return;
}

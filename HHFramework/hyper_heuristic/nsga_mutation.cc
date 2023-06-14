/*
 * =====================================================================================
 *
 *       Filename:  nsga_mutuation.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/28/14 12:13:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

void Nsga_mutation_random(Swarms *sw_pt, int num_testcase)
{
    float MUT = MUTRATE;

    if (ALG == 4) MUT = 0.2;

    int num_rand = (int )(num_testcase * MUT * 2) + 2;

    int *array_rand = NULL;
    array_rand = (int*)malloc(sizeof(int) * num_testcase);
    int old_part = SWARMSIZE * num_testcase;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        srand((unsigned int)time(0) * (i + 1));
        for (int j = 0; j < num_testcase; j ++)
            array_rand[j] = rand() % num_testcase;
        
        for (int j = 0; j < num_rand; j += 2)
        {
            int pos1 = old_part + i * num_testcase + array_rand[j];
            int pos2 = old_part + i * num_testcase + array_rand[j + 1];

            int temp_record = sw_pt->particle[pos1];
            sw_pt->particle[pos1] = sw_pt->particle[pos2];
            sw_pt->particle[pos2] = temp_record;
        }
    }

    free(array_rand);
}

void Nsga_mutation_change_binary(Swarms *sw_pt, int num_testcase)
{
    float MUT = MUTRATE;

    if (ALG == 4) MUT = 0.1;

    int num_rand = (int )(num_testcase * MUT * 2);

    int *array_rand = NULL;
    array_rand = (int*)malloc(sizeof(int) * num_rand);
    int old_part = SWARMSIZE * num_testcase;
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        srand((unsigned int)time(0) * (i + 1));
        for (int j = 0; j < num_rand; j ++)
            array_rand[j] = rand() % num_testcase;
        
        for (int j = 0; j < num_rand; j += 2)
        {
            int pos1 = old_part + i * num_testcase + array_rand[j];
            int pos2 = old_part + i * num_testcase + array_rand[j + 1];
            
            int temp_record = sw_pt->particle[pos1];
            sw_pt->particle[pos1] = sw_pt->particle[pos2];
            sw_pt->particle[pos2] = temp_record;
        }
    }
    free(array_rand);
}


void Nsga_mutation_reverse_binary(Swarms* sw_pt, int num_testcase)
{
    float MUT = MUTRATE;

    if (ALG == 4) MUT = 0.1;

    int num_rand = (int )(num_testcase * MUT * 2);

    int *array_rand = NULL;
    array_rand = (int*)malloc(sizeof(int) * num_rand);
    int old_part = SWARMSIZE * num_testcase;
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        srand((unsigned int)time(0) * (i + 1));
        for (int j = 0; j < num_rand; j ++)
            array_rand[j] = rand() % num_testcase;
        
        for (int j = 0; j < num_rand; j++)
        {
            int pos1 = old_part + i * num_testcase + array_rand[j];
            
            int temp_record = sw_pt->particle[pos1];
            if (sw_pt->particle[pos1] == 0) sw_pt->particle[pos1] = 1;
            else  sw_pt->particle[pos1] = 0;
        }
    }
    free(array_rand);
}



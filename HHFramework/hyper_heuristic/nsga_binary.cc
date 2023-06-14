/*
 * =====================================================================================
 *
 *       Filename:  nsga_ii.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/03/15 15:18:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include "nsga_config.h"

#include "cal_fitness_kernel_binary.h"
#include "nsga_selection.h"
#include "nsga_crossover.h"
#include "nsga_crossover_binary.h"
#include "nsga_mutation.h"
#include "eliter_selection.h"
#include "check_sequence.h"
#include "check_front.h"
#include "learning.h"
#include "update_schema.h"

#include <iostream>

using namespace std;

int Nsga_II_binary(Swarms *sw_pt, Schema *sch_pt, int num_testcase, int code_line, bool* CMatrix, float* TVector, 
        bool* max_cover, int start, int g, int *change_cro, float* learn_val, int* learn_time, 
        int rand_seed)
{
    //if (g < STR) *change_cro = g;
    
    int *temp_ind = NULL;
    temp_ind = (int*) malloc(sizeof(int) * SWARMSIZE * 2);
    
    bool *temp_pos = NULL;
    temp_pos = (bool*) malloc(sizeof(bool) * SWARMSIZE * num_testcase);

    // For random num selection
    //if (TYPE == -2) *change_cro = rand_seed;
    //if (TYPE >= 0) *change_cro = TYPE;
    //cout << *change_cro << endl;
    
    switch(*change_cro)
    {
        case 0:
            Nsga_selection_rand(sw_pt, temp_ind);
            Nsga_crossover_serial_binary(sw_pt, temp_ind, num_testcase); break;
        case 1:
            Nsga_selection_rand(sw_pt, temp_ind);
            Nsga_crossover_singlepoint_binary(sw_pt, temp_ind, num_testcase); break; 
        case 2:
            Nsga_selection_rand_twodim(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_uniform_binary(sw_pt, temp_ind, temp_pos, num_testcase); break;
        //case 6:
            //Nsga_selection_repeat(sw_pt, sch_pt, temp_ind, temp_pos, num_testcase);
            //Nsga_crossover_antiscan(sw_pt, temp_ind, temp_pos, num_testcase); break;
        default:
            Nsga_copy(sw_pt, num_testcase);
    }
    
    //Check_sequence(sw_pt, num_testcase); getchar();

    free(temp_ind);
    free(temp_pos);

    //Nsga_mutation_change_binary(sw_pt, num_testcase);
    Nsga_mutation_reverse_binary(sw_pt, num_testcase);
    
    Cal_fitness_binary(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
    
    int range =  Eliter_selection(sw_pt, num_testcase);

    //int learn = Learning(sw_pt, *change_cro, learn_val, learn_time);

    //if (TYPE == -1) *change_cro = learn;

    //if (range != 1 || range != 2) Update_schema(sw_pt, sch_pt, range, num_testcase);

    return range;
}

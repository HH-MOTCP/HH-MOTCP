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

#include "cal_fitness_kernel.h"
#include "spea_selection.h"
#include "nsga_crossover.h"
#include "nsga_mutation.h"
#include "eliter_selection.h"
#include "check_sequence.h"

#include <iostream>

using namespace std;

int Spea(Swarms *sw_pt, Swarms *new_sw_pt, int num_testcase, int code_line, bool* CMatrix, float* TVector, 
        bool* max_cover, int start, int front_0, int *change_cro)
{
    int *temp_ind = NULL;
    temp_ind = (int*) malloc(sizeof(int) * SWARMSIZE * 2);
    
    bool *temp_pos = NULL;
    temp_pos = (bool*) malloc(sizeof(bool) * SWARMSIZE * num_testcase);
 
    switch(*change_cro)
    {
        case 0:
            Spea_selection(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_serial(sw_pt, temp_ind, num_testcase); break;
        case 1:
            Spea_selection(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_singlepoint(sw_pt, temp_ind, num_testcase); break;
        case 2:
            Spea_selection(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_antsinglepoint(sw_pt, temp_ind, num_testcase); break;
        case 3:
            Spea_selection(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_scan(sw_pt, temp_ind, temp_pos, num_testcase); break;
        case 4:
            Spea_selection(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_antiscan(sw_pt, temp_ind, temp_pos, num_testcase); break;
        case 5:
            Spea_selection(sw_pt, temp_ind, temp_pos, num_testcase);
            Nsga_crossover_uniform(sw_pt, temp_ind, temp_pos, num_testcase); break;
        default:
            Nsga_copy(sw_pt, num_testcase);
    }
    //Check_sequence(sw_pt, num_testcase);
    //cout << "Check" << endl;
    
    free(temp_ind);
    free(temp_pos);

    Nsga_mutation_random(sw_pt, num_testcase);

    Cal_fitness_tcp(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
    
    int eliter = Eliter_selection_spea(sw_pt, num_testcase);
        
    return eliter;
}

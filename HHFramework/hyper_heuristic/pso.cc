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
#include "pso_selection.h"
#include "pso_crossover_nor.h"
#include "nsga_crossover.h"
#include "nsga_mutation.h"
#include "pso_eliter_selection.h"
#include "check_sequence.h"

#include <iostream>

using namespace std;

int Pso(Swarms *sw_pt, Swarms *pso_sw_pt, int num_testcase, int code_line, bool* CMatrix, 
        float* TVector, bool* max_cover, int start, int front_0)
{
                
    int *temp_ind = NULL;
    temp_ind = (int*) malloc(sizeof(int) * SWARMSIZE * 2);
    
    bool *temp_pos = NULL;
    temp_pos = (bool*) malloc(sizeof(bool) * SWARMSIZE * num_testcase);
    
    //int count = Pso_selection(sw_pt, temp_ind);
    //Pso_selection(sw_pt, temp_ind, temp_pos, front_0, num_testcase);
    
    //Pso_crossover_nor(sw_pt, temp_ind, num_testcase, count);
            
    //Nsga_crossover_serial(sw_pt, temp_ind, num_testcase);
    //Nsga_crossover_singlepoint(sw_pt, temp_ind, num_testcase); 
    //Nsga_crossover_antsinglepoint(sw_pt, temp_ind, num_testcase);
    //Nsga_crossover_scan(sw_pt, temp_ind, temp_pos, num_testcase); 
    //Nsga_crossover_antiscan(sw_pt, temp_ind, temp_pos, num_testcase);
    //Nsga_crossover_uniform(sw_pt, temp_ind, temp_pos, num_testcase); 
    //Nsga_copy(sw_pt, num_testcase);
    
    Pso_selection(pso_sw_pt, temp_ind, temp_pos, front_0, num_testcase);
    
    Nsga_crossover_uniform(pso_sw_pt, temp_ind, temp_pos, num_testcase); 
    
    Pso_selection(pso_sw_pt, temp_ind, temp_pos, front_0, num_testcase);
    
    Pso_crossover_uniform(sw_pt, pso_sw_pt, temp_pos, num_testcase); 
    
    free(temp_ind);
    free(temp_pos);
    
    //Check_sequence(sw_pt, num_testcase);
    //getchar();

    //Nsga_mutation_random(sw_pt, num_testcase);

    Cal_fitness_tcp(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
    
    Pso_update_pos(sw_pt, pso_sw_pt, num_testcase);
    
    int eliter = Pso_eliter_selection(sw_pt, pso_sw_pt, num_testcase);
    
    return eliter;
            
}

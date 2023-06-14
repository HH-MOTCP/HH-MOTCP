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
#include "nsga_crossover.h"
#include "nsga_mutation.h"
#include "eliter_selection.h"
#include "check_sequence.h"

#include <iostream>

using namespace std;

int Paes(Swarms *sw_pt, int num_testcase, int code_line, bool* CMatrix, float* TVector, 
        bool* max_cover, int start, int front_0, int *change_cro)
{
    Nsga_copy(sw_pt, num_testcase);
    
    Nsga_mutation_random(sw_pt, num_testcase);

    Cal_fitness_tcp(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
    
    int eliter = Eliter_selection_taea(sw_pt, num_testcase);

    return eliter;
}

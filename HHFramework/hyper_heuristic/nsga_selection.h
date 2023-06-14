/*
 * =====================================================================================
 *
 *       Filename:  nsga_crossover.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/22/14 10:38:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _NSGA_SELECTION_H_
#define _NSGA_SELECTION_H_

void Nsga_selection_rand(Swarms* sw_pt, int *temp_ind);

void Nsga_selection_rand_twodim(Swarms* sw_pt, int *temp_ind, bool *temp_pos,
        int num_testcase);

void Nsga_selection_repeat(Swarms* sw_pt, Schema* sch_pt, int *temp_ind, bool *temp_pos,
        int num_testcase);

#endif 


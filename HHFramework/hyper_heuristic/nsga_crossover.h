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

#ifndef _NSGA_CROSSOVER_H_
#define _NSGA_CROSSOVER_H_

void Nsga_crossover_singlepoint(Swarms* sw_pt, int *temp_ind, int num_testcase);

void Nsga_crossover_antsinglepoint(Swarms* sw_pt, int *temp_ind, int num_testcase);

void Nsga_crossover_antiscan(Swarms* sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase);

void Nsga_crossover_uniform(Swarms* sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase);

void Pso_crossover_uniform(Swarms* sw_pt, Swarms* pso_sw_pt, bool *temp_pos, int num_testcase);

void Nsga_crossover_scan(Swarms* sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase);

void Nsga_crossover_serial(Swarms* sw_pt, int *temp_ind, int num_testcase);

void Nsga_copy(Swarms* sw_pt, int num_testcase);

#endif 


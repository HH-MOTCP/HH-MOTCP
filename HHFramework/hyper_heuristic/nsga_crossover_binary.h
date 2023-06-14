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

#ifndef _NSGA_CROSSOVER_BINARY_H_
#define _NSGA_CROSSOVER_BINARY_H_

void Nsga_crossover_singlepoint_binary(Swarms* sw_pt, int *temp_ind, int num_testcase);

void Nsga_crossover_uniform_binary(Swarms* sw_pt, int *temp_ind, bool *temp_pos, 
        int num_testcase);

void Pso_crossover_uniform_binary(Swarms* sw_pt, Swarms* pso_sw_pt, bool *temp_pos, int num_testcase);

void Nsga_crossover_serial_binary(Swarms* sw_pt, int *temp_ind, int num_testcase);

void Nsga_copy_binary(Swarms* sw_pt, int num_testcase);

#endif 


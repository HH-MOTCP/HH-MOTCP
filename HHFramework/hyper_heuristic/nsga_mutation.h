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

#ifndef _NSGA_MUTUATION_H_
#define _NSGA_MUTUATION_H_

void Nsga_mutation_random(Swarms* sw_pt, int num_testcase);
//void Nsga_mutation(Swarms* sw_pt, int num_testcase);

void Nsga_mutation_change_binary(Swarms* sw_pt, int num_testcase);

void Nsga_mutation_reverse_binary(Swarms* sw_pt, int num_testcase);

#endif 


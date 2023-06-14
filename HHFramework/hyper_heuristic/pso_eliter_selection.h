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

#ifndef _PSO_ELITER_SELECTION_H_
#define _PSO_ELITER_SELECTION_H_

int Pso_eliter_selection(Swarms* sw_pt, Swarms* pso_sw_pt, int num_testcase);

void Pso_update_pos(Swarms* sw_pt, Swarms* pso_sw_pt, int num_testcase);

//void Copy_swarm(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase);

#endif 


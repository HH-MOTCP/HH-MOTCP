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

#ifndef _ELITER_SELECTION_H_
#define _ELITER_SELECTION_H_

int Eliter_selection(Swarms* sw_pt, int num_testcase);

void Eliter_selection_new(Swarms* sw_cp, Swarms* sw_pt, int num_testcase);

int Eliter_selection_taea(Swarms* sw_pt, int num_testcase);

int Eliter_selection_spea(Swarms* sw_pt, int num_testcase);

//void Copy_swarm(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase);

#endif 


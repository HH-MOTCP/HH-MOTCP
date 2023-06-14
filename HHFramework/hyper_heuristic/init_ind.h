/*
 * =====================================================================================
 *
 *       Filename:  read_file.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/14 10:39:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */



#ifndef _INIT_IND_H_
#define _INIT_IND_H_

void Initial_individuals(Swarms* sw_pt, bool* CMatirx, float* learn_val , int* learn_time,
        int num_testcase);

void Initial_individuals_binary(Swarms* sw_pt, bool* CMatirx, float* learn_val, int* learn_time,
        int num_testcase);

void Initial_individuals_pure(Swarms* pso_sw_pt, Swarms* sw_pt, int num_testcase);

void Initial_individuals_pure_binary(Swarms* pso_sw_pt, Swarms* sw_pt, int num_testcase);

void Initial_individuals_greedy(Swarms* sw_pt, int num_testcase);

#endif



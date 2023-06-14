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

#ifndef _NSGA_II_BINARY_H_
#define _NSGA_II_BINARY_H_

int Nsga_II_binary(Swarms* sw_pt, 
        Schema* sch_pt,
        int num_testcase,
        int code_line,
        bool* CMatrix,
        float* TVector,
        bool* max_cover,
        int start,
        int g,
        int *change_cro,
        float* learn_val,
        int* learn_time,
        int rand_seed);

#endif 


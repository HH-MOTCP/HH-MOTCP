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

#ifndef _SPEA_H_
#define _SPEA_H_

int Spea(Swarms* sw_pt, 
        Swarms* new_sw_pt,
        int num_testcase,
        int code_line,
        bool* CMatrix,
        float* TVector,
        bool* max_cover,
        int start,
        int front_0,
        int *change_cro);

#endif 


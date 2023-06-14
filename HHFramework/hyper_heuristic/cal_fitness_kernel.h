/*
 * =====================================================================================
 *
 *       Filename:  cal_fitness.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/07/14 11:19:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
//#ifndef _PSO_CONFIG_H_
//#define _PSO_CONFIG_H_

//#include "pso_config.h"

//#endif

#ifndef _CAL_FITNESS_CU_
#define _CAL_FITNESS_CU_
//#include "cal_fitness_kernel.cu"


void Cal_fitness_tcp(Swarms* sw_pt, 
        int num_testcase, 
        int code_line,
        bool* CMatrix, 
        float* TVector,
        bool* max_cover,
        int start);

#endif

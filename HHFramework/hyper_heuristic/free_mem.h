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



#ifndef _FREE_MEM_H_
#define _FREE_MEM_H_

void Free_memory(bool* CMatrix, float* TVector, bool* max_cover, float* learn_val, 
        int* learn_time);

void Free_swarm(Swarms* sw_pt); 

void Free_schema(Schema* sch_pt);

#endif



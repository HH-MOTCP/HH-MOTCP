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



#ifndef _COPY_SWARM_H_
#define _COPY_SWARM_H_

extern void Copy_swarm(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase);
extern void Copy_swarm_samebatch(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase);

#endif



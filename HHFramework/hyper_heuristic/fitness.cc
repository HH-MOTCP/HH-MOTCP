/*
 * =====================================================================================
 *
 *       Filename:  learning.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/03/15 12:25:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
float g_count;//sw_pt的pareto解集的超体积值
float g_count_div;
float distance_all;
float distance_avg;
float distance_m[100];//用于保存新一代种群的pareto front上的每个解与离其最近的解的曼哈顿距离


using namespace std;

float ffabs(float a){
	if(a >= 0.0f){
		return a;
	}
	else{
		return -a;
	}
}
	
float Cal_g_count(Swarms* sw_pt){
	
	g_count = 0.0f;
	
	g_count = g_count + sw_pt->disexet[0] * (1 - sw_pt->disapsc[0]);
	for( int i = 1; i < sw_pt->range; i++ ){				
		g_count = g_count + (sw_pt->disexet[i] - sw_pt->disexet[i - 1]) * (1 - sw_pt->disapsc[i]);		
	}	
	 	 
	return g_count;
}

float Cal_g_count_div(Swarms* sw_pt){
	
	g_count_div = 0.0f;
	distance_all = 0.0f;
	distance_avg = 0.0f;
	
	for(int i = 0; i < sw_pt->range; i++){
		distance_m[i] = 0.0f;
	}
	
	if(sw_pt->range != 1){
		for(int i = 0; i < sw_pt->range; i++){
		    float minDistance = float(MAX);
		    for(int j = 0; j < sw_pt->range; j++){
				if(j != i){		                    			
			        if(ffabs(sw_pt->distance[j] - sw_pt->distance[i]) < minDistance){
				       minDistance = ffabs(sw_pt->distance[j] - sw_pt->distance[i]);                       					  					   
			        }
				}
				
		    }
		    distance_m[i] = minDistance;				           					
	    }
	   
	   for(int i = 0; i < sw_pt->range; i++){	             
		  distance_all += distance_m[i]; 		  
	   }          
	   distance_avg = distance_all/sw_pt->range;	
      	   
	   for(int i = 0; i < sw_pt->range; i++){
		  g_count_div += (distance_m[i] - distance_avg) * (distance_m[i] - distance_avg); 
	   }
	   g_count_div = sqrt(g_count_div / sw_pt->range);
	}					    					  
	return g_count_div;
}

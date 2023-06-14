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
#include "domain_judge.h"
#include "eliter_selection.h"
#include "fitness.h"
#include "copy_swarm.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

using namespace std;

int Learning(Swarms* sw_pt, Swarms* sw_cp, int change_alg, int change_cro, float* learn_val, int* learn_time, int num_testcase,
         int g, int loops)   //sw_cp是上一代种群,sw_pt是经过处理(遗传算法）后产生的新一代种群,已经被分层
{
	
    Eliter_selection_new(sw_cp, sw_pt, num_testcase);
	
	
    float g_count = 0.0f;  
	float max_disexet = sw_cp->exetime[SWARMSIZE - 1];
	
	
    float ss[SWARMSIZE * 2];  
    //float tt[SWARMSIZE * 2];  
    for(int i=0;i<=(int)max_disexet;i++)
    {
       ss[i]=0.0;
       //tt[i]=0.0;
    }
	//float ss = 0.0f;
	//float tt = 0.0f;
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (sw_cp->newgen[i] == 1)
        {
			ss[(int)sw_cp->exetime[i]] += sw_cp->disapsc[i] + 1 - sw_cp->disexet[i];
			//g_count += sw_cp->disapsc[i] + 1 - sw_cp->disexet[i];
	                
        }
        //tt[(int)sw_cp->exetime[i]] += sw_cp->disapsc[i] + 1 - sw_cp->disexet[i];		
		//tt[(int)sw_cp->exetime[i]] += sw_cp->apsc[i];
		//tt += sw_cp->disapsc[i] + 1 - sw_cp->disexet[i];
    }
	
	for(int i=0;i<=(int)max_disexet;i++)
		if(max_disexet == 0)
			g_count += ss[i] * (max_disexet + 1 - i) / (max_disexet + 1);
		else
			g_count += ss[i] * (max_disexet - i) / max_disexet;
	//g_count = ss / tt;
		      	
    int strategy = change_alg * CRO_NUM + change_cro;
	learn_time[strategy]++;
	
	if(learn_time[strategy] == 1){		
		learn_val[strategy] = g_count;
	}
	else{	
		learn_val[strategy] = (learn_val[strategy] + g_count) / 2;		
	}
	
	float max = learn_val[0];
    int num = 0;
    
    for (int i = 1; i < STR; i++) 
    {               
        if (learn_val[i] > max) 
        {
            max = learn_val[i];
            num = i;
        }
    }
	
	for (int i = 0; i < STR; i++) 
    {
        cout<<learn_val[i]<<" ";
    }
    cout<<endl;
	
	for (int i = 0; i < STR; i++) 
    {
        cout<<learn_time[i]<<" ";
    }
    cout<<endl;
 
    if (g < 17)
    {
        num = (strategy + 1) % STR;
    } 
    	
    cout << "check * " << num << " *" << endl << endl;
    return num;
}


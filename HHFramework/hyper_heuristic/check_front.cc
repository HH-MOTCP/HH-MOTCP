/*
 * =====================================================================================
 *
 *       Filename:  check_front.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/29/14 11:36:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"
#include "domain_judge.h"
#include <iostream>

using namespace std;

int Check_front(Swarms *sw_pt, int num_testcase, int front_range)  //三个参数分别表示种群、测试用例数、前沿个体数 
{
    for (int i = 0; i < front_range; i++) sw_pt->efficient[i] = 0;

    int front_count = 0;
    for (int i = 0; i < front_range; i++)
    {
        bool dom_mark = 0;
        
        if (sw_pt->efficient[i] == 1) 
        {
            dom_mark = 1;
            continue;
        }
        

        for(int j = i + 1; j < front_range; j++)
        {
            if(sw_pt->efficient[j] == 1) continue;     

            int judge = Domain_compare(sw_pt, i, j);

            if (judge == -1) continue;            //judge==-1，表示个体i和j互不支配 
            else if (judge == i || judge == -2)
            {
                if (judge == -2) front_count++;   //judge==-2，表示个体i和个体j在所有目标上都是一致的 
                sw_pt->efficient[j] = 1;         //i支配j 
            }
            else
            {
                sw_pt->efficient[i] = 1;      //j支配i 
                dom_mark = 1;
                break;
            }
        }
        //if (dom_mark == 0) front_count++;
    }

    return front_count;      //所以返回的是在所有目标上都相同的个体数？？？ 
}

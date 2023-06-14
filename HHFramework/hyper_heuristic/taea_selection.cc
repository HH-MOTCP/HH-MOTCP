/*
 * =====================================================================================
 *
 *       Filename:  nsga_selection.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/15 16:13:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"
#include <iostream>
#include "domain_judge.h"

using namespace std;

void Taea_selection(Swarms *sw_pt, int num_testcase)
{
    int POS = SWARMSIZE * num_testcase;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++)
        {
            sw_pt->particle[POS + i * num_testcase + j] = 
                sw_pt->particle[i * num_testcase + j];
        }
    }

    /*  
    int count = 0;

    for (int i = 0; i < SWARMSIZE; i++) sw_pt->efficient[i] = 0;
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (sw_pt->efficient[i] == 1) continue;

        for (int j = i + 1; j < SWARMSIZE; j++)
        {
            if (sw_pt->efficient[j] == 1) continue;

            int judge = Domain_compare(sw_pt, i, j);
            
            if (judge == -1) continue;
            else if (judge == i)
            {
                sw_pt->efficient[j] = 1;    
                continue;
            }
            else
            {
                sw_pt->efficient[i] = 1;
                break;
            }
        }

        if (sw_pt->efficient[i] == 0)
        {
            temp_ind[count] = i;
            count++;
        }
    }
    */

    return;
}


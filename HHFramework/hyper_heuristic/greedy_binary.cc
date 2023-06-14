/*
 * =====================================================================================
 *
 *       Filename:  greedy.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/06/15 16:33:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <iostream>
#include "nsga_config.h"
#include "eliter_selection.h"

using namespace std;

int Greedy_binary(Swarms *sw_pt, bool* CMatrix, float* TVector, bool* Max_cover, 
        int num_testcase, int code_line, double* runtime)
{
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->exetime[i] = 0;
        sw_pt->effnum[i] = 0;

        clock_t gstart, gend;
        gstart = clock();

        bool* mark_testcase = NULL;
        mark_testcase = (bool*) malloc(sizeof(bool) * num_testcase);
        for (int j = 0; j < num_testcase; j++) mark_testcase[j] = 0;

        bool* mark_line = NULL;
        mark_line = (bool*) malloc(sizeof(bool) * code_line);
        for (int j = 0; j < num_testcase; j++) mark_line[j] = 0;

        srand((unsigned int)time(0) * (i + 1));
        int position = 0;
        
        while(Check(Max_cover, mark_line, code_line) == 0)
        {
            int add = rand() % num_testcase;
            for (int j = 0; j < num_testcase; j++)
            {
                int temp = (position + add + j) % num_testcase;
                if (mark_testcase[temp] == 1)
                {
                    continue;
                }
                else
                {
                    position = temp;
                    mark_testcase[temp] = 1;
                    break;
                }
            }
            int contri = 0;
            for (int j = 0; j < code_line; j++)
            {
                if (Max_cover[j] == 0 || mark_line[j] == 1) continue;
                else
                {
                    if (CMatrix[position * code_line + j] == 1)
                    {
                        contri = 1;
                        mark_line[j] = 1;
                    }
                }
                
            }

            if (contri == 1)
            {
                sw_pt->exetime[i] += TVector[position];
                sw_pt->effnum[i] += 1;
                sw_pt->particle[i * num_testcase + position] = 1;
            }

        }
        
        free(mark_line);
        free(mark_testcase);

        gend = clock();
        runtime[i] = (double) (gend - gstart) / CLOCKS_PER_SEC;
    }
    
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->apsc[i] = 1.0 - runtime[i];
    }
        
    int front_0 = Eliter_selection(sw_pt, num_testcase);

    sw_pt->range = front_0;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        cout << i << " " << sw_pt->exetime[i] << endl;
    }

    return front_0;

}

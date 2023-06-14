/*
 * =====================================================================================
 *
 *       Filename:  check_sequence.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/03/15 12:37:33
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

using namespace std;

void Check_sequence(Swarms *sw_pt, int num_testcase)         //避免个体(序列)中有重复的测试用例 
{
    int error_mark = 0;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        int *check = NULL;
        check = (int*) malloc(sizeof(int) * num_testcase);
        for (int j = 0; j < num_testcase; j++) check[j] = 0;
        

        for (int j = 0; j < num_testcase; j++)
        {
            int temp = sw_pt->particle[i * num_testcase + j];    //表示第i+1个个体中的第j+1个测试用例的序号 
            check[temp] += 1;         //当一个序列中出现重复的测试用例时，check[temp]!=1,会>1 
        }

        for (int j = 0; j < num_testcase; j++)
        {
            if (check[j] != 1)
            {
                cout << "Old sequence Ind: " << i << "  Pos: " << j << " error" << endl;
                error_mark = 1;
                break;
            }
        }
    }

    if (error_mark == 0) cout << "Old sequence OK!" << endl;
    
    error_mark = 0;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        int *check = NULL;
        check = (int*) malloc(sizeof(int) * num_testcase);
        for (int j = 0; j < num_testcase; j++) check[j] = 0;
        

        for (int j = 0; j < num_testcase; j++)
        {
            int temp = sw_pt->particle[SWARMSIZE * num_testcase + i * num_testcase + j];
            check[temp] += 1;
        }

        for (int j = 0; j < num_testcase; j++)
        {
            if (check[j] != 1)
            {
                cout << "New sequence Ind: " << i << "  Pos: " << j << " error" << endl;
                error_mark = 1;
                break;
            }
        }
    }
    
    if (error_mark == 0) cout << "New sequence OK!" << endl;

    return;
}

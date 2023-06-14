/*
 * =====================================================================================
 *
 *       Filename:  nsga_config.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/13/14 09:53:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

//#include "nsga_config.h"
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;


extern const string prog_name = "commons-cli"; 
//Project Name

extern const string input_path = "../../new_total_cover/";
//extern const string input_path = "../../" + prog_name + "/";
extern const string cover_file = prog_name + ".cov";
//extern const string cover_file = "_" + prog_name + "_subSuite.out";
extern const string eet_file = prog_name + ".time";
//extern const string eet_file = "_" + prog_name + "_time.out";
extern const string output_path = "../learn_output/";

extern void Init_sequence(int *temp_sequence, int gen_num)     //第一个参数表示个体paticle，第二个参数表示测试用例的个数 																    
{ 
    for (int i = 0; i < gen_num; i++)   						//初始化首先生成1到n的有序测试用例序列
    {
        temp_sequence[i] = i;
    }
    return;
}

extern void Init_sequence_binary(int *temp_sequence, int gen_num, int loop)
{
    srand((unsigned int)time(0) * loop);
    for (int i = 0; i < gen_num; i++)
    {
        int temp_rand = rand();
        //temp_sequence[i] = temp_rand % 2;
        int num = temp_rand % 10;
        if (num < 2) 
        {
            temp_sequence[i] = 1;
        }
        else
            temp_sequence[i] = 0;
    }
    return;
}


extern void Shuffle(int *temp_sequence, int gen_num, unsigned int loop_count)     // 然后根据随机生成位置交换
{
    srand((unsigned int)time(0) * loop_count);                                 
    for (int i = gen_num - 1; i > 0; i--)
    {
        int rand_num = rand()%i;
        int temp = temp_sequence[i];
        temp_sequence[i] = temp_sequence[rand_num];
        temp_sequence[rand_num] = temp;
    }

    return;
}


extern int Check(bool* max_cover, bool* mark_line, int code_line)
{
    int finish = 1;

    for (int i = 0; i < code_line; i++)
    {
        if (max_cover[i] == 0) continue;   //max_cover[i] == 0表示第i行代码无任何测试用例覆盖 
        else
        {
            if (mark_line[i] == 0)
            {
                finish = 0;
                break;
            }
        }
    }

    return finish;
}

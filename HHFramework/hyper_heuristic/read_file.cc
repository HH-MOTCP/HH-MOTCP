/*
 * =====================================================================================
 *
 *       Filename:  read_file.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/14 10:57:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


//#include "file_path.h"
#include "nsga_config.h"
#include <iostream>
#include <fstream>

using namespace std;

void Read_file(int s, bool* CMatrix, float* TVector, bool* Max_cover, int num_testcase,
        int code_line)
{  
    stringstream ss;
    ss << s;
    string serial_num = ss.str();

    //orignal file
    //string coverage_file_path = input_path + serial_num + cover_file;
    //string eet_file_path = input_path + serial_num + eet_file;
    string coverage_file_path = input_path + cover_file;
    string eet_file_path = input_path + eet_file;

    //big file
    //string coverage_file_path = "../all_clean.cov";
    //string eet_file_path = "../all.time";
    
    const char *cover_inf = coverage_file_path.c_str();
    const char *eet_inf = eet_file_path.c_str();

    FILE* file_cover = NULL;
    FILE* file_eet = NULL;

    file_cover = fopen(cover_inf, "r");
    if (NULL == file_cover) cout<<"Error at read file_cover" <<endl;

    file_eet = fopen(eet_inf, "r");
    if (NULL == file_eet) cout<<"Error at read file_eet" <<endl;

    int real_gen = 0;
    while (!feof(file_eet))
    {
        fscanf(file_eet, "%f", &TVector[real_gen]);
        real_gen++;
    }

    for (int i = 0; i < code_line; i++)
        Max_cover[i] = 0;

    int position = 0;
    char ch;
    int line = 0;

    /*  
    ifstream cover;
    cover.open(cover_inf);

    string line_cover;
    while (getline(cover, line_cover))
    {
        for (int i = 0; i < line_cover.length(); i++)
        {

            if (line_cover[i] == '0')
            {
                CMatrix[line * code_line + i] = 0;
                continue;
            }
            else
            {
                CMatrix[line * code_line + i] = 1;
                Max_cover[i] = 1;
                continue;
            }
        }

        line += 1;
    }
        
    */  

    while ((ch = fgetc(file_cover)) != EOF)
    {   
        switch (ch)
        {   
            case '\n':
                position = 0;
                line += 1;
                break;
            case '1':
                CMatrix[line * code_line + position] = 1; //coverage matrix
                Max_cover[position] = 1;                 //为1说明改行代码有测试用例覆盖，为0说明无测试用例覆盖 
                position ++;
                continue;
            default:
                CMatrix[line * code_line + position] = 0;
                position ++;
                continue;
        }   
    }


    
    //cover.close();

    fclose(file_cover);
    fclose(file_eet);

    return;
}

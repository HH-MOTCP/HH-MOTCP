/*
 * =====================================================================================
 *
 *       Filename:  check_file.cc
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

using namespace std;


void Check_file(int s, int* num_testcase, int* code_line)
{

    stringstream ss;
    ss << s;
    string serial_num = ss.str();

    //orignal path
    //string coverage_file_path = input_path + serial_num + cover_file;
    //string eet_file_path = input_path + serial_num + eet_file;
    string coverage_file_path = input_path + cover_file;
    string eet_file_path = input_path + eet_file;

    cout << coverage_file_path << endl;
    cout << eet_file_path << endl;


    //full code
    //coverage_file_path = "../all_clean.cov";
    //eet_file_path = "../all.time";

    const char *cover_inf = coverage_file_path.c_str();
    const char *eet_inf = eet_file_path.c_str();

    FILE* file_cover = NULL;
    FILE* file_eet = NULL;

    file_cover = fopen(cover_inf, "r");
    if (NULL == file_cover) cout<<"Error at read file_cover" <<endl;

    file_eet = fopen(eet_inf, "r");
    if (NULL == file_eet) cout<<"Error at read file_eet" <<endl;


    int real_gen = 0;
    float ftemp;
    
    while (!feof(file_eet))
    {
        fscanf(file_eet, "%f", &ftemp);
        ++real_gen;
    }
    real_gen -= 1;

    int position = 0;
    char ch;
    //int line = 0;
    while ((ch = fgetc(file_cover)) != EOF)
    {   
        if (ch == '\n')
        {
            break;
        }
        position ++;
    }
    position -= 1;

    fclose(file_cover);
    fclose(file_eet);

    *num_testcase = real_gen;
    *code_line = position;

    return;
}

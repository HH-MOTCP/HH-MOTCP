/*
 * =====================================================================================
 *
 *       Filename:  file_path.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/20/14 10:55:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _FILE_PAHT_H_
#define _FILE_PATH_H_

#include <string.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
//For file path
const string prog_name = "v8";

string input_path = "../" + prog_name + "_5_subsuite/";
string cover_file = "_" + prog_name + "_5_subSuite.out";
string eet_file = "_" + prog_name + "_5_time.out";

const string output_path = "./output/";

string front_improved_file = output_path + "front_improve/" + 
    prog_name + "_200p_front_improved.xls";
string front_file = output_path + "front/" + prog_name + 
    "100P_300G_front.xls";
string time_file = output_path + "time/" + prog_name + 
    "100P_300G_front.xls";

#endif

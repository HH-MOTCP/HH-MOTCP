/*
 * =====================================================================================
 *
 *       Filename:  read_file.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/14 10:39:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */



#ifndef _READ_FILE_H_
#define _READ_FILE_H_
#include <iostream>
//int Read_file(FILE , FILE );
//int Read_file(FILE *file_cover, FILE *file_eet, bool *CMatrix, 
//        float *TVector, bool *Max_cover);
void Read_file(int s, bool* CMatrix, float* TVector, bool* Max_cover, int num_test,
        int code_line);

#endif



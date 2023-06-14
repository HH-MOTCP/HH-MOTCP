/*
 * =====================================================================================
 *
 *       Filename:  main_c.c
 *
 *    Description:  For Hyper Heristic Evolution Algorithm
 *
 *        Version:  1.0
 *        Created:  05/03/15 16:11:56
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  YOUR NAME (Yi Bian), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"

bool* CMatrix = NULL;
float* TVector = NULL;
bool* max_cover = NULL;
float* learn_val = NULL;
int* learn_time = NULL;

Swarms swarm;
Schema schema;

#include "read_file.h"
#include "init_ind.h"
#include "init_sch.h"
#include "check_file.h"
#include "cal_fitness_kernel.h"
#include "nsga_ii.h"
#include "taea.h"
#include "pso.h"
#include "free_mem.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main()
{
    Swarms* sw_pt = &swarm; //Create the swarm
    Schema* sch_pt = &schema;


    //for (int s = 1; s <= SUITENUM; s++)
    for (int s = 1; s <= 1; s++)
    {
        string ss = s.str();
        string file_path = output_path + prog_name + "/" + ss + "/";

        string apsc_str = file_path + "rand_a_" + prog_name + ".out";
        string sequ_str = file_path + "sequ_a_" + prog_name + ".out";
        string tota_str = file_path + "tota_a_" + prog_name + ".out";
        string mult_str = file_path + "mult_a_" + prog_name + ".out";
        
            
        iofstream fapsc, fsequ, ftota, fmult;
        fapsc.open(apsc_str, ios::app);
        fsequ.open(sequ_str, ios::app);
        ftota.open(tota_str, ios::app);
        fmult.open(mult_str, ios::app);
       
        ifstream fschema;
        fschema.open("./schema_new.out");
        
        cout << "The number " << s << " test suite" << endl;
        cout << endl;
        
        int num_testcase = 0;
        int code_line = 0;
        
        //Check the basic information of test case
        Check_file(s, &num_testcase, &code_line);
        cout << "test cases number: " << num_testcase << endl
             << "code lines:        " << code_line << endl;
        

        CMatrix = (bool*) malloc(sizeof(bool) * num_testcase * code_line);
        if (CMatrix == NULL) { cout << "CMatrix malloc error! " << endl;}

        TVector = (float*) malloc(sizeof(float) * num_testcase);
        if (TVector == NULL) { cout << "TVector malloc error! " << endl;}

        max_cover = (bool*) malloc(sizeof(bool) * code_line);
        if (max_cover == NULL) { cout << "max_cover malloc error! " << endl;}
        
        learn_val = (float*) malloc(sizeof(float) * STR);
        if (learn_val == NULL) { cout << "learn_val malloc error! " << endl;}

        learn_time = (int*) malloc(sizeof(int) * STR);
        if (learn_time == NULL) { cout << "learn_time malloc error! " << endl;}

        //Read the coverage matrix and time array
        Read_file(s, CMatrix, TVector, max_cover, num_testcase, code_line);
         
        
        //Initial the individuals
        Initial_individuals(sw_pt, CMatrix, learn_val, learn_time, num_testcase);

        //Initial the schema
        Initial_schema(sch_pt, CMatrix, num_testcase, code_line);

        bool mark[2000];
        for (int i = 0; i < 2000; i++) mark[i] = 0;
        int temp = 0;
        int count = 0;
        while (fschema >> temp)
        {
            mark[temp] = 1;
            count++;
        }
        //cout << "all ele: " << count << endl;
        //cout << "schema : " << sch_pt->map_size << endl;


        //Calculate the fitness first time
        int start = 0;
        Cal_fitness(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
     
        int front_0 = 0;
        int change_cro = 0;

        //For selection random number
        srand(time(NULL));
        int rand_seed[GENERATION];
        for (int i = 0; i < GENERATION; i++) rand_seed[i] = rand() % STR;
       
        int c_a = 0, c_t = 0;
        int c_sum = 0;
        int c_judge = 0;

        clock_t begin, end;

        begin = clock();

        for(int g = 0; g < GENERATION; g++)
        {
            //cout << "Num " << g << " loop.  " << endl;

            clock_t in_begin, in_end;
            
            in_begin = clock();
            int start = SWARMSIZE;
            
            //front_0 = Nsga_II(sw_pt, sch_pt, num_testcase, code_line, CMatrix, TVector, max_cover, 
            //        start, g, &change_cro, learn_val, learn_time, rand_seed[g]);
            
            front_0 = Pso(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, front_0);
             
            //front_0 = Taea(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);

            for (int i = 0; i < front_0; i++)
            {
                int sch_num = 0;
                int sch_num1 = 0;
    
                bool sub_mark[2000];
                for (int j = 0; j < 2000; j++)
                {
                    sub_mark[j] = mark[j];
                }
                for (int j = 0; j < sw_pt->effnum[i]; j++)
                {
                    int mir = sch_pt->matrix_map[sw_pt->particle[i * num_testcase + j]];
                    if (sub_mark[mir] == 1)
                    {
                        sch_num += 1;
                        sub_mark[mir] = 0;
                    }
                    if (mark[mir] == 1)
                    {
                        sch_num1 += 1;
                    }
                }
                //cout << i << " only :  " << sch_num << endl; 
                //cout << i << " repe :  " << sch_num1 << endl; 
                //cout << "----------------" << endl;
            }
            //getchar();
            in_end = clock();
            ///* 
            int count_num = 0;
            float tempa = 0.0f;
            float tempt = 0.0f;
            for (int i = 0; i < front_0; i++)
            {
                tempt += sw_pt->exetime[i];
                tempa += sw_pt->apsc[i];
                count_num += 1;
            //    cout << sw_pt->efficient[i] << "   " << sw_pt->apsc[i] << "  " 
            //        << sw_pt->exetime[i] << endl;
            }
            //fapsc <<  tempa / ((float)count_num) << endl;
            //ftime <<  tempt / ((float)count_num) << endl;
            //ftime << change_cro << " ";

            //*/
            //cout << (double)(in_end - in_begin) / CLOCKS_PER_SEC << endl;
            //cout << front_0 << endl;
            /*
            float temp = 0.0f;
            for (int i = 0; i < SWARMSIZE; i++)
            {
                temp += sw_pt->distance[i];
            }
            cout << temp / (float)SWARMSIZE << endl;
            */            
            
            float temp_a = 0, temp_t = 0;
            for (int i = 0; i < front_0; i++)
            {
                temp_a += sw_pt->apsc[i];
                temp_t += sw_pt->exetime[i];
            }

            int change_a = abs(c_a - (int)(temp_a / (float)front_0 * 1000));
            int change_t = abs(c_t - (int)(temp_t / (float)front_0 * 1000));
            
            c_a = abs((int)(temp_a / (float)front_0 * 1000));
            c_t = abs((int)(temp_t / (float)front_0 * 1000));
            
            int temp_sum = change_a + change_t;
            if (temp_sum == 0) c_sum += 1; else c_sum = 0;
            if (c_sum == 5) break;
            
        }
        //ftime << change_cro << endl;
        cout << "frint set:  " << front_0 << endl;
        for (int i = 0; i < front_0; i++)
        {
            cout << sw_pt->apsc[i] << "  " << sw_pt->exetime[i] << endl;
        }
        cout << endl;

        fapsc.close();
        ftime.close();
        
        end = clock();
        cout << "EXE time: " << (double)(end - begin) / CLOCKS_PER_SEC << endl;
        
        for (int j = 0; j < front_0; j++) 
        {for (int i = 0; i < sch_pt->sch_size; i++)
        {
            //cout << sch_pt->sch_table[j * num_testcase + i] << "  ";
        }
        //cout << "  (" << sw_pt->effnum[j] << ")" << endl;
        }
        //cout << "------------------" << endl;
        int num = 0;
        for (int i = 0; i < front_0; i++)
        {
            for (int j = 0; j < sw_pt->effnum[i]; j++)
            {
                int temp_num = sch_pt->matrix_map[sw_pt->particle[i * num_testcase + j]];
                //cout << temp_num << endl;
            }
        }
        //cout << front_0 << endl;
        //cout << num << endl;

        //cout << "Finish!" << endl;
        //getchar();
        //cout << "size: " << sch_pt->sch_size << endl;
        for (int i = 0; i <sch_pt->sch_size; i++)
        {
            if (sch_pt->schema[i] == -1) continue;
            //cout << sch_pt->schema[i] << endl;
        }
        //cout << endl;
        //getchar();

        Free_memory(sw_pt, sch_pt, CMatrix, TVector, max_cover, learn_val, learn_time);
        
    }
    
}


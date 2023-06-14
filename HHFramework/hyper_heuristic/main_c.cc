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
Swarms temp_swarm;
Swarms copy_swarm;
Schema schema;

#include "read_file.h"
#include "init_ind.h"
#include "init_sch.h"
#include "check_file.h"
#include "check_sequence.h"
#include "cal_fitness_kernel.h"
#include "cal_fitness_kernel_binary.h"
#include "nsga.h"
#include "nsga_binary.h"
#include "taea.h"
#include "taea_binary.h"
#include "paes.h"
#include "paes_binary.h"
#include "spea.h"
#include "spea_binary.h"
#include "pso.h"
#include "pso_binary.h"
#include "greedy_binary.h"
#include "greedy.h"
#include "free_mem.h"
#include "learning.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

#ifdef TCP
string type(int i)
{
    switch(i)
    {
        case -2: return "";
        case -1: return "";
        case 0:  return "_order";
        case 1:  return "_sing";
        case 2:  return "_antsing";
        case 3:  return "_scan";
        case 4:  return "_antscan";
        case 5:  return "_uniform";
    }
}
#endif

#ifdef TCS
string type(int i)
{
    switch(i)
    {
        case -2: return "";
        case -1: return "";
        case 0:  return "_order";
        case 1:  return "_sing";
        case 2:  return "_uniform";
    }
}
#endif

string alg(int i)
{
    switch(i)
    {
        case -2: return "Random";
        case -1: return "Learn";
        case -3: return "Greedy";
        case 0:  return "Nsga";
        case 1:  return "Taea";
        case 2:  return "Spea";
        case 3:  return "Pso";
        case 4:  return "Paes";
    }
}

void Strategy_selection(int *change_alg, int *change_cro, int rand_seed,
        int g, int learn)
{
   if (ALG >= 0)                        // random:-2  learn:-1  greedy:-3  nsga:0  taea:1  spea:2  pso:3  paes:4
   {
       *change_alg = ALG; 
       *change_cro = TYPE;
   }
   else if (ALG < 0)
   {
       int dic_num = 0;

       if (ALG == -2) dic_num = rand_seed;
       else if (ALG == -1)
       {
           if (g < STR) dic_num = g;     //这表示前18代 每个策略先跑一遍 
           else dic_num = learn;
       }

       *change_alg = dic_num / CRO_NUM;
       *change_cro = dic_num % CRO_NUM;
   }

   return;
}

int main()
{
    Swarms* sw_pt = &swarm;         //Create the swarm
    Schema* sch_pt = &schema;

    Swarms* new_sw_pt = &temp_swarm;
    Swarms* sw_cp = &copy_swarm;

    //for (int s = 1; s <= SUITENUM; s++)
    for (int s = 1; s <= 1; s++)
    {//Start of Suite

        stringstream ss;
        ss << s;
        string str_s = ss.str();
        string rt;
#ifdef TCP
        rt = "tcp";
#endif
#ifdef TCS
        rt = "tcs";
#endif
        //string file_path = output_path + prog_name + "/" + rt + "/" + str_s + "/" + 
        //    alg(ALG) + type(TYPE) + "_";
		//定义输出文件路径及名称
        string file_path = output_path + prog_name + "/" + rt + "/" + 
            alg(ALG) + type(TYPE) + "_";

        string apsc_str = file_path + "rand_a_" + prog_name + ".out";
        string tota_str = file_path + "tota_a_" + prog_name + ".out";
        string mult_str = file_path + "mult_a_" + prog_name + ".out";
		string call_time_str = file_path + "call_time_a_" + prog_name + ".out";
        
        const char *path_apsc = apsc_str.c_str();
        const char *path_tota = tota_str.c_str();
        const char *path_mult = mult_str.c_str();
		const char *path_call_time = call_time_str.c_str();

        ofstream fapsc, ftota, fmult, fcall;
        fapsc.open(path_apsc, ios::app);
        ftota.open(path_tota, ios::app);
        fmult.open(path_mult, ios::app);
		fcall.open(path_call_time, ios::app);
        
        cout << endl;
        cout << "PROBLEM    " << rt << endl;
        cout << "SUT        " << prog_name << endl;
        cout << "ALG        " << alg(ALG) << endl;
        cout << "TYPE       " << type(TYPE) << endl << endl; 
        cout << "The number " << s << " test suite" << endl;
        cout << endl;
        
        int num_testcase = 0;
        int code_line = 0;
        
        //Check the basic information of test case
        Check_file(s, &num_testcase, &code_line);
        cout << "test cases number: " << num_testcase << endl
             << "code lines:        " << code_line << endl;
        cout << endl;
        

        CMatrix = (bool*) malloc(sizeof(bool) * num_testcase * code_line);     //覆盖矩阵 
        if (CMatrix == NULL) { cout << "CMatrix malloc error! " << endl;}

        TVector = (float*) malloc(sizeof(float) * num_testcase);             // 存测试用例的执行时间
        if (TVector == NULL) { cout << "TVector malloc error! " << endl;}

        max_cover = (bool*) malloc(sizeof(bool) * code_line);   //该矩阵max_cover[codeLine]为1说明改行代码有测试用例覆盖，为0说明无测试用例覆盖 
        if (max_cover == NULL) { cout << "max_cover malloc error! " << endl;}
        
        learn_val = (float*) malloc(sizeof(float) * STR);
        if (learn_val == NULL) { cout << "learn_val malloc error! " << endl;}

        learn_time = (int*) malloc(sizeof(int) * STR);
        if (learn_time == NULL) { cout << "learn_time malloc error! " << endl;}
		

        //Read the coverage matrix and time array
        Read_file(s, CMatrix, TVector, max_cover, num_testcase, code_line);


        if (ALG == -3)                                            // Line209~~Line262是Greedy算法 
        {
            double* runtime = NULL;
            runtime = (double*) malloc(sizeof(double) * SWARMSIZE * 2);

            Initial_individuals_greedy(sw_pt, num_testcase);

            clock_t g_start, g_end;
            g_start = clock();
#ifdef TCP
            Greedy(sw_pt, CMatrix, TVector, max_cover, num_testcase, code_line,
                    runtime);
#endif

#ifdef TCS
            Greedy_binary(sw_pt, CMatrix, TVector, max_cover, num_testcase, code_line,
                    runtime);
#endif

            g_end = clock();

#ifdef OUTPUT
            double tota = 0.0f, tote = 0.0f, totr = 0.0f, totf = 0.0f;
            
            for (int i = 0; i < SWARMSIZE; i++)
            {
                tota += sw_pt->apsc[i];
                tote += sw_pt->exetime[i];
                totf += (double)(sw_pt->effnum[i]);
                totr += runtime[i];
                fapsc << i << " " << sw_pt->apsc[i] << " " << sw_pt->exetime[i] << " "
                    << sw_pt->effnum[i] << " " << runtime[i] << endl; 
            }
            
            for (int i = 0; i < sw_pt->range; i++)
            {
                fmult << i << " " << sw_pt->apsc[i] << " " << sw_pt->exetime[i] << " "
                    << sw_pt->effnum[i] << " " << runtime[i] << endl; 
            }

            ftota << tota / (double)num_testcase << " " << tote / (double)num_testcase << " " 
                << totf / (double)num_testcase << " " << totr / (double)num_testcase << endl;
#endif 

            fapsc.close();
            fmult.close();
            ftota.close();

            Free_swarm(sw_pt);
            free(runtime);
            Free_memory(CMatrix, TVector, max_cover, learn_val, learn_time);

            return 1;
        }
		
		//Start of Loop
        for (int loops = 0; loops < 30; loops++)              
        {
        
        stringstream loopss;
        loopss << loops;
        string str_loops = loopss.str();
        string sequ_str = file_path + "sequ_" + str_loops + "_" + prog_name + ".out";
        const char *path_sequ = sequ_str.c_str();

        ofstream fsequ;
        fsequ.open(path_sequ, ios::app);
        
        //Initial the individuals
#ifdef TCP
        Initial_individuals(sw_pt, CMatrix, learn_val ,learn_time, num_testcase);   
#endif

#ifdef TCS
        Initial_individuals_binary(sw_pt, CMatrix, learn_val,learn_time, num_testcase);
#endif

        //Initial the schema
        Initial_schema(sch_pt, CMatrix, num_testcase, code_line);

        //Calculate the fitness first time
        int start = 0;
#ifdef TCP
        Cal_fitness_tcp(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
#endif

#ifdef TCS
        Cal_fitness_binary(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start);
#endif

        if (ALG == -1 || ALG == -2 || ALG == 3 || ALG == 4)      //Initial for PSO:4 SPEA:3  LEARN:-2 RANDOM:-1   
        {
            Initial_individuals_pure(new_sw_pt, sw_pt, num_testcase);    
        }
        
        if (ALG == -1)        //Initial for Learn rank individuals
        {
            Initial_individuals_pure(sw_cp, sw_pt, num_testcase);    
        }
     
        int front_0 = 0;             //是pareto前沿个体数 
        int change_cro = 0;
        int change_alg = 0;
        int learn = 0;             //learn是Learning函数返回的所选算法序号

        //For selection random number
        int rand_seed[GENERATION];
        
        if (ALG == -2)         //Random随机 
        {
            srand((unsigned int)time(0) * (loops + 1));
            for (int i = 0; i < GENERATION; i++) rand_seed[i] = rand() % STR;  // 每一代随机从18种策略选一个，赋值给rand_seed[i] 
        }

        //Stop condition judge
        int c_a = 0, c_t = 0;
        int c_sum = 0;
        int c_judge = 0;

        clock_t begin, end;

        begin = clock();

        int gener = 0;
        for(int g = 0; g < GENERATION; g++)
        {//Start of Gener
            cout << "Num " << g << "  gene. -- test suite: " << s << "  loops: " << loops << endl;

            clock_t in_begin, in_end;
            
            in_begin = clock();
            int start = SWARMSIZE;

            // For select algorithms and strategies
            Strategy_selection(&change_alg, &change_cro, rand_seed[g], g, learn);   //g是代数，learn是Learning函数返回的所选算法序号 
            
            if (change_alg == 0)
            {
#ifdef TCP
                front_0 = Nsga_II(sw_pt, sch_pt, num_testcase, code_line, CMatrix, TVector, max_cover, 
                        start, g, &change_cro, learn_val, learn_time, rand_seed[g]);
#endif

#ifdef TCS
                front_0 = Nsga_II_binary(sw_pt, sch_pt, num_testcase, code_line, CMatrix, TVector, max_cover, 
                        start, g, &change_cro, learn_val, learn_time, rand_seed[g]);
#endif
            }
            else if (change_alg == 1)
            {
#ifdef TCP
                front_0 = Taea(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, 
                        front_0, &change_cro);
#endif

#ifdef TCS
                front_0 = Taea_binary(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, 
                        front_0, &change_cro);
#endif
            }
            else if (change_alg == 2)
            {
#ifdef TCP
                front_0 = Spea(sw_pt, new_sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, 
                        front_0, &change_cro);
#endif

#ifdef TCS
                front_0 = Spea_binary(sw_pt, new_sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, 
                        front_0, &change_cro);
#endif
            }
            else if (change_alg == 4)
            {
#ifdef TCP
                front_0 = Paes(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, 
                        front_0, &change_cro);
#endif

#ifdef TCS
                front_0 = Paes_binary(sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, start, 
                        front_0, &change_cro);
#endif
            }
            else if (change_alg == 3)
            {
#ifdef TCP
                front_0 = Pso(sw_pt, new_sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, 
                        start, front_0);
#endif

#ifdef TCS
                front_0 = Pso_binary(sw_pt, new_sw_pt, num_testcase, code_line, CMatrix, TVector, max_cover, 
                        start, front_0);
#endif
            }
            else
            {
                cout << "Error: no algorithm " << ALG << endl;
                getchar();
            }

            in_end = clock();

            double gen_time = (in_end - in_begin) / CLOCKS_PER_SEC; //运行一代所花费的时间

            if (ALG == -1) 
            {
                learn = Learning(sw_pt, sw_cp, change_alg, change_cro, learn_val, learn_time, gen_time,
                        num_testcase,g);
            }

            // Output -----------------------------------------------------
#ifdef OUTPUT
            float tempa = 0.0f, tempe = 0.0f, tempf = 0.0f;
            for (int i = 0; i < front_0; i++)//front_0是当代种群的pareto前沿个体数
            {
                tempa += sw_pt->apsc[i];
                tempe += sw_pt->exetime[i];
                tempf += sw_pt->effnum[i];
            }
            fapsc << loops << " " << tempa / (float)front_0 << " " << tempe / (float)front_0 << " "
                  << tempf / (float)front_0 << " " 
                  << (double)(in_end - in_begin) / CLOCKS_PER_SEC //<< endl; 
                  << " " << change_alg * CRO_NUM + change_cro << endl;
            
            fsequ << loops << " " << tempa / (float)front_0 << " " << tempe / (float)front_0 << " "
                  << tempf / (float)front_0 << " " 
                  << (double)(in_end - in_begin) / CLOCKS_PER_SEC //<< endl; 
                  << " " << change_alg * CRO_NUM + change_cro <<endl;//sequ文件记录算法每一次运行时每一代所得到的pareto前沿中所有个体的平均属性值
            
            if (g % 50 ==0)          //每50代输出一次前沿个体信息 
            {
                for (int i = 0; i < front_0; i++)
                {
                    fmult << g << " " << loops << " " << sw_pt->apsc[i] << " " << sw_pt->exetime[i]
                          << " " << sw_pt->effnum[i] << endl;
                }
                fmult << endl;
            }
#endif
            // End Output

            //Stop condition judge ----------------------------------------
            float temp_a = 0, temp_t = 0;
            for (int i = 0; i < front_0; i++)
            {
                temp_a += sw_pt->apsc[i];
                temp_t += sw_pt->exetime[i];
                //cout << sw_pt->apsc[i] << "   " << sw_pt->exetime[i] << endl;
            }

            int change_a = abs(c_a - (int)(temp_a / (float)front_0 * 1000));//上一代种群pareto前沿上所有个体apsc值的平均值乘以1000减去这一代~的绝对值
            int change_t = abs(c_t - (int)(temp_t / (float)front_0 * 1000));
            
            c_a = abs((int)(temp_a / (float)front_0 * 1000));//当代种群pareto前沿上所有个体apsc值的平均值乘以1000
            c_t = abs((int)(temp_t / (float)front_0 * 1000));//当代种群pareto前沿上所有个体exetime值的平均值乘以1000
            
            gener = g;
            int temp_sum = change_a + change_t;
            if (temp_sum == 0) c_sum += 1; else c_sum = 0;
            if (c_sum == 10) 
            {
                if (gener < 50) continue;
                else
                    break;
            }

            if (sw_pt->apsc[0] == 1) break;//当代种群pareto前沿上排第一的个体

            cout << "******** " << c_sum << " **********" << endl;
        
        }//End of Gener
        end = clock();

        // Output
#ifdef OUTPUT 
        float totala = 0.0f, totale = 0.0f, totalf = 0.0f;
        for (int i = 0; i < front_0; i++)
        {
            totala += sw_pt->apsc[i];
            totale += sw_pt->exetime[i]; 
            totalf += sw_pt->effnum[i];

            fmult << "-1 " << sw_pt->apsc[i] << " " << sw_pt->exetime[i]
                  << " " << sw_pt->effnum[i] << endl;//mult文件除了记录每次运行每隔50代的前沿个体信息外，还记录了每次运行收敛时的那代种群的前沿个体信息
        }
        fmult << endl;
        //tota文件记录每次运行收敛时的那代种群的平均属性值
        ftota << totala / (float)front_0 << " " << totale / (float)front_0 << " " 
              << totalf / (float)front_0 << " " << gener << " " << front_0 << " "
              << " " << (double)(end - begin) / CLOCKS_PER_SEC << endl;
        for(int i = 0; i < STR; i++){
			fcall << learn_time[i] << " ";
		}
		fcall <<endl;
		
        fapsc << endl << "&" << endl;
#endif        
        // End Output
		
        cout << sw_pt->range << endl;
        //输出每次运行收敛时的那代种群的前沿个体信息
        for (int i = 0; i < sw_pt->range; i++)
        {
            cout << sw_pt->apsc[i] << "   " << sw_pt->exetime[i] << endl;
            for (int j = 0; j < sw_pt->effnum[i]; j++)
            //for (int j = 0; j < num_testcase; j++)
            {
                //cout << sw_pt->particle[i * num_testcase + j] << " ";
                if (sw_pt->particle[i * num_testcase +j] == 1)
                {
                    //cout << j << " ";
                }
            }
            //cout << endl;
        }
	
		//输出每次运行收敛时各个底层算法的调用次数
		for (int i = 0; i < STR; i++)
        {
             cout << "(" << i << ") ";
             cout << "  " << learn_time[i];
             cout << endl;
        }
		  
        fsequ.close();
        
        } //End of Loop

        fapsc.close();
        fmult.close();
        ftota.close();
		fcall.close();

        if (ALG == -1 || ALG == -2 || ALG == 3 || ALG == 4)
        {//Free for PSO and other algorithms
            Free_swarm(new_sw_pt);    
        }

        if (ALG == -1)
        {//Free learning rank individual
            Free_swarm(sw_cp);    
        }

        Free_memory(CMatrix, TVector, max_cover, learn_val, learn_time);
        Free_swarm(sw_pt);
        Free_schema(sch_pt);
        
    }//End of Suite
    
    return 1;
}


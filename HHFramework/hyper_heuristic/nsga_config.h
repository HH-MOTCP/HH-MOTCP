/*
 * =====================================================================================
 *
 *       Filename:  pso_config.h
 *
 *    Description:  For pso base config
 *
 *        Version:  1.0
 *        Created:  06/07/14 16:00:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _NSGA_CONFIG_H_
#define _NSGA_COFNIG_H_

//For systhem lib
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <iostream>

//For cuda lib
//#include <cuda.h>
//#include <cutil.h>
//#include <cutil.h>
//#include <cudart.h>

using namespace std;


//Switch
#define OUTPUT          //output mark  
#define TCP             //TCP prioritization: permutation //TCS selection: binary

#define ALG            -1       // random:-2  learn:-1  greedy:-3
                               // nsga:0  taea:1  spea:2 
                               // pso:3  paes:4

#define TYPE           -1       // permutation: "other: -1" "order: 0" "sing: 1"
                               //              "antsing: 2" "scan: 3" "antscan: 4" "uniform: 5"
                        
                               // binary: "other: -1" "order: 0" "sing: 1"
                               //              "uniform: 2"
#define CRO_NUM        6 
#define ALG_NUM        3     
#define STR            (CRO_NUM * ALG_NUM)

//Parameters
#define SWARMSIZE      200
#define GENERATION     10000
#define SUITENUM       1
#define MIN            1e-6
#define MAX            1000000
#define MUTRATE        0.001 //TCS: 0.05
#define CRORATE        0.1   //TCS: 0.2
//#define THREADNUM      256//512
//#define ARCHSIZE       300//5000
//#define MAXFUN         2
//#define MAXLINE        (CODELINE + THREADNUM)/THREADNUM * THREADNUM
//#define MAXGENE        5000

// Program struct

struct Swarms      //将种群定义为结构体 
{
    int    *particle;  //个体 
    float  *apsc;     //apsc 
    float  *exetime;  //执行时间 
    int    *effnum;   //有效测试用例数 
    float  *disapsc;    //计算个体之间apsc的距离
    float  avgapsc;    //平均apsc 
    float  *disexet;   //计算个体之间eet的距离（
    float  avgexetime;  //平均执行时间 
    float  *distance;     //距离 
    float  avgdistance;   //平均距离 
    bool   *efficient;    //标注个体中是否被选，被选为1,否则为0 
    bool   *newgen;   //表示新的一代个体，全部置1，旧代个体置0 
    int    *batch;    //表示hierarchy的层数，pareto前沿该值为0，后面的层就是1,2,3... 
    int    range;     //记录pareto前沿的个体数  精英选择策略也是返回前沿个体数 
};

struct Schema
{
    int    *matrix_map;
    int    map_size;
    int    *schema;
    int    sch_size;
    float  *sch_value;
    int    *sch_table;
    bool   *sch_chose;
};

extern void Init_sequence(int *temp_sequence, int gen_num);      
extern void Init_sequence_binary(int *temp_sequence, int gen_num, int loop);
extern void Shuffle(int *temp_sequence, int gen_num, unsigned int loop_count);   //初始化 
extern int  Check(bool* max_cover, bool* mark_line, int code_line); 

extern string prog_name;
extern string input_path;
extern string cover_file;
extern string eet_file;
extern string output_path;

#endif //end _PSO_CONFIG_H_


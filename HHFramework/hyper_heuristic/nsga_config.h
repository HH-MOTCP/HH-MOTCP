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

struct Swarms      //����Ⱥ����Ϊ�ṹ�� 
{
    int    *particle;  //���� 
    float  *apsc;     //apsc 
    float  *exetime;  //ִ��ʱ�� 
    int    *effnum;   //��Ч���������� 
    float  *disapsc;    //�������֮��apsc�ľ���
    float  avgapsc;    //ƽ��apsc 
    float  *disexet;   //�������֮��eet�ľ��루
    float  avgexetime;  //ƽ��ִ��ʱ�� 
    float  *distance;     //���� 
    float  avgdistance;   //ƽ������ 
    bool   *efficient;    //��ע�������Ƿ�ѡ����ѡΪ1,����Ϊ0 
    bool   *newgen;   //��ʾ�µ�һ�����壬ȫ����1���ɴ�������0 
    int    *batch;    //��ʾhierarchy�Ĳ�����paretoǰ�ظ�ֵΪ0������Ĳ����1,2,3... 
    int    range;     //��¼paretoǰ�صĸ�����  ��Ӣѡ�����Ҳ�Ƿ���ǰ�ظ����� 
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
extern void Shuffle(int *temp_sequence, int gen_num, unsigned int loop_count);   //��ʼ�� 
extern int  Check(bool* max_cover, bool* mark_line, int code_line); 

extern string prog_name;
extern string input_path;
extern string cover_file;
extern string eet_file;
extern string output_path;

#endif //end _PSO_CONFIG_H_


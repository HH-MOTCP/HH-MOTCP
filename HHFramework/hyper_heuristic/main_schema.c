#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sstream>
#include<iostream>

#define swarmsize 1000 //100 500 1000
#define archsize 5109
#define maxfun 2
#define MAXgenes 5109
#define goalnum 60000
#define GENERATION 100 //100 300 200//500
#define E 1e-8
#define SUITENUM 10//100

// all_clear.cov has 59411 statements and 10029 test caes.
//         7.cov has 59411 statements and 5109  test case.

int CMatrix[MAXgenes][goalnum];
float TVector[MAXgenes];
int flag[archsize];
FILE* arch_fit;
FILE* gen_time;
FILE* FRONT_ALY;

typedef struct
{
	int particle[swarmsize][MAXgenes];//粒子的位置 
	int speed[swarmsize][MAXgenes];//粒子的速度
	int efficient[swarmsize];// 每个粒子的有效长度 
	 
	float fitness[maxfun][swarmsize];//每个粒子都有maxfun个fitness值 
	
	int pbest[swarmsize][MAXgenes];// 记录粒子本身所达到的最优位置 
	float pbestfit[maxfun][swarmsize];//粒子本身最优位置的fitness 
	
	int gbest[MAXgenes];// 全局最优位置
	float gbestfit[maxfun];// 全局最优位置的fitness 
	
	int EA[archsize][MAXgenes];// 外部档案，存储非支配解集
	float EA_fit[maxfun][archsize];// EA中每个解的fitness值 
	int EA_len;// 外部档案的大小
	 
	
}swarm;

swarm swar,
		*sw;


#include"init.h"
#include"fun-con.h"
#include"Arch_gbest.h"
#include"vol_pos.h"
#include"updatePbest.h"

int main()
{
	int i,j,k,zs;
	//int zs;
	int realgenes=0;
	char ch;
	clock_t t_start,t_end;
	int file_num;
	
	//for(file_num=1;file_num<=SUITENUM;file_num++)
    for (int count = 0; count < 10; count++)
    {
	for(file_num=1;file_num<=30;file_num++)
	{
		// just for tips
		printf("***********FILE_NUM = %d***********\n",file_num);
	
        stringstream cs;
        cs << count;
        string count_num = cs.str();

		//char stz[50];
		//char stz2[50];
		stringstream ss;
        ss << file_num;
        string serial_num = ss.str();

   	 	//itoa(file_num,stz,10);
   	 	//itoa(file_num,stz2,10);
		//sprintf(stz,"%d",file_num);
		//sprintf(stz2,"%d",file_num);

   	 	//char sz1[] = "_v8_subSuite.out";
   		//char sz2[] = "_v8_time.out";
        
        string in_path = "../v8_5_subsuite/";
        string out_path = "../v8_pso_serial_out/" + count_num + "/";
        //string ssz1 = in_path + serial_num + "_v8_5_subSuite.out";
        string ssz2 = in_path + serial_num + "_v8_5_time.out";
        
        string ssz1 = "../7.cov";
        
        const char *sz1 = ssz1.c_str();
	    const char *sz2 = ssz2.c_str();
		// ************read the coverage matrix*****************	
		t_start = clock();// 开始时间 
	
		FILE* file_CMatrix;// pointer of Matrix file
 		//int CMatrix[maxchrom][goalnum];// store the Matrix
 	
 		//file_CMatrix = fopen(strcat(stz,sz1),"r");// open the maxtrix file
 		if (0 == (file_CMatrix = fopen(sz1, "r")))
        {
            cout<<"File CMatrix Open Error!"<<endl;
        }// open the maxtrix file
 		// put number to the CMatrix
 		int j= 0;
 		realgenes = 0;
 		for(i=0;i<MAXgenes;i++)
 		{
			while((ch=fgetc(file_CMatrix))!=EOF)
			{
				if(ch=='\n')
				{
					j = 0 ;
					realgenes++;
					break;
				}
				CMatrix[i][j] = (int)(ch-'0');// int:0,1
				j++;
			}
		}
		fclose(file_CMatrix);// close the file
			
		t_end = clock();// 结束时间 
	
		printf("Read Cmatrix time = %f\n",(double)(t_end-t_start)/CLOCKS_PER_SEC);
		printf("***********realgenes = %d***********\n",realgenes);
		// ************read the coverage matrix IS OVER***************
	
        
		// ************combina the test with same coverage***************


        int dif_tc[MAXgenes];
        for (i = 0; i < MAXgenes; i++) dif_tc[i] = 1;
        
        int dif_count[MAXgenes];
        for (i = 0; i < MAXgenes; i++) dif_count[i] = 0;

        int kind_count = 0;
        for (i = 0; i < MAXgenes; i++)
        {
            //cout << i << " ";

            if (dif_tc[i] == 0) continue;

            int same_count = 0;
            for (j = i + 1; j < MAXgenes; j++)
            {
                if (dif_tc[j] == 0) continue;

                int mark_dif = 0;
                for (k = 0; k < goalnum; k++)
                {
                    if (CMatrix[i][k] == CMatrix[j][k]) continue;
                    else
                    {
                        mark_dif = 1;
                        break;
                    }
                }

                if (mark_dif == 0)
                {
                    dif_tc[j] = 0;
                    same_count++;
                }
            }
            dif_count[i] = same_count;
        }

        int count = 0;
        int total = 0;
        for (i = 0; i < MAXgenes; i++)
        {
            if (dif_tc[i] == 1) 
            {
                //count++;
                //total += dif_count[i];
                cout << "(" << i << ", " << dif_count[i] << ") ";
            }
        }

        cout << endl;
        cout << "the diferent TC: " << count << endl;
        getchar();

		// clear
		i = 0;
		j = 0;
	
	
		//************read from file:the cost of each test case***********
	
		t_start = clock();// 开始时间 
	    /*  
		FILE* file_TVector;//pointer of the time file
		
		file_TVector = fopen(sz2, "r");// open the time file
	
		float s;
	
		while(!feof(file_TVector))
		{
			fscanf(file_TVector,"%f",&TVector[i]);// assign the time number		
			i++;
		}
		fclose(file_TVector);// close the file
        */	
		t_end = clock();// 结束时间
	
		printf("Read Tvector time = %f\n",(double)(t_end-t_start)/CLOCKS_PER_SEC); 
		//******read from file:the cost of each test case IS OVER********
	
	
		//char stz3[50];
		//itoa(file_num,stz3,10);
		///sprintf(stz3,"%d",file_num);
   	 	//char sz3[] = "_100p_front_file.xls";    
        
        string sz6 = out_path + serial_num +"_1000P_100G_front_file.xls";
        const char *stz6 = sz6.c_str();
    	FRONT_ALY = fopen(stz6,"w+");
    
		fprintf(FRONT_ALY,"exp=%d\n",file_num);
		t_start = clock();// 开始时间
	 
		sw = &(swar);
		Initializing(sw,realgenes);
	
		t_end = clock();// 结束时间
	
		printf("Initizing time = %f\n",(double)(t_end-t_start)/CLOCKS_PER_SEC);
	
		// initize IS OVER
	
		//evaluate the particles
	
		t_start = clock();// 开始时间
	
		func(sw,realgenes);

		t_end = clock();// 结束时间
	
		printf("function evaluating time= %f\n",(double)(t_end-t_start)/CLOCKS_PER_SEC);
	
		// evaluate IS OVER
		
		// initize the pbestfitness
		for(i=0;i<swarmsize;i++)
		{
			for(j=0;j<maxfun;j++)
			{
				sw->pbestfit[j][i] = sw->fitness[j][i];
			}
		}
	
		updateArch(sw,realgenes);
    	gbest_select(sw,realgenes);
    
    
    	//char stz4[50];
    	//char stz5[50];
    	//itoa(file_num,stz4,10);
    	//itoa(file_num,stz5,10);
	    //sprintf(stz4,"%d",file_num);
	    //sprintf(stz5,"%d",file_num);
    	//char s1[] = "_100P_300G_front.xls";
   		//char s2[] = "_100P_300G_time.xls";

        string s4 = out_path + serial_num + "_1000P_100G_front.xls";
        string s5 = out_path + serial_num + "_1000P_100G_time.xls";
        const char *stz4 = s4.c_str();
        const char *stz5 = s5.c_str();
		arch_fit = fopen(stz4, "w+");//store the fitness of archive
		gen_time = fopen(stz5, "w+");// store the cost time of every generation
	
	
		for(i=1;i<GENERATION;i++)
		{// iteration for getting the best archive
			printf("the generation = %d\n",i); 
		
			fprintf(gen_time,"generation==%d\n",i);
			// **********refresh the speed and position of each particle
			t_start = clock();// 开始时间
			updateVol_Pos(sw,realgenes);//Oridional Update
			//updateVol_Pos_Quick(sw,realgenes);//Combin Three Formule into One --BY
			t_end = clock();// 结束时间
			fprintf(gen_time,"%.8f ",(double)(t_end-t_start)/CLOCKS_PER_SEC);
	
			// ******evaluation, get the pareto front***********
			t_start = clock();// 开始时间
		
			func(sw,realgenes);
		
			t_end = clock();// 结束时间
			fprintf(gen_time,"%.8f ",(double)(t_end-t_start)/CLOCKS_PER_SEC);
		
			//*******************************************
			t_start = clock();// 开始时间
		
			updatepbest(sw,realgenes);
		
			t_end = clock();// 结束时间
			fprintf(gen_time,"%.8f ",(double)(t_end-t_start)/CLOCKS_PER_SEC);
		
			// ****************refresh the EA and gbest
			t_start = clock();// 开始时间
			updateArch(sw,realgenes);
		
			t_end = clock();// 结束时间
			fprintf(gen_time,"%.8f ",(double)(t_end-t_start)/CLOCKS_PER_SEC);
		
			fprintf(arch_fit,"generation==%d\n",i+1);
		
			for(j=0;j<sw->EA_len;j++)
    		{// 输出EA中的fitness值到文件中 
            	if(flag[j]!=0)
    	    	fprintf(arch_fit,"%.8f %.8f\n",sw->EA_fit[1][j],sw->EA_fit[0][j]);
        	}
        
    	
    		//*******************select the global best******
    		t_start = clock();// 开始时间
			gbest_select(sw,realgenes);
			t_end = clock();// 结束时间
			fprintf(gen_time,"%.8f\n",(double)(t_end-t_start)/CLOCKS_PER_SEC);
		
		
			if(i%50==0)
			{
             	fprintf(FRONT_ALY,"GENERATION=%d\n",i);
           	 	for(j=0;j<sw->EA_len;j++)
        		{// 输出EA中的fitness值到文件中 
        	    
                	if(flag[j]!=0)
        	    	fprintf(FRONT_ALY,"%.8f %.8f\n",sw->EA_fit[1][j],sw->EA_fit[0][j]);
            	}     
        	}    
		}
		printf("arch_len = %d\n",sw->EA_len);
		fclose(FRONT_ALY);
		fclose(arch_fit); 
		fclose(gen_time);

	}
    }    
	//system("pause");
	return 0;
}

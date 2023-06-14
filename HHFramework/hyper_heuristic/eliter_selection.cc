/*
 * =====================================================================================
 *
 *       Filename:  eliter_selection.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/27/14 17:25:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include "nsga_config.h"
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "domain_judge.h"
#include "copy_swarm.h"
#include "free_mem.h"

using namespace std;

void Copy_swarm_new(Swarms *sw_pt, Swarms *sw_new, int a, int b, int num_testcase)
{
    
    //sw_new->apsc[a] =      sw_pt->apsc[b];
    sw_new->exetime[a] =   sw_pt->exetime[b];
    sw_new->disapsc[a] =   sw_pt->disapsc[b];
    sw_new->disexet[a] =   sw_pt->disexet[b];    
    sw_new->newgen[a] =   sw_pt->newgen[b];   
    return;
}

void Switch_particle(Swarms *sw_pt, int a, int b, int num_testcase)
{
    //if (a == b) return;
     
    for (int i = 0; i < num_testcase; i++)
    {
        int pos1 = a * num_testcase + i;
        int pos2 = b * num_testcase + i;
        swap(sw_pt->particle[pos1], sw_pt->particle[pos2]);
    } 
    
    swap(sw_pt->apsc[a],      sw_pt->apsc[b]);
    swap(sw_pt->exetime[a],   sw_pt->exetime[b]);
    swap(sw_pt->distance[a],  sw_pt->distance[b]);
    swap(sw_pt->disapsc[a],   sw_pt->disapsc[b]);
    swap(sw_pt->disexet[a],   sw_pt->disexet[b]);
    swap(sw_pt->effnum[a],    sw_pt->effnum[b]);
    swap(sw_pt->efficient[a], sw_pt->efficient[b]);
    swap(sw_pt->newgen[a],    sw_pt->newgen[b]);
    swap(sw_pt->batch[a],    sw_pt->batch[b]);

    return;
}
/*
void Switch_particle_samebatch(Swarms *sw_pt, int a, int b, int num_testcase)
{
    //if (a == b) return;
     
    for (int i = 0; i < num_testcase; i++)
    {
        int pos1 = a * num_testcase + i;
        int pos2 = b * num_testcase + i;
        swap(sw_pt->particle[pos1], sw_pt->particle[pos2]);
    } 
    
    swap(sw_pt->apsc[a],      sw_pt->apsc[b]);
    swap(sw_pt->exetime[a],   sw_pt->exetime[b]);
    swap(sw_pt->distance[a],  sw_pt->distance[b]);
    swap(sw_pt->disapsc[a],   sw_pt->disapsc[b]);
    swap(sw_pt->disexet[a],   sw_pt->disexet[b]);
    swap(sw_pt->effnum[a],    sw_pt->effnum[b]);
    return;
}
*/

int compare(float a, float b)
{
    if(a < b){
		return -1;
	}else if(a > b){
		return 1;
	}else{
		return 0;
	}
    
}

void Maopao(Swarms *sw_pt, int *record, int count)        
{

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = count -2; j >= i; j--)
        {
            int dis1 = (int)(sw_pt->distance[record[j]] * 1000000);
            int dis2 = (int)(sw_pt->distance[record[j + 1]] * 1000000);
            
            if (dis1 < dis2)
                swap(record[j], record[j + 1]);           
        }
    }
}

void sort(Swarms* sw_pt, int* record, int lo, int hi){
	int lt = lo;
    int i = lo + 1;
    int gt = hi;
    float diffNum = sw_pt->exetime[record[lo]];
 
    if ( hi <= lo )
    {
        return; 
    }
 
    while ( i <= gt )
    {
        int cmp = compare(sw_pt->exetime[record[i]], diffNum);
		
        if (cmp < 0)
        {
            swap(record[i], record[lt]);
			lt++;
			i++;
        } 
        else if ( cmp > 0 )
        {
            swap(record[i], record[gt]);
			gt--;
        }
        else
        {
            i++; 
        }
    }
 
    sort(sw_pt, record, lo, lt - 1);
    sort(sw_pt, record, gt + 1, hi);
}

void Init_temp_swarm(Swarms* sw_pt, int num_testcase)
{
    
    sw_pt->particle   = (int*)malloc(sizeof(int) * SWARMSIZE * num_testcase * 2);
    sw_pt->apsc       = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->exetime    = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->effnum     = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    sw_pt->distance   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disapsc   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->disexet   = (float*)malloc(sizeof(float) * SWARMSIZE * 2);
    sw_pt->efficient  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->newgen  = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2); 
    sw_pt->batch  = (int*)malloc(sizeof(int) * SWARMSIZE * 2); 
    
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        sw_pt->apsc[i]      = (float)MIN; 
        sw_pt->exetime[i]   = (float)MAX;
        sw_pt->distance[i]  = (float)MAX;
        sw_pt->disapsc[i]   = (float)MAX;
        sw_pt->disexet[i]   = (float)MAX;
        sw_pt->effnum[i]    = 0;
        sw_pt->efficient[i] = 0;
        sw_pt->newgen[i] = 0;
        sw_pt->batch[i] = 0;
    }

    sw_pt->avgapsc = 0.0f;
    sw_pt->avgexetime = 0.0f;
    sw_pt->avgdistance = 0.0f;
}

int Eliter_selection(Swarms *sw_pt, int num_testcase)
{
    int front_num = 0;          //pareto前沿的个体数 
    int set_count = 0;          //记录每一层的个体数之和 

    Swarms sw_new;
    Init_temp_swarm(&(sw_new), num_testcase);

    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->efficient[i] = 0;
    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->batch[i] = 0;
    
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[i] = 0;
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[SWARMSIZE + i] = 1;
    
    bool* mark_dom = NULL;
    mark_dom = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2);
    int* mark_record = NULL;
    mark_record = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
   
    int batch_num = 0;      //表示是第batch_num层前沿
	// 这个while循环是对种群中所有个体分层
    while (set_count < SWARMSIZE && batch_num < SWARMSIZE)
    {
        batch_num++;
        
        int mark_record[SWARMSIZE * 2];
        
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            mark_dom[i] = 0;
            mark_record[i] = -1;
        }

        //for (int i = set_count; i < SWARMSIZE * 2; i++)
        //这个for循环实质是找出本层hierarchy（实质是从未分层的个体中找出前沿） 
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            if (sw_pt->efficient[i] == 1)   //表示已经被选的在上一层中的个体，就将这些个体排除在本层之外 
            {
                mark_dom[i] = 1;
            }

            if (mark_dom[i] == 1) continue;

            for (int j = i + 1; j < SWARMSIZE * 2; j++)
            {
                if (mark_dom[j] == 1 || sw_pt->efficient[j] == 1) continue;

                int judge = Domain_compare(sw_pt, i, j);   //该函数比较个体i和j的支配关系 
                
                if (judge == -1) continue;   //说明个体i、j互不支配 
                else if (judge == -2)         //说明个体i、j的各个目标值相等
                {
                    sw_pt->efficient[j] = 1;   //把j个体标记为已选，也就是排除在本层之外，只留下i个体进行后续比较即可 
                }
                else if (judge == i)       //说明个体i支配个体j 
                {
                    mark_dom[j] = 1;
                }
                else
                {
                    mark_dom[i] = 1;     //说明个体j支配个体i 
                    break;
                }
            }
        }
       
        int f_count = 0;   //记录每层前沿个体数
		 
        //这个for循环是把上一个for循环中mark_dom[i]==0的个体作为本层（前沿） 
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            if (0 == mark_dom[i])           //说明没有个体支配个体i 
            {
                mark_record[f_count] = i;   //记录本层的个体所对应的序号    
                sw_pt->efficient[i] = 1;
                sw_pt->batch[i] = batch_num;
                f_count++;
            }
        }
		
        if (set_count == 0 || set_count + f_count >= SWARMSIZE)
        {
            Maopao(sw_pt, &(mark_record[0]), f_count);      //将前沿个体的拥挤距离从大到小排序，越大越好 
        }
       
     
        
        for (int i = 0; i < f_count; i++)
        {
            //Switch_particle(sw_pt, i + set_count, mark_record[i], num_testcase);
            Copy_swarm(sw_pt, &(sw_new), i + set_count, mark_record[i], num_testcase);  //mark_record[i]是前沿中的第i个个体对应的序号 
        }

        if (0 == set_count) 
        {
            front_num = f_count;
        }

        set_count += f_count;
    }

    for (int i = 0; i < set_count; i++)
    {
        Copy_swarm(&(sw_new), sw_pt, i, i, num_testcase);
    }

    Free_swarm(&(sw_new));

    free(mark_dom);
    free(mark_record);
    
    sw_pt->range = front_num;

    return front_num;    //返回pareto前沿个体数 
}

void Eliter_selection_new(Swarms *sw_cp, Swarms *sw_pt, int num_testcase)
{
    Swarms sw_temp;
    Init_temp_swarm(&(sw_temp), num_testcase);
	Swarms* sw_new = &sw_temp;

    int* record = NULL;
    record = (int*)malloc(sizeof(int) * SWARMSIZE * 2);	
    
	
	for(int i = 0; i < SWARMSIZE; i++){
		sw_new->disapsc[i] = sw_cp->disapsc[i];
		sw_new->disexet[i] = sw_cp->disexet[i];
		record[i] = i;
        sw_new->exetime[i] = 0;       				
	}
	
	for(int i = 0; i < SWARMSIZE; i++){
		sw_new->disapsc[i + SWARMSIZE] = sw_pt->disapsc[i];
		sw_new->disexet[i + SWARMSIZE] = sw_pt->disexet[i];
		record[i + SWARMSIZE] = i + SWARMSIZE;
        sw_new->exetime[i + SWARMSIZE] = 0;       				
	}
	
	for (int i = 0; i < SWARMSIZE; i++) sw_new->newgen[i] = 0;
    for (int i = SWARMSIZE; i < SWARMSIZE * 2; i++) sw_new->newgen[i] = 1;       
    	
    for (int i = 0; i < SWARMSIZE * 2 - 1; i++)
    {                    		   		                  
        for (int j = i + 1; j < SWARMSIZE * 2; j++)
        {						
            int judge = Domain_compare_new(sw_new, i, j);   //该函数比较个体i和j的支配关系                
            if (judge == -1 || judge == -2) continue;   //说明个体i、j互不支配 
            else if (judge == i)       //说明个体i支配个体j 
            {               
				sw_new->exetime[j]++;							
            }
            else
            {                  //说明个体j支配个体i 
				sw_new->exetime[i]++;							
            }            
        }
    }
    
	sort(sw_new, record, 0,  SWARMSIZE * 2 - 1);
	
	for (int i = 0; i < SWARMSIZE; i++)
    {
        Copy_swarm_new(sw_new, sw_cp, i, record[i], num_testcase);
    }
	
	Free_swarm(sw_new);	
	free(record);
    return ;		
}


void Update_distance_taea(Swarms* sw_pt, Swarms* sw_new, int *dive_record, int num_testcase, //验证可得，更新后的距离和pareto支配关系是不一致的
            int f_count, int d_count)
{
    for (int d = 0; d < d_count; d++)
    {
        sw_pt->distance[dive_record[d]] = 0;
        for (int f = 0; f < f_count; f++)
        {
            int tempa = abs((int)(sw_new->apsc[f] * 1000) - 
                    (int)(sw_pt->apsc[dive_record[d]] * 1000)); 
            int tempt = abs((int)(sw_pt->exetime[dive_record[d]] * 1000) - 
                    (int)(sw_new->exetime[f] * 1000));

            sw_pt->distance[dive_record[d]] += (float)tempa / sw_pt->avgapsc + 
                (float)tempt / sw_pt->avgexetime;
        }
    }
}

int Eliter_selection_taea(Swarms *sw_pt, int num_testcase)
{
    int front_num = 0;
    int set_count = 0;

    Swarms sw_new;
    Init_temp_swarm(&(sw_new), num_testcase);
		
	
    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->efficient[i] = 0;
    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->batch[i] = 0;
    
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[i] = 0;
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[SWARMSIZE + i] = 1;
   
    bool* mark_dom = NULL;
    mark_dom = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2);
    int* mark_record = NULL;
    mark_record = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
    int* dive_record = NULL;
    dive_record = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
        
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        mark_dom[i] = 0;
        mark_record[i] = -1;
    }

    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        if (sw_pt->efficient[i] == 1) 
        {
            mark_dom[i] = 1;
        }

        if (mark_dom[i] == 1) continue;

        for (int j = i + 1; j < SWARMSIZE * 2; j++)
        {
            if (mark_dom[j] == 1 || sw_pt->efficient[j] == 1) continue;

            int judge = Domain_compare(sw_pt, i, j);
                
            if (judge == -1) continue;
            else if (judge == -2)
            {
                sw_pt->efficient[j] = 1;
            }
            else if (judge == i)
            {
                mark_dom[j] = 1;
            }
            else
            {
                mark_dom[i] = 1;
                break;
            }
        }
    }
       
    int f_count = 0, d_count = 0;
    for (int i = 0; i < SWARMSIZE * 2; i++)
    {
        if (0 == mark_dom[i])
        {
            mark_record[f_count] = i;
            sw_pt->efficient[i] = 1;
            sw_pt->batch[i] = 1; // For TAEA
            f_count++;
        }
        else if (0 != mark_dom[i] && i < SWARMSIZE)
        {
            dive_record[d_count] = i;
            d_count++;
        }
    }
    
    Maopao(sw_pt, &(mark_record[0]), f_count);
        
    for (int i = 0; i < f_count; i++)
    {
        Copy_swarm(sw_pt, &(sw_new), i, mark_record[i], num_testcase);
    }

    front_num = f_count;
    set_count = f_count;
    
    Update_distance_taea(sw_pt, &(sw_new), &(dive_record[0]), num_testcase, 
            f_count, d_count);
	
    
    Maopao(sw_pt, &(dive_record[0]), d_count); //并没有按照拥挤距离从小到大的顺序排序，不过不影响，反正就是那些点
    
 
	
	for (int i = 0; i < d_count; i++)
    {
        Copy_swarm(sw_pt, &(sw_new), i + set_count, dive_record[i], num_testcase);
    }
	
	
	

    free(mark_dom);
    free(mark_record);
    free(dive_record);


    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        Copy_swarm(&(sw_new), sw_pt, i, i, num_testcase);
    }

    Free_swarm(&(sw_new));
	

    sw_pt->range = front_num;

    return front_num;
}

int Eliter_selection_spea(Swarms *sw_pt, int num_testcase)
{
    int front_num = 0;
    int set_count = 0;

    Swarms sw_new;
    Init_temp_swarm(&(sw_new), num_testcase);

    for (int i = 0; i < sw_pt->range ; i++) sw_pt->efficient[i] = 0;
    for (int i = sw_pt->range; i < SWARMSIZE ; i++) sw_pt->efficient[i] = 1;
    
    for (int i = SWARMSIZE; i < SWARMSIZE * 2; i++) sw_pt->efficient[i] = 0;
    for (int i = 0; i < SWARMSIZE * 2; i++) sw_pt->batch[i] = 0;
    
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[i] = 0;
    for (int i = 0; i < SWARMSIZE; i++) sw_pt->newgen[SWARMSIZE + i] = 1;
    
    bool* mark_dom = NULL;
    mark_dom = (bool*)malloc(sizeof(bool) * SWARMSIZE * 2);
    int* mark_record = NULL;
    mark_record = (int*)malloc(sizeof(int) * SWARMSIZE * 2);
   
    int batch_num = 0;
    while (set_count < SWARMSIZE && batch_num < SWARMSIZE)
    {
        batch_num++;
        
        int mark_record[SWARMSIZE * 2];
        
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            mark_dom[i] = 0;
            mark_record[i] = -1;
        }

        //for (int i = set_count; i < SWARMSIZE * 2; i++)
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            if (sw_pt->efficient[i] == 1) 
            {
                mark_dom[i] = 1;
            }

            if (mark_dom[i] == 1) continue;

            for (int j = i + 1; j < SWARMSIZE * 2; j++)
            {
                if (mark_dom[j] == 1 || sw_pt->efficient[j] == 1) continue;

                int judge = Domain_compare(sw_pt, i, j);
                
                if (judge == -1) continue;
                else if (judge == -2)
                {
                    sw_pt->efficient[j] = 1;
                }
                else if (judge == i)
                {
                    mark_dom[j] = 1;
                }
                else
                {
                    mark_dom[i] = 1;
                    break;
                }
            }
        }
       
        int f_count = 0;
        for (int i = 0; i < SWARMSIZE * 2; i++)
        {
            if (0 == mark_dom[i])
            {
                mark_record[f_count] = i;
                sw_pt->efficient[i] = 1;
                sw_pt->batch[i] = batch_num;
                f_count++;
            }
        }
        
        if (set_count == 0 || set_count + f_count >= SWARMSIZE)
        {
            Maopao(sw_pt, &(mark_record[0]), f_count);
        }
        
        for (int i = 0; i < f_count; i++)
        {
            //Switch_particle(sw_pt, i + set_count, mark_record[i], num_testcase);
            Copy_swarm(sw_pt, &(sw_new), i + set_count, mark_record[i], num_testcase);
        }

        if (0 == set_count) 
        {
            front_num = f_count;
        }

        set_count += f_count;
    }
    
    for (int i = 0; i < set_count; i++)
    {
        Copy_swarm(&(sw_new), sw_pt, i, i, num_testcase);
    }

    Free_swarm(&(sw_new));

    free(mark_dom);
    free(mark_record);
    
    sw_pt->range = front_num;

    return front_num;
}

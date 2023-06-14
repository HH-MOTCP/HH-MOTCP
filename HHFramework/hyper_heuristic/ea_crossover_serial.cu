/*
    For the PSO crossover operation
    on GPU platform, Including the pbest and gbest, and 
    the gbest is been selected from the front set randomly.
 */

#include <iostream>
#include <cuda.h>

#include "pso_config.h"

using namespace std;

/*
__global__ void crossover_g_singlepoint(int *d_purpose, int *d_element1, int *d_element2,
        bool *d_geffic, bool *d_mark, int *d_rand, int num_testcase, int g_num)
{
    int tid = threadIdx.x; //100
    
    int pos = d_rand[tid];
    int g_count = pos % g_num;
    int gbest_num = 0;

    if (d_geffic[g_count] == 1)
    {
        gbest_num = g_count;
    }
    else
    {
        for (int i = 1; i < g_num; i++)
        {
            if (g_count + i >= g_num)
            {
                g_count = 0;
                i = 0;
                continue;
            }
            else if (d_geffic[g_count + i] == 1)
            {
                gbest_num = g_count + i;
                break;
            }
        }
    }
    //d_purpose[tid] = gbest_num; 
        
    for (int i = 0; i < pos; i++)
    {
        d_purpose[tid * MAXGENE + i] = d_element1[tid * MAXGENE + i];
        d_mark[tid * MAXGENE + d_element1[tid * MAXGENE + i]] = 1;
    }

    for (int i = 0; i < num_testcase; i++)
    {
        if (d_mark[tid * MAXGENE + d_element2[gbest_num * MAXGENE + i]] == 0)
        {
            d_purpose[tid * MAXGENE + pos] = d_element2[gbest_num * MAXGENE 
                + i];
            pos += 1;
            //d_mark[tid * MAXGENE + d_element2[gbest_num * MAXGENE + i]] = 1; //***************
        }
    }
}

__global__ void crossover_singlepoint(int *d_purpose, int *d_element1, int *d_element2,
        bool *d_mark, int *d_rand, int num_testcase)
{
    int tid = threadIdx.x; //100
    
    int pos = d_rand[tid];
        
    for (int i = 0; i < pos; i++)
    {
        d_purpose[tid * MAXGENE + i] = d_element1[tid * MAXGENE + i];
        d_mark[tid * MAXGENE + d_element1[tid * MAXGENE + i]] = 1;
    }

    for (int i = 0; i < num_testcase; i++)
    {
        if (d_mark[tid * MAXGENE + d_element2[tid * MAXGENE + i]] == 0)
        {
            d_purpose[tid * MAXGENE + pos] = d_element2[tid * MAXGENE + i];
            pos += 1;
            //d_mark[tid * MAXGENE + d_element2[tid * MAXGENE + i]] = 1; //***************
        }
    }
}

__global__ void cal_distance_order(int *d_order, float *d_apsc, float *d_exetime,
        float max_apsc, float min_apsc, float max_exetime, float min_exetime)
{
    int tid = threadIdx.x; //100
    
    __shared__ float d_max_apsc;
    __shared__ float d_min_apsc;// = min_apsc;
    __shared__ float d_max_exetime;// = max_exetime;
    __shared__ float d_min_exetime;// = min_exetime;
    
    if (tid == 0) 
    {
        d_max_apsc = max_apsc;
        d_min_apsc = min_apsc;
        d_max_exetime = max_exetime;
        d_min_exetime = min_exetime;
    }
    __syncthreads();

    __shared__ float d_distance[SWARMSIZE];
    d_distance[tid] = 0;

    d_distance[tid] = ((d_apsc[tid] - d_min_apsc) / (d_max_apsc - d_min_apsc)) 
        + ((d_exetime[tid] - d_min_exetime) / (d_max_exetime - d_min_exetime));
    
    __syncthreads();
    
    __shared__ int d_count[SWARMSIZE];
    d_count[tid] = 0; __syncthreads();

    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (tid == i) continue;
        if (d_distance[i] > d_distance[tid])
        {
            d_count[tid] += 1;
        }

    }
    __syncthreads();

    d_order[d_count[tid]] = tid;
}

__global__ void copy_gbest(int *d_rand, int *d_gbest, int *d_pg, bool *d_geffic,
            int num_testcase, int g_num)
{
    //int bid = blockIdx.y * gridDim.x + blockIdx.x; // 50 * 6 + 6
    int tid = threadIdx.x; //50

    __shared__ int g_count[50];
    __shared__ int gbest_num[50];


    
    g_count[tid] = d_rand[tid] % g_num;
    gbest_num[tid] = 0;

    
    if (d_geffic[g_count[tid]] == 1)
    {
        gbest_num[tid] = g_count[tid];
    }
    else
    {
        for (int i = 1; i < g_num; i++)
        {
            if (g_count[tid] + i >= g_num)
            {
                g_count[tid] = 0;
                i = 0;
                continue;
            }
            else if (d_geffic[g_count[tid] + i] == 1)
            {
                gbest_num[tid] = g_count[tid] + i;
                break;

            }
        }
    }
    
    __syncthreads();

    for (int i = 0; i < num_testcase; i++)
    {
        d_pg[tid * MAXGENE + i] = d_gbest[gbest_num[tid] * MAXGENE + i];
    }

    //d_pg[blockIdx.x] = g_count;
    //d_pg[bid * blockDim.x + tid] = d_gbest[(gbest_num * gridDim.x + blockIdx.x)
    //    * blockDim.x + tid];

}

__global__ void copy_particle(int *d_order, int *d_particle, int *d_pg,
            int num_testcase)
{
    //int bid = blockIdx.y * gridDim.x + blockIdx.x; // 6 * 50
    int tid = threadIdx.x; // 50

    //__shared__ int p_pos;
    //if (tid == 0) p_pos = d_order[blockIdx.y];
    //__syncthreads();

    for (int i = 0; i < num_testcase; i++)
    {
        d_pg[(tid + SWARMSIZE / 2) * MAXGENE + i] = d_particle[d_order[tid] *
            MAXGENE + i];
    }
    //d_pg[((blockIdx.y + SWARMSIZE / 2) * gridDim.x + blockIdx.x) * blockDim.x +
    //    tid] = d_particle[(p_pos * gridDim.x + blockIdx.x) * blockDim.x + tid];
}

__global__ void ea_crossover(int *d_rand_cross, int *d_pg, int num_rand)
{
    int bid = blockIdx.x;  //100 
    int tid = threadIdx.x; //304

    extern __shared__ int pos[];
    pos[tid] = d_rand_cross[bid * num_rand * 2 + tid];
    pos[tid + num_rand] = d_rand_cross[bid * num_rand * 2 + tid + num_rand];
    __syncthreads();

    //d_pg[bid * num_rand * 2 + tid] = pos[tid];
    //d_pg[bid * num_rand * 2 + tid + num_rand] = pos[tid + num_rand];
    
    int temp_gen = d_pg[bid * MAXGENE + pos[tid]];
    d_pg[bid * MAXGENE + pos[tid]] = d_pg[bid * MAXGENE + pos[tid + num_rand]];
    d_pg[bid * MAXGENE + pos[tid + num_rand]] = temp_gen; 
}
*///bytest

void distance_order_serial(int *order, float *apsc, float *exetime, float max_apsc,
            float min_apsc, float max_exetime, float min_exetime)
{
    float distance[SWARMSIZE];
    float temp_apsc = max_apsc - min_apsc;
    float temp_exetime = max_exetime - min_exetime;
    
    for (int i = 0; i < SWARMSIZE; i++)
    {
        distance[i] = ((apsc[i] - min_apsc) / temp_apsc) + ((exetime[i] -
                    min_apsc) / temp_exetime);
    }
    
    for (int i = 0; i < SWARMSIZE; i++) order[i] = i;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = i + 1; j < SWARMSIZE; j++)
        {
            if (distance[i] < distance[j])
            {
                float temp_dis = 0;
                int temp_order = 0;

                temp_dis = distance[i];
                distance[i] = distance[j];
                distance[j] = temp_dis;

                temp_order = order[i];
                order[i] = order[j];
                order[j] = temp_order;
            }
        }
    }
}

void ea_crossover_serial(int *rand_cross, int *dp, int num_rand)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_rand; j++)
        {
            int temp_change = dp[i * MAXGENE + rand_cross[i * 2 * num_rand +
                j]];
            dp[i * MAXGENE + rand_cross[i * 2 * num_rand + j]] = dp[i * MAXGENE
                + rand_cross[i * 2 * num_rand + j + num_rand]];
            dp[i * MAXGENE + rand_cross[i * 2 * num_rand + j + num_rand]] =
                temp_change;
        }
    }
}

extern "C" void EA_crossover_serial(Swarms* sw_pt, int num_testcase)
{
    //int *d_particle = NULL;
    //int *d_pbest = NULL, *d_speed = NULL;
    //float *d_apsc = NULL, *d_exetime = NULL;
    //size_t ind_size = SWARMSIZE * MAXGENE;

    //CUDA_CALL(cudaMalloc((void**)&d_particle, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMemset(d_particle, 0, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMalloc((void**)&d_pbest, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMemset(d_pbest, 0, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMalloc((void**)&d_speed, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMemset(d_speed, 0, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMalloc((void**)&d_apsc, sizeof(float) * SWARMSIZE));
    //CUDA_CALL(cudaMemset(d_apsc, 0, sizeof(float) * SWARMSIZE));
    //CUDA_CALL(cudaMalloc((void**)&d_exetime, sizeof(float) * SWARMSIZE));
    //CUDA_CALL(cudaMemset(d_exetime, 0, sizeof(float) * SWARMSIZE));

    //int *d_gbest = NULL;
    //bool *d_geffic = NULL;
    //size_t g_size = (sw_pt->EAlen) * MAXGENE;
    
    //CUDA_CALL(cudaMalloc((void**)&d_gbest, sizeof(int) * g_size));
    //CUDA_CALL(cudaMemset(d_gbest, 0, sizeof(int) * g_size));
    //CUDA_CALL(cudaMalloc((void**)&d_geffic, sizeof(bool) * sw_pt->EAlen));
    //CUDA_CALL(cudaMemset(d_geffic, 0, sizeof(bool) * sw_pt->EAlen));

    //CUDA_CALL(cudaMemcpy(d_particle, &(sw_pt->particle[0]), sizeof(int) * ind_size,
    //            cudaMemcpyHostToDevice));
    //CUDA_CALL(cudaMemcpy(d_pbest, &(sw_pt->pbest[0]), sizeof(int) * ind_size,
    //            cudaMemcpyHostToDevice));
    //CUDA_CALL(cudaMemcpy(d_speed, &(sw_pt->speed[0]), sizeof(int) * ind_size,
    //            cudaMemcpyHostToDevice));
    //CUDA_CALL(cudaMemcpy(d_apsc, &(sw_pt->apsc[0]), sizeof(float) * SWARMSIZE,
    //            cudaMemcpyHostToDevice));
    //CUDA_CALL(cudaMemcpy(d_exetime, &(sw_pt->exetime[0]), sizeof(float) *
    //            SWARMSIZE, cudaMemcpyHostToDevice));
    //CUDA_CALL(cudaMemcpy(d_gbest, &(sw_pt->EA[0]), sizeof(int) * g_size,
    //            cudaMemcpyHostToDevice));
    //CUDA_CALL(cudaMemcpy(d_geffic, &(sw_pt->EAefficient[0]), sizeof(bool) *
    //            sw_pt->EAlen, cudaMemcpyHostToDevice));

    //int *d_pg = NULL, *d_sp = NULL; //pg: pbest & gbest  sp:speed & pbest
    //int *d_order = NULL;
    //bool *d_mark = NULL;
    //CUDA_CALL(cudaMalloc((void**)&d_order, sizeof(int) * SWARMSIZE));
    //CUDA_CALL(cudaMemset(d_order, 0,sizeof(int) * SWARMSIZE));
    //CUDA_CALL(cudaMalloc((void**)&d_pg, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMemset(d_pg, 0, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMalloc((void**)&d_sp, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMemset(d_sp, 0, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMalloc((void**)&d_mark, sizeof(bool) * ind_size));
    //CUDA_CALL(cudaMemset(d_mark, 0, sizeof(bool) * ind_size));

    float max_apsc = (float)MIN, min_apsc = (float)MAX;
    float max_exetime = (float)MIN, min_exetime = (float)MAX;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (sw_pt->apsc[i] > max_apsc)
        {
            max_apsc = sw_pt->apsc[i];
        }
        if (sw_pt->apsc[i] < min_apsc)
        {
            min_apsc = sw_pt->apsc[i];
        }
        if (sw_pt->exetime[i] > max_exetime)
        {
            max_exetime = sw_pt->exetime[i];
        }
        if (sw_pt->exetime[i] < min_exetime)
        {
            min_exetime = sw_pt->exetime[i];
        }
    }

    //cal_distance_order<<<1, SWARMSIZE>>>(d_order, d_apsc, d_exetime, max_apsc,
    //        min_apsc, max_exetime, min_exetime);
    
    int order[SWARMSIZE];
    for (int i = 0; i < SWARMSIZE; i++) order[i] = 0;

    distance_order_serial(&(order[0]), sw_pt->apsc, sw_pt->exetime, max_apsc,
            min_apsc, max_exetime, min_exetime);

    int ea_count = 0;
    int ea_member[ARCHSIZE];
    for (int i = 0; i < ARCHSIZE; i++) ea_member[i] = 0;

    for (int i = 0; i < sw_pt->EAlen; i++)
    {
        if (sw_pt->EAefficient[i] == 1)
        {
            ea_member[ea_count] = i;
            ea_count += 1;
        }
    }

    int percent = SWARMSIZE / (2 * ea_count);
    int g_order[SWARMSIZE / 2];
    for (int i = 0; i < SWARMSIZE / 2; i++) g_order[i] = 0;
    for (int i = 0; i < ea_count; i++)
    {
        for (int j = 0; j < percent; j++)
        {
            g_order[i * percent + j] = ea_member[i];
        }
    }

    int *dp = NULL;
    dp = (int*)malloc(sizeof(int) * SWARMSIZE * MAXGENE);
    for (int i = 0; i < SWARMSIZE / 2; i++)
    {
        for (int j = 0; j < num_testcase; j++)
        {
            dp[i * MAXGENE + j] = sw_pt->EA[g_order[i] * MAXGENE + j];
        }
    }

    for (int i = SWARMSIZE / 2; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++)
        {
            dp[i * MAXGENE + j] = sw_pt->particle[order[i] * MAXGENE + j];
        }
    }

    //int *rand_num = NULL, *d_rand = NULL;
    //rand_num = (int*)malloc(sizeof(int) * SWARMSIZE);
    //CUDA_CALL(cudaMalloc((void**)&d_rand, sizeof(int) * SWARMSIZE));
    //CUDA_CALL(cudaMemset(d_rand, 0, sizeof(int) * SWARMSIZE));
     
    //srand((unsigned int)time(0));
    //for (int i = 0; i < SWARMSIZE / 2; i++)
    //    rand_num[i] = rand() % sw_pt->eff_num[i];
        //rand_num[i] = rand() % num_testcase;
    //CUDA_CALL(cudaMemcpy(d_rand, rand_num, sizeof(int) * SWARMSIZE / 2,
    //            cudaMemcpyHostToDevice));
 
    //int blockx = (num_testcase + 512) / 512;
    //dim3 grid(blockx, SWARMSIZE / 2);

    //copy_gbest<<<grid, 512>>>(d_rand, d_gbest, d_pg, d_geffic, num_testcase, 
    //        sw_pt->EAlen);

    //copy_gbest<<<1, SWARMSIZE / 2>>>(d_rand, d_gbest, d_pg, d_geffic, num_testcase, 
    //        sw_pt->EAlen);

    //copy_particle<<<1, SWARMSIZE / 2>>>(d_order, d_particle, d_pg, num_testcase);

    int *rand_cross = NULL; //, *d_rand_cross = NULL;
    int num_rand = (int)((num_testcase * MUTRATE) / 8) * 8;
    if (num_rand > 512) num_rand = 512;

    rand_cross = (int*)malloc(sizeof(int) * num_rand * 2 * SWARMSIZE);
    //CUDA_CALL(cudaMalloc((void**)&d_rand_cross, sizeof(int) * num_rand * 2 *
    //            SWARMSIZE));
    
    int *str_mark = (int*)malloc(sizeof(int) * num_testcase);
    //srand((unsigned int)time(NULL)); 
    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++) str_mark[j] = 0;
        
        srand((unsigned int)time(NULL) * i); 
        for (int j = 0; j < num_rand * 2; j++)
        {
            int temp_rand = rand() % num_testcase;
            if (str_mark[temp_rand] != 0)
            {
                for (int k = 1; k < num_testcase; k += 7)
                {
                    if (temp_rand + k >= num_testcase)
                    {
                        temp_rand = 0;
                    }
                    if (str_mark[temp_rand + k] == 0)
                    {
                        rand_cross[i * num_rand * 2 + j] = temp_rand + k;
                        str_mark[temp_rand + k] += 1;
                        break;
                    }
                    else continue;
                }
            }
            else
            {
                rand_cross[i * num_rand * 2 + j] = temp_rand;
                str_mark[temp_rand] += 1;
            }
        }
    }
    free(str_mark);

    ea_crossover_serial(rand_cross, dp, num_rand);

    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = 0; j < num_testcase; j++)
        {
            sw_pt->particle[i * MAXGENE + j] = dp[i * MAXGENE + j];
        }
    }
    //CUDA_CALL(cudaMemcpy(d_rand_cross, rand_cross, sizeof(int) * num_rand * 2 *
    //            SWARMSIZE, cudaMemcpyHostToDevice));

    //ea_crossover<<<SWARMSIZE, num_rand, sizeof(int) * num_rand * 2>>>(d_rand_cross, d_pg, num_rand);

    //crossover_g_singlepoint<<<1, SWARMSIZE>>>(d_pg, d_particle, d_gbest, d_geffic, d_mark, d_rand,
    //        num_testcase, sw_pt->EAlen);
    //CUDA_CALL(cudaMemcpy(&(sw_pt->particle[0]), d_pg, sizeof(int) * ind_size,
    //            cudaMemcpyDeviceToHost));
    /*
    CUDA_CALL(cudaMemset(d_mark, 0, sizeof(bool) * ind_size));
    srand((unsigned int)time(0) * 2);
    for (int i = 0; i < SWARMSIZE; i++)
        rand_num[i] = rand() % sw_pt->eff_num[i];
        //rand_num[i] = rand() % num_testcase;
    CUDA_CALL(cudaMemcpy(d_rand, rand_num, sizeof(int) * SWARMSIZE,
                cudaMemcpyHostToDevice));
    
    crossover_singlepoint<<<1, SWARMSIZE>>>(d_sp, d_speed, d_pg, d_mark, d_rand, num_testcase);
    CUDA_CALL(cudaMemcpy(&(sw_pt->speed[0]), d_sp, sizeof(int) * ind_size,
                cudaMemcpyDeviceToHost));

    CUDA_CALL(cudaMemset(d_mark, 0, sizeof(bool) * ind_size));
    srand((unsigned int)time(0) * 3);
    for (int i = 0; i < SWARMSIZE; i++)
        rand_num[i] = rand() % sw_pt->eff_num[i];
        //rand_num[i] = rand() % num_testcase;
    CUDA_CALL(cudaMemcpy(d_rand, rand_num, sizeof(int) * SWARMSIZE,
                cudaMemcpyHostToDevice));
    
    crossover_singlepoint<<<1, SWARMSIZE>>>(d_pg, d_particle, d_sp, d_mark, d_rand, num_testcase);
    CUDA_CALL(cudaMemcpy(&(sw_pt->particle[0]), d_pg, sizeof(int) * ind_size,
                cudaMemcpyDeviceToHost));
    */

    //crossover_g_singlepoint<<<1, SWARMSIZE>>>(d_pg, d_particle, d_gbest, d_geffic, d_mark, d_rand,
    //        num_testcase, sw_pt->EAlen);
    //CUDA_CALL(cudaMemcpy(&(sw_pt->particle[0]), d_pg, sizeof(int) * ind_size,
    //            cudaMemcpyDeviceToHost));

    //testby-------------
    /*
    int *out = NULL;
    out = (int*)malloc(sizeof(int) * ind_size);
    cudaMemcpy(out, d_pg, sizeof(int) * ind_size,cudaMemcpyDeviceToHost);
    
    int *check = NULL;
    check = (int*)malloc(sizeof(int) * num_testcase);
    for (int i = 0; i < num_testcase; i++) check[i] = 0;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < num_testcase; j++) check[j] = 0;

        for (int j = 0; j < num_testcase; j++)
        {
            check[out[j]] += 1;
        }

        for (int j = 0; j < num_testcase; j++)
        {
            if (check[j] != 1)
            {
                cout << "pos: " << i << "  num: " << j << " error: " << check[j] << endl;
            }
        }
    }
    
    cout << endl <<endl;
    cout << "Finish" << endl;
    cout << endl;
    getchar();

    free(out);
    */
    //-------------
    
    //CUDA_CALL(cudaFree(d_particle));
    //CUDA_CALL(cudaFree(d_pbest));
    //CUDA_CALL(cudaFree(d_gbest));
    //CUDA_CALL(cudaFree(d_speed));
    //CUDA_CALL(cudaFree(d_geffic));
    //CUDA_CALL(cudaFree(d_apsc));
    //CUDA_CALL(cudaFree(d_exetime));
    //CUDA_CALL(cudaFree(d_pg));
    //CUDA_CALL(cudaFree(d_sp));
    //CUDA_CALL(cudaFree(d_rand));
    //free(rand_num);
    free(rand_cross);
    //CUDA_CALL(cudaFree(d_rand_cross));
}

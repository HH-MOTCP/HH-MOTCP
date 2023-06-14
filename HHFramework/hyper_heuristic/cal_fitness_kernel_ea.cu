/* 
   For fitness calcualte in pso algorithm 
   the parallel version.
 */


#include "pso_config.h"
#include <iomanip>
//#include "cal_fitness_kernel.h"
#include <cuda.h>

using namespace std;
/*
__global__ void apsc_parallel(bool *d_CMatrix, int *d_ind, int *d_temp_cover,
        bool *d_max_cover, int num_testcase)
{
    int bid = blockIdx.y * gridDim.x + blockIdx.x;  // y:100  x:block_num 
    int bdim = blockDim.x;                          // 512
    int tid = threadIdx.x;                          // 0 - 511

    __shared__ bool statement[512]; //Record the max_coverage array
    statement[tid] = d_max_cover[blockIdx.x * bdim + tid];
    __syncthreads();
    
    //size_t in_size = (MAXGENE + 512)/512;
    __shared__ int individuals[MAXGENE + 512]; //Record the individuals
    for (int i = 0; i < (MAXGENE+512)/512; i++)
    {
        individuals[i * 512 + tid] = d_ind[blockIdx.y * MAXGENE + i * 512 + tid];
    }
    __syncthreads();

    
    __shared__ int record[512];
    record[tid] = 0;
    __syncthreads();
    
    if (statement[tid])
    {
        int line_num = blockIdx.x * bdim + tid;

        for (int i = 0; i < num_testcase; i++)
        {
            if (d_CMatrix[individuals[i] * CODELINE + line_num])
            {
                record[tid] = i + 1;
                break;
            }
        }
    }
    __syncthreads();
    
    d_temp_cover[bid * bdim + tid] = record[tid];
    //d_temp_cover[bid * bdim +tid] = statement[tid];
    __syncthreads();
}

__global__ void cal_apsc(int *d_temp_cover, float *d_apsc, int *d_effnum, int
        block_num, int num_testcase)
{
    int bid = blockIdx.x;
    int tid = threadIdx.x;

    __shared__ int total_cover[512];
    total_cover[tid] = 0;
    __shared__ int max_eff[512];
    max_eff[tid] = 0;
    
    __syncthreads();

    for (int i = 0; i < block_num; i++)
    {
        int pos = bid * block_num * 512 + i * 512 + tid;
        total_cover[tid] += d_temp_cover[pos];
        if (max_eff[tid] < d_temp_cover[pos])
        {
            max_eff[tid] = d_temp_cover[pos];
        }
    }
    __syncthreads();

    for (int i = 256; i > 0; i = i/2)
    {
        if (tid < i)
        {
            total_cover[tid] += total_cover[tid + i];
            if (max_eff[tid] < max_eff[tid + i])
            {
                max_eff[tid] = max_eff[tid + i];
            }
        }
    }
    __syncthreads();

    if (tid == 0)
    {
        d_apsc[bid] = 1.0f - ((float)total_cover[0] / (float)(CODELINE *
                    num_testcase)) + 1.0f / (float)(2 * num_testcase);
        d_effnum[bid] = max_eff[0];
        //d_effnum[bid] = total_cover[0];

    }
    __syncthreads();
}

__global__ void cal_efftime(int *d_ind, int *d_effnum, float *d_TVector,
        int num_testcase, float *d_exetime)
{
    int tid = threadIdx.x;

    extern __shared__ float tvector[];

    int limit = (num_testcase + 512)/512;
    for (int i = 0; i < limit; i++)
    {
        int pos = i * 512 + tid;
        if (pos < num_testcase)
        {
            tvector[pos] = d_TVector[pos];
        }
    }

    __shared__ int loop_limit[SWARMSIZE];
    __shared__ float total_time[SWARMSIZE];
    if (tid < SWARMSIZE)
    {
        loop_limit[tid] = d_effnum[tid];
        total_time[tid] = 0.0f;
    }
    __syncthreads();

    if (tid < SWARMSIZE)
    {
        for (int i = 0; i < loop_limit[tid]; i++)
        {
            int tc_num = d_ind[tid * MAXGENE + i];
            total_time[tid] += tvector[tc_num];
        }
    }
    __syncthreads();

    if (tid < SWARMSIZE)
    {
        d_exetime[tid] = total_time[tid];
    }
    __syncthreads();
    
}

__global__ void cal_front(float *d_apsc, float *d_exetime, bool *d_efficient)
{
    int tid = threadIdx.x;
    
    __shared__ float apsc[SWARMSIZE];
    apsc[tid] = d_apsc[tid];

    __shared__ float exetime[SWARMSIZE];
    exetime[tid] = d_exetime[tid];

    __shared__ bool efficient[SWARMSIZE];
    efficient[tid] = 1;

    __syncthreads();

    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (apsc[tid] < apsc[i] && exetime[tid] > exetime[i])
        {
            efficient[tid] = 0;
            break;
        }
    }

    d_efficient[tid] = efficient[tid];

}
*/// bytest

void cal_apsc_serial(bool *CMatrix, int *particle, float *apsc, int *eff_num, 
        bool *max_cover, int num_testcase)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        int sum_testnum = 0;
        int max_testnum = 0;

        for (int j = 0; j < CODELINE; j++)
        {
            if (max_cover[j] == 0) continue;

            for (int k = 0; k < num_testcase; k++)
            {
                bool temp = CMatrix[particle[i * MAXGENE + k] * 
                    CODELINE + j]; 
                
                if (temp == 1)
                {
                    k += 1;
                    sum_testnum += k; 
                    if (k > max_testnum) max_testnum = k;
                    break;
                }
            }
        }
        
        eff_num[i] = max_testnum;
        apsc[i] = 1 - ((float)sum_testnum / (float)(CODELINE * num_testcase))
            + 1.0f / (float)(2 * num_testcase);
    }
}

void cal_efftime_serial(int *particle, int *effnum, float *TVector, float
        *exetime)
{
    for (int i = 0; i < SWARMSIZE; i++)
    {
        float sum_time = 0.0f;
        for (int j = 0; j < effnum[i]; j++)
        {
            sum_time += TVector[particle[j]];       
        }
        exetime[i] = sum_time;
    }
}

void cal_front_serial(float *apsc, float *exetime, bool *efficient, 
        int *eff_mark)
{
    for (int i = 0; i < SWARMSIZE; i++) efficient[i] = 1;
    //for (int i = 0; i < SWARMSIZE; i++) eff_mark[i] = 0;

    for (int i = 0; i < SWARMSIZE; i++)
    {
        for (int j = i + 1; j < SWARMSIZE; j++)
        {
            if (efficient[j] == 0) continue;

            if (apsc[i] < apsc[j] && exetime[i] > exetime[j])
            {
                efficient[i] = 0;
                break;
            }
            else if (apsc[i] > apsc[j] && exetime[i] < exetime[j])
            {
                efficient[j] = 0;
            }
        }
    }
}

extern "C" void Cal_fitness(Swarms* sw_pt, int num_testcase, bool* CMatrix, float*
        TVector, bool* max_cover)
{
    //size_t ind_size = SWARMSIZE * MAXGENE;

    //int *d_ind = NULL;
    //CUDA_CALL(cudaMalloc((void**)&d_ind, sizeof(int) * ind_size));
    //CUDA_CALL(cudaMemcpy(d_ind, &(sw_pt->particle[0]), sizeof(int) * ind_size,
    //            cudaMemcpyHostToDevice));
    
    //float *d_apsc = NULL;
    //int *d_effnum = NULL;
    //float *d_exetime = NULL;
    //bool *d_efficient = NULL;
    //CUDA_CALL(cudaMalloc((void**)&d_apsc, sizeof(float) * SWARMSIZE));
    //CUDA_CALL(cudaMalloc((void**)&d_effnum, sizeof(int) * SWARMSIZE));
    //CUDA_CALL(cudaMalloc((void**)&d_exetime, sizeof(float) * SWARMSIZE));
    //CUDA_CALL(cudaMalloc((void**)&d_efficient, sizeof(bool) * SWARMSIZE));
    //CUDA_CALL(cudaMemset(d_efficient, 0, sizeof(bool) * SWARMSIZE));

    //int block_num = (CODELINE + 512)/512;
    //cout << "Block Number: " << block_num << endl; 
    //cout << "Test case NO: " << num_testcase << endl;

    //int *d_temp_cover = NULL;
    //CUDA_CALL(cudaMalloc((void**)&d_temp_cover, sizeof(int) * SWARMSIZE *
    //            block_num * 512));
    //CUDA_CALL(cudaMemset(d_temp_cover, 0, sizeof(int) * SWARMSIZE * block_num *
    //            512));

    //dim3 grid(block_num, SWARMSIZE);
    
    /*
    apsc_parallel<<<grid, 512>>>(d_CMatrix, d_ind, d_temp_cover, d_max_cover,
            num_testcase);
    
    cal_apsc<<<SWARMSIZE, 512>>>(d_temp_cover, d_apsc, d_effnum, block_num,
            num_testcase);
    */

    cudaEvent_t fstime, fetime;

    CUDA_CALL(cudaEventCreate(&fstime, 0));
    CUDA_CALL(cudaEventCreate(&fetime, 0));

    cudaEventRecord(fstime, 0);

    cal_apsc_serial(CMatrix, &(sw_pt->particle[0]), &(sw_pt->apsc[0]),
            &(sw_pt->eff_num[0]), max_cover, num_testcase);

    cudaEventRecord(fetime, 0);
    cudaEventSynchronize(fetime);
    float cptime = 0.0f;
    cudaEventElapsedTime(&cptime, fstime, fetime);
    //cout << "   cal_apsc time: " << cptime << endl;
    //size_t tv_size = num_testcase * sizeof(float);
    //cal_efftime<<<1, 512, tv_size>>>(d_ind, d_effnum, d_TVector, num_testcase,
    //        d_exetime);
    cudaEventRecord(fstime, 0);

    cal_efftime_serial(&(sw_pt->particle[0]), &(sw_pt->eff_num[0]), TVector, 
            &(sw_pt->exetime[0]));

    cudaEventRecord(fetime, 0);
    cudaEventSynchronize(fetime);
    cptime = 0.0f;
    cudaEventElapsedTime(&cptime, fstime, fetime);
    //cout << "   cal_efftime time: " << cptime << endl;
    //CUDA_CALL(cudaMemcpy(&(sw_pt->apsc[0]), d_apsc, sizeof(float) * SWARMSIZE,
    //            cudaMemcpyDeviceToHost));
    //CUDA_CALL(cudaMemcpy(&(sw_pt->eff_num[0]), d_effnum, sizeof(int) * SWARMSIZE,
    //            cudaMemcpyDeviceToHost));
    //CUDA_CALL(cudaMemcpy(&(sw_pt->exetime[0]), d_exetime, sizeof(float) *
    //            SWARMSIZE, cudaMemcpyDeviceToHost));
    
    //Update the particle best with 1/2 rate 
    /*
    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (sw_pt->apsc[i] > sw_pt->papsc[i] && sw_pt->exetime[i] <
                sw_pt->pexetime[i])
        {
            for (int j = 0; j < MAXGENE; j ++)
            {
                sw_pt->pbest[i * MAXGENE + j] = sw_pt->particle[i * MAXGENE +
                    j];
            }

            sw_pt->papsc[i] = sw_pt->apsc[i];
            sw_pt->pexetime[i] = sw_pt->exetime[i];
        }
        else if (sw_pt->apsc[i] <= sw_pt->papsc[i] && sw_pt->exetime[i] >=
                sw_pt->pexetime[i])
        {
            continue;
        }
        else
        {
            srand((unsigned int)time(0) * i);
            int rand_num = rand()%2;
            if (rand_num)
            {
                for (int j = 0; j < MAXGENE; j++)
                {
                    sw_pt->pbest[i * MAXGENE + j] = sw_pt->particle[i * MAXGENE +
                        j];
                }
                sw_pt->papsc[i] = sw_pt->apsc[i];
                sw_pt->pexetime[i] = sw_pt->exetime[i];
            }
            else continue;
        }
    }
    */
    //cal_front<<<1, SWARMSIZE>>>(d_apsc, d_exetime, d_efficient);
    cudaEventRecord(fstime, 0);

    int eff_mark[SWARMSIZE];
    cal_front_serial(&(sw_pt->apsc[0]), &(sw_pt->exetime[0]), 
            &(sw_pt->efficient[0]), &(eff_mark[0])); 
  
    cudaEventRecord(fetime, 0);
    cudaEventSynchronize(fetime);
    cptime = 0.0f;
    cudaEventElapsedTime(&cptime, fstime, fetime);
    //cout << "   cal_front time: " << cptime << endl;
    //CUDA_CALL(cudaMemcpy(&(sw_pt->efficient[0]), d_efficient, sizeof(bool) *
    //            SWARMSIZE, cudaMemcpyDeviceToHost));
    
    // Update the front set

    cudaEventRecord(fstime, 0);

    int eff_num[SWARMSIZE];
    int count = 0;
    for (int i = 0; i < SWARMSIZE; i++)
    {
        if (sw_pt->efficient[i] == 1)
        {
            eff_num[count] = i;
            count++;
        }
    }
   
    bool *mark = NULL;
    mark = (bool*)malloc(sizeof(bool) * count);
    for (int i = 0; i < count; i++) mark[i] = 1;
    
    //testby---------------------------------------
    /* 
    sw_pt->EAapsc[0] = 0.990;
    sw_pt->EAexetime[0] = 90.09;
    sw_pt->EAefficient[0] = 1;
    
    sw_pt->EAapsc[5] = 0.995;
    sw_pt->EAexetime[5] = 100.09;
    sw_pt->EAefficient[5] = 1;
    
    sw_pt->EAlen = 6;
    */

    for (int i = 0; i <= sw_pt->EAlen; i++)
    {
        if (sw_pt->EAefficient[i] == 0)
        {
            continue;
        }
        for (int j = 0; j < count; j++)
        {
            if ((sw_pt->EAapsc[i] > sw_pt->apsc[eff_num[j]]) && (sw_pt->EAexetime[i]
                    < sw_pt->exetime[eff_num[j]]))
            {
                mark[j] = 0;
            }
            else if ((sw_pt->EAapsc[i] < sw_pt->apsc[eff_num[j]]) && (sw_pt->EAexetime[i]
                    > sw_pt->exetime[eff_num[j]]))
            {
                sw_pt->EAefficient[i] = 0;
            }
            else continue;
        }
    }

    //Output------------------------------
    /*
    cout << "count: " << count << endl;
    for (int i = 0; i < count; i++)
    {
        cout << "M: " << mark[i] << endl;
        cout << sw_pt->apsc[eff_num[i]] << endl;
        cout << sw_pt->exetime[eff_num[i]] << endl << endl;
    }
    */

    int front_tail = 0;
    for (int i = 0; i < count; i++)
    {
        if (mark[i] == 0) continue;
        for (int j = front_tail; j < ARCHSIZE; j++)
        {
            if (sw_pt->EAefficient[j] == 0)
            {
                for (int k = 0; k < num_testcase; k++)
                {
                    sw_pt->EA[j * MAXGENE + k] = sw_pt->particle[eff_num[i] *
                        MAXGENE + k];
                }
                sw_pt->EAapsc[j] = sw_pt->apsc[eff_num[i]];
                sw_pt->EAexetime[j] = sw_pt->exetime[eff_num[i]];
                sw_pt->EAefficient[j] = 1;
                front_tail = j + 1;

                if (front_tail > sw_pt->EAlen)
                {
                    sw_pt->EAlen = front_tail;
                }
                break;
            }
            
        }
    }
    
    cudaEventRecord(fetime, 0);
    cudaEventSynchronize(fetime);
    cptime = 0.0f;
    cudaEventElapsedTime(&cptime, fstime, fetime);
    //cout << "   update front time: " << cptime << endl;
    //cout << "Lens: " << sw_pt->EAlen << endl;
    
    //Update the front set end
    /*    
    //The temperate parameters to test the parallel programs.
    //It can leave here untial the whole sure to be finished.

    int *temp_cover = NULL;
    temp_cover = (int*)malloc(sizeof(int) * SWARMSIZE * block_num * 512);
    CUDA_CALL(cudaMemcpy(temp_cover, d_temp_cover, sizeof(int) * SWARMSIZE *
                block_num * 512, cudaMemcpyDeviceToHost));
    
    float *temp_total = NULL;
    temp_total = (float*)malloc(sizeof(float) * SWARMSIZE);
    CUDA_CALL(cudaMemcpy(temp_total, d_exetime, sizeof(float) * SWARMSIZE, 
                cudaMemcpyDeviceToHost));
    int *temp_eff = NULL;
    temp_eff = (int*)malloc(sizeof(int) * SWARMSIZE);
    CUDA_CALL(cudaMemcpy(temp_eff, d_effnum, sizeof(int) * SWARMSIZE, 
                cudaMemcpyDeviceToHost));

    CUDA_CALL(cudaFree(temp_cover));
    CUDA_CALL(cudaFree(temp_total));
    CUDA_CALL(cudaFree(temp_eff));
    */    

    //free(temp_cover);
    /*
    CUDA_CALL(cudaFree(d_ind));
    CUDA_CALL(cudaFree(d_apsc));
    CUDA_CALL(cudaFree(d_exetime));
    CUDA_CALL(cudaFree(d_efficient));
    CUDA_CALL(cudaFree(d_temp_cover));
    */
}

CFLAGS = #-G
LIBS = #-L/usr/local/C/lib
LDFLAGS = 
INCLUDE = #-I/usr/local/C/common/inc
CC = g++#nvcc
RM = rm -f
OBJ = nsga_config.o check_file.o read_file.o init_ind.o free_mem.o copy_swarm.o domain_judge.o cal_fitness_kernel.o cal_fitness_kernel_binary.o main_serial.o nsga_selection.o nsga_crossover.o nsga_crossover_binary.o nsga_mutation.o check_sequence.o eliter_selection.o pso_eliter_selection.o nsga.o nsga_binary.o pso_binary.o pso.o pso_selection.o pso_crossover_nor.o taea_binary.o taea.o spea_selection.o spea.o spea_binary.o taea_selection.o check_front.o learning.o init_sch.o update_schema.o greedy_binary.o greedy.o paes.o paes_binary.o

OPTION = -c
ARCH = #-arch=sm_20
OUTPUT_FLAG = -g
OPT = -O2
OUTPUT_OPTION = -o
EXE = main_c_rand.out#nsga2_parallel.out

$(EXE):$(OBJ) 
	$(CC) $(LDFLAGS) $(CFLAGS) $(OPT) $(ARCH) $(OUTPUT_OPTION) $(EXE) $(OBJ) $(INCLUDE) $(LIBS) $(LDFLAGS)

nsga_config.o : nsga_config.h nsga_config.cc
	$(CC) $(CFLAGS) $(OPTION) nsga_config.cc $(OPT) $(OUTPUT_OPTION) nsga_config.o

check_file.o : nsga_config.h check_file.h check_file.cc
	$(CC) $(CFLAGS) $(OPTION) check_file.cc $(OPT) $(OUTPUT_OPTION) check_file.o

read_file.o : nsga_config.h read_file.h read_file.cc
	$(CC) $(CFLAGS) $(OPTION) read_file.cc $(OPT) $(OUTPUT_OPTION) read_file.o

init_ind.o : nsga_config.h init_ind.cc
	$(CC) $(CFLAGS) $(OPTION) init_ind.cc $(OPT) $(OUTPUT_OPTION) init_ind.o 

init_sch.o : nsga_config.h init_sch.cc
	$(CC) $(CFLAGS) $(OPTION) init_sch.cc $(OPT) $(OUTPUT_OPTION) init_sch.o 

free_mem.o : nsga_config.h free_mem.cc
	$(CC) $(CFLAGS) $(OPTION) free_mem.cc $(OPT) $(OUTPUT_OPTION) free_mem.o 

copy_swarm.o : nsga_config.h copy_swarm.cc
	$(CC) $(CFLAGS) $(OPTION) copy_swarm.cc $(OPT) $(OUTPUT_OPTION) copy_swarm.o 

cal_fitness_kernel.o : nsga_config.h cal_fitness_kernel.cc
	$(CC) $(CFLAGS) $(OPTION) cal_fitness_kernel.cc $(OPT) $(OUTPUT_OPTION) cal_fitness_kernel.o

cal_fitness_kernel_binary.o : nsga_config.h cal_fitness_kernel_binary.cc
	$(CC) $(CFLAGS) $(OPTION) cal_fitness_kernel_binary.cc $(OPT) $(OUTPUT_OPTION) cal_fitness_kernel_binary.o

domain_judge.o : nsga_config.h domain_judge.cc
	$(CC) $(CFLAGS) $(OPTION) domain_judge.cc $(OPT) $(OUTPUT_OPTION) domain_judge.o 

nsga_selection.o : nsga_config.h nsga_selection.cc domain_judge.o
	$(CC) $(CFLAGS) $(OPTION) nsga_selection.cc $(OPT) $(OUTPUT_OPTION) nsga_selection.o

nsga_crossover.o : nsga_config.h nsga_crossover.cc
	$(CC) $(CFLAGS) $(OPTION) nsga_crossover.cc $(OPT) $(OUTPUT_OPTION) nsga_crossover.o 

nsga_crossover_binary.o : nsga_config.h nsga_crossover_binary.cc
	$(CC) $(CFLAGS) $(OPTION) nsga_crossover_binary.cc $(OPT) $(OUTPUT_OPTION) nsga_crossover_binary.o 

eliter_selection.o : nsga_config.h eliter_selection.cc domain_judge.o copy_swarm.o
	$(CC) $(CFLAGS) $(OPTION) eliter_selection.cc $(OPT) $(OUTPUT_OPTION) eliter_selection.o
 
pso_eliter_selection.o : nsga_config.h pso_eliter_selection.cc domain_judge.o copy_swarm.o
	$(CC) $(CFLAGS) $(OPTION) pso_eliter_selection.cc $(OPT) $(OUTPUT_OPTION) pso_eliter_selection.o
 
nsga_mutation.o : nsga_mutation.h nsga_mutation.cc
	$(CC) $(CFLAGS) $(OPTION) nsga_mutation.cc $(OPT) $(OUTPUT_OPTION) nsga_mutation.o

check_sequence.o : check_sequence.h check_sequence.cc
	$(CC) $(CFLAGS) $(OPTION) check_sequence.cc $(OPT) $(OUTPUT_OPTION) check_sequence.o

check_front.o : check_front.h nsga_config.h check_front.cc domain_judge.o
	$(CC) $(CFLAGS) $(OPTION) check_front.cc $(OPT) $(OUTPUT_OPTION) check_front.o

learning.o : nsga_config.h learning.cc
	$(CC) $(CFLAGS) $(OPTION) learning.cc $(OPT) $(OUTPUT_OPTION) learning.o 

update_schema.o : nsga_config.h update_schema.cc
	$(CC) $(CFLAGS) $(OPTION) update_schema.cc $(OPT) $(OUTPUT_OPTION) update_schema.o 

nsga.o : nsga.h nsga_config.h nsga.cc  learning.o
	$(CC) $(CFLAGS) $(OPTION) nsga.cc $(OPT) $(OUTPUT_OPTION) nsga.o

nsga_binary.o : nsga.h nsga_config.h nsga_binary.cc  learning.o
	$(CC) $(CFLAGS) $(OPTION) nsga_binary.cc $(OPT) $(OUTPUT_OPTION) nsga_binary.o

pso_selection.o : nsga_config.h pso_selection.cc domain_judge.o
	$(CC) $(CFLAGS) $(OPTION) pso_selection.cc $(OPT) $(OUTPUT_OPTION) pso_selection.o

spea_selection.o : nsga_config.h spea_selection.cc domain_judge.o
	$(CC) $(CFLAGS) $(OPTION) spea_selection.cc $(OPT) $(OUTPUT_OPTION) spea_selection.o

pso_crossover_nor.o : nsga_config.h pso_crossover_nor.cc domain_judge.o
	$(CC) $(CFLAGS) $(OPTION) pso_crossover_nor.cc $(OPT) $(OUTPUT_OPTION) pso_crossover_nor.o 

pso.o : pso.h nsga_config.h pso.cc 
	$(CC) $(CFLAGS) $(OPTION) pso.cc $(OPT) $(OUTPUT_OPTION) pso.o

pso_binary.o : pso.h nsga_config.h pso_binary.cc 
	$(CC) $(CFLAGS) $(OPTION) pso_binary.cc $(OPT) $(OUTPUT_OPTION) pso_binary.o

taea_selection.o : nsga_config.h taea_selection.cc domain_judge.o
	$(CC) $(CFLAGS) $(OPTION) taea_selection.cc $(OPT) $(OUTPUT_OPTION) taea_selection.o

taea.o : taea.h nsga_config.h taea.cc 
	$(CC) $(CFLAGS) $(OPTION) taea.cc $(OPT) $(OUTPUT_OPTION) taea.o

taea_binary.o : taea.h nsga_config.h taea_binary.cc 
	$(CC) $(CFLAGS) $(OPTION) taea_binary.cc $(OPT) $(OUTPUT_OPTION) taea_binary.o

spea.o : spea.h nsga_config.h spea.cc 
	$(CC) $(CFLAGS) $(OPTION) spea.cc $(OPT) $(OUTPUT_OPTION) spea.o

spea_binary.o : spea_binary.h nsga_config.h spea_binary.cc 
	$(CC) $(CFLAGS) $(OPTION) spea_binary.cc $(OPT) $(OUTPUT_OPTION) spea_binary.o

paes.o : paes.h nsga_config.h paes.cc 
	$(CC) $(CFLAGS) $(OPTION) paes.cc $(OPT) $(OUTPUT_OPTION) paes.o

paes_binary.o : paes.h nsga_config.h paes_binary.cc 
	$(CC) $(CFLAGS) $(OPTION) paes_binary.cc $(OPT) $(OUTPUT_OPTION) paes_binary.o

greedy_binary.o : greedy_binary.h nsga_config.h greedy_binary.cc 
	$(CC) $(CFLAGS) $(OPTION) greedy_binary.cc $(OPT) $(OUTPUT_OPTION) greedy_binary.o

greedy.o : greedy.h nsga_config.h greedy.cc 
	$(CC) $(CFLAGS) $(OPTION) greedy.cc $(OPT) $(OUTPUT_OPTION) greedy.o

main_serial.o : nsga_config.h nsga_config.cc main_c.cc
	$(CC) $(CFLAGS) $(OPTION) main_c.cc $(OPT) $(OUTPUT_OPTION) main_serial.o

.PHONY:clean
clean:
	$(RM) $(OBJ) $(EXE)

CPUC      = gcc
CFLAGS    = -O3	 #    --std=c11 -m64 -w -Wunused-variable #-Wno-conversion-null -Wdelete-non-virtual-dtor
DEFS      =
INCLUDES  =
#LIBDIR   = -L/usr/lib
#LIBS     = -lm
#LINK     =  $(LIBDIR) $(LIBS)
CPU_COMPILE_C  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)
BIN_DIR     = bin
SCRIPTS_DIR = scripts
#SRC_DIR     = src
LIB_DIR		= lib
SRCS_ALL = $(wildcard $(LIB_DIR)/*.c)

all:
	@echo "run \"make compile\" to compile the program for both sequential and parallel codes"
	@echo "run \"make run_omp\" to run the parallel program with some_number iterations"
	@echo "run \"make run_seq\" to run the sequential program with some_number iterations"

compile:
	@echo "compiling..."
	@$(CPU_COMPILE_C) -fopenmp main.c $(SRCS_ALL) -o $(BIN_DIR)/brt_omp
	@$(CPU_COMPILE_C) main.c $(SRCS_ALL) -o $(BIN_DIR)/brt_seq
	
clean:
	@echo "removing executable programs:"
	rm $(BIN_DIR)/brt_omp
	rm $(BIN_DIR)/brt_seq

run_omp:
	@$(SCRIPTS_DIR)/exec_omp.sh > output_omp.txt

run_seq:
	@$(SCRIPTS_DIR)/exec_seq.sh > output_seq.txt

run_both: run_seq run_omp

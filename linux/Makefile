# Template makefile for prototyping with the Linux MMR libs implementation

MMR_CORE_DIR = ../..

INC = $\
 -I $(MMR_CORE_DIR)/app/lib/can/inc $\
 -I $(MMR_CORE_DIR)/app/lib/timing/inc $\
 -I $(MMR_CORE_DIR)/app/lib/util/inc

LIB = $\
 $(MMR_CORE_DIR)/app/linux/mmr_socketcan.c $\
 $(MMR_CORE_DIR)/app/lib/can/can.c $\
 $(MMR_CORE_DIR)/app/lib/timing/timing.c $\
 $(MMR_CORE_DIR)/app/linux/timing.c $\
 $(MMR_CORE_DIR)/app/lib/timing/delay.c

OUT = test

test: $(LIB) test.c
	$(CC) test.c $(LIB) $(INC) -o test
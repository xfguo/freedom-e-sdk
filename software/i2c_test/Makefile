TARGET = i2c_test
CFLAGS += -O2 -fno-builtin-printf -fmax-errors=1 # -DUSE_PLIC -DUSE_M_TIME

BSP_BASE = ../../bsp
BASE = ../..

C_SRCS += i2c_test.c
C_SRCS += $(BSP_BASE)/drivers/i2c/i2c-ocores.c

INCLUDES += -H -I$(BASE)/riscv-gnu-toolchain/linux-headers/include 

include $(BSP_BASE)/env/common.mk

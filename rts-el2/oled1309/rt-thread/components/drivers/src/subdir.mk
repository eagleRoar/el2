################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/drivers/src/completion.c \
../rt-thread/components/drivers/src/dataqueue.c \
../rt-thread/components/drivers/src/pipe.c \
../rt-thread/components/drivers/src/ringblk_buf.c \
../rt-thread/components/drivers/src/ringbuffer.c \
../rt-thread/components/drivers/src/waitqueue.c \
../rt-thread/components/drivers/src/workqueue.c 

OBJS += \
./rt-thread/components/drivers/src/completion.o \
./rt-thread/components/drivers/src/dataqueue.o \
./rt-thread/components/drivers/src/pipe.o \
./rt-thread/components/drivers/src/ringblk_buf.o \
./rt-thread/components/drivers/src/ringbuffer.o \
./rt-thread/components/drivers/src/waitqueue.o \
./rt-thread/components/drivers/src/workqueue.o 

C_DEPS += \
./rt-thread/components/drivers/src/completion.d \
./rt-thread/components/drivers/src/dataqueue.d \
./rt-thread/components/drivers/src/pipe.d \
./rt-thread/components/drivers/src/ringblk_buf.d \
./rt-thread/components/drivers/src/ringbuffer.d \
./rt-thread/components/drivers/src/waitqueue.d \
./rt-thread/components/drivers/src/workqueue.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/drivers/src/%.o: ../rt-thread/components/drivers/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\4.Workspace\rts-el2\drivers" -I"D:\4.Workspace\rts-el2\applications\oled_1602" -I"D:\4.Workspace\rts-el2\applications\light" -I"D:\4.Workspace\rts-el2\applications\Includes" -I"D:\4.Workspace\rts-el2\drivers\include" -I"D:\4.Workspace\rts-el2\drivers\include\config" -I"D:\4.Workspace\rts-el2\libraries" -I"D:\4.Workspace\rts-el2\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\4.Workspace\rts-el2\libraries\CMSIS\Include" -I"D:\4.Workspace\rts-el2\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\4.Workspace\rts-el2" -I"D:\4.Workspace\rts-el2\packages\LedBlink-latest" -I"D:\4.Workspace\rts-el2\packages\agile_button-latest\inc" -I"D:\4.Workspace\rts-el2\packages\u8g2-c-latest\port" -I"D:\4.Workspace\rts-el2\packages\u8g2-c-latest" -I"D:\4.Workspace\rts-el2\rt-thread\components\dfs\filesystems\devfs" -I"D:\4.Workspace\rts-el2\rt-thread\components\dfs\include" -I"D:\4.Workspace\rts-el2\rt-thread\components\drivers\include" -I"D:\4.Workspace\rts-el2\rt-thread\components\drivers\include\drivers" -I"D:\4.Workspace\rts-el2\rt-thread\components\libc\compilers\minilibc" -I"D:\4.Workspace\rts-el2\rt-thread\include" -I"D:\4.Workspace\rts-el2\rt-thread\libcpu\arm\common" -I"D:\4.Workspace\rts-el2\rt-thread\libcpu\arm\cortex-m3" -include"D:\4.Workspace\rts-el2\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

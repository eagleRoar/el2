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
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F1 -DUSE_HAL_DRIVER -DSTM32F103xE -I"D:\3.Workspace\el2-release\applications\Includes" -I"D:\3.Workspace\el2-release\applications\light" -I"D:\3.Workspace\el2-release\applications\oled_1602" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\drivers" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include\drivers" -I"D:\3.Workspace\el2-release\drivers\include" -I"D:\3.Workspace\el2-release\drivers\include\config" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\RTOS\Template" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc\Legacy" -I"D:\3.Workspace\el2-release\applications" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\packages\LedBlink-latest" -I"D:\3.Workspace\el2-release\packages\agile_button-latest\inc" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest\port" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include" -I"D:\3.Workspace\el2-release\rt-thread\components\libc\compilers\minilibc" -I"D:\3.Workspace\el2-release\rt-thread\include" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\common" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\cortex-m3" -include"D:\3.Workspace\el2-release\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


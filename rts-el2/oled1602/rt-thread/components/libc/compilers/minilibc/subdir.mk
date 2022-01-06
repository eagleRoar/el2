################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/libc/compilers/minilibc/ctype.c \
../rt-thread/components/libc/compilers/minilibc/math.c \
../rt-thread/components/libc/compilers/minilibc/qsort.c \
../rt-thread/components/libc/compilers/minilibc/rand.c \
../rt-thread/components/libc/compilers/minilibc/stdlib.c \
../rt-thread/components/libc/compilers/minilibc/string.c \
../rt-thread/components/libc/compilers/minilibc/time.c 

OBJS += \
./rt-thread/components/libc/compilers/minilibc/ctype.o \
./rt-thread/components/libc/compilers/minilibc/math.o \
./rt-thread/components/libc/compilers/minilibc/qsort.o \
./rt-thread/components/libc/compilers/minilibc/rand.o \
./rt-thread/components/libc/compilers/minilibc/stdlib.o \
./rt-thread/components/libc/compilers/minilibc/string.o \
./rt-thread/components/libc/compilers/minilibc/time.o 

C_DEPS += \
./rt-thread/components/libc/compilers/minilibc/ctype.d \
./rt-thread/components/libc/compilers/minilibc/math.d \
./rt-thread/components/libc/compilers/minilibc/qsort.d \
./rt-thread/components/libc/compilers/minilibc/rand.d \
./rt-thread/components/libc/compilers/minilibc/stdlib.d \
./rt-thread/components/libc/compilers/minilibc/string.d \
./rt-thread/components/libc/compilers/minilibc/time.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/libc/compilers/minilibc/%.o: ../rt-thread/components/libc/compilers/minilibc/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F1 -DUSE_HAL_DRIVER -DSTM32F103xE -I"D:\3.Workspace\el2-release\applications\Includes" -I"D:\3.Workspace\el2-release\applications\light" -I"D:\3.Workspace\el2-release\applications\oled_1602" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\drivers" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include\drivers" -I"D:\3.Workspace\el2-release\drivers\include" -I"D:\3.Workspace\el2-release\drivers\include\config" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\RTOS\Template" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc\Legacy" -I"D:\3.Workspace\el2-release\applications" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\packages\LedBlink-latest" -I"D:\3.Workspace\el2-release\packages\agile_button-latest\inc" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest\port" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include" -I"D:\3.Workspace\el2-release\rt-thread\components\libc\compilers\minilibc" -I"D:\3.Workspace\el2-release\rt-thread\include" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\common" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\cortex-m3" -include"D:\3.Workspace\el2-release\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


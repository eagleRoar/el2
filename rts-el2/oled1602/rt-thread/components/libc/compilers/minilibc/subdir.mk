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
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\4.Workspace\rts-el2\drivers" -I"D:\BBL_location\gitRepos\rts-el2\applications\light" -I"C:\RT-ThreadStudio\workspace\rts-el2\applications\light" -I"G:\rt-thread\EL2\PRO\rts-el2\applications\light" -I"D:\4.Workspace\rts-el2\applications\light" -I"D:\4.Workspace\rts-el2\rt-thread\components\drivers\include\drivers" -I"D:\4.Workspace\rts-el2\applications\Includes" -I"D:\4.Workspace\rts-el2\applications\oled_1602" -I"D:\4.Workspace\rts-el2\drivers\include" -I"D:\4.Workspace\rts-el2\drivers\include\config" -I"D:\4.Workspace\rts-el2\libraries" -I"D:\4.Workspace\rts-el2\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\4.Workspace\rts-el2\libraries\CMSIS\Include" -I"D:\4.Workspace\rts-el2\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\4.Workspace\rts-el2" -I"D:\4.Workspace\rts-el2\packages\LedBlink-latest" -I"D:\4.Workspace\rts-el2\packages\agile_button-latest\inc" -I"D:\4.Workspace\rts-el2\packages\u8g2-c-latest\port" -I"D:\4.Workspace\rts-el2\packages\u8g2-c-latest" -I"D:\4.Workspace\rts-el2\rt-thread\components\dfs\filesystems\devfs" -I"D:\4.Workspace\rts-el2\rt-thread\components\dfs\include" -I"D:\4.Workspace\rts-el2\rt-thread\components\drivers\include" -I"D:\4.Workspace\rts-el2\rt-thread\components\libc\compilers\minilibc" -I"D:\4.Workspace\rts-el2\rt-thread\include" -I"D:\4.Workspace\rts-el2\rt-thread\libcpu\arm\common" -I"D:\4.Workspace\rts-el2\rt-thread\libcpu\arm\cortex-m3" -include"D:\4.Workspace\rts-el2\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


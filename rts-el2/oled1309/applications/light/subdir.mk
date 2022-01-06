################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applications/light/uuzLIGHT.c 

OBJS += \
./applications/light/uuzLIGHT.o 

C_DEPS += \
./applications/light/uuzLIGHT.d 


# Each subdirectory must supply rules for building sources it contributes
applications/light/%.o: ../applications/light/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F1 -DUSE_HAL_DRIVER -DSTM32F103xE -I"D:\workspace\rts-el2-103\applications\Includes" -I"D:\workspace\rts-el2-103\packages\u8g2-cpp-latest\src" -I"D:\workspace\rts-el2-103\applications\light" -I"D:\workspace\rts-el2-103\applications\oled_1602" -I"D:\workspace\rts-el2-103" -I"D:\workspace\rts-el2-103\drivers" -I"D:\workspace\rts-el2-103\rt-thread\components\drivers\include\drivers" -I"D:\workspace\rts-el2-103\drivers\include" -I"D:\workspace\rts-el2-103\drivers\include\config" -I"D:\workspace\rts-el2-103\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\workspace\rts-el2-103\libraries\CMSIS\Include" -I"D:\workspace\rts-el2-103\libraries\CMSIS\RTOS\Template" -I"D:\workspace\rts-el2-103\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\workspace\rts-el2-103\libraries\STM32F1xx_HAL_Driver\Inc\Legacy" -I"D:\workspace\rts-el2-103\applications" -I"D:\workspace\rts-el2-103" -I"D:\workspace\rts-el2-103\packages\LedBlink-latest" -I"D:\workspace\rts-el2-103\packages\agile_button-latest\inc" -I"D:\workspace\rts-el2-103\packages\u8g2-cpp-latest\port" -I"D:\workspace\rts-el2-103\packages\u8g2-cpp-latest" -I"D:\workspace\rts-el2-103\rt-thread\components\drivers\include" -I"D:\workspace\rts-el2-103\rt-thread\components\libc\compilers\minilibc" -I"D:\workspace\rts-el2-103\rt-thread\include" -I"D:\workspace\rts-el2-103\rt-thread\libcpu\arm\common" -I"D:\workspace\rts-el2-103\rt-thread\libcpu\arm\cortex-m3" -include"D:\workspace\rts-el2-103\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


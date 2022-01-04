################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/u8g2-c-latest/port/u8g2_port.c 

OBJS += \
./packages/u8g2-c-latest/port/u8g2_port.o 

C_DEPS += \
./packages/u8g2-c-latest/port/u8g2_port.d 


# Each subdirectory must supply rules for building sources it contributes
packages/u8g2-c-latest/port/%.o: ../packages/u8g2-c-latest/port/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"G:\rt-thread\EL2\PRO\rts-el2\drivers" -I"G:\rt-thread\EL2\PRO\rts-el2\applications\Includes" -I"G:\rt-thread\EL2\PRO\rts-el2\drivers\include" -I"G:\rt-thread\EL2\PRO\rts-el2\drivers\include\config" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries\CMSIS\Include" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries\STM32F1xx_HAL_Driver\Inc" -I"G:\rt-thread\EL2\PRO\rts-el2" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\LedBlink-latest" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\agile_button-latest\inc" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\u8g2-c-latest\port" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\u8g2-c-latest" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\dfs\filesystems\devfs" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\dfs\include" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\drivers\include" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\libc\compilers\minilibc" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\include" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\libcpu\arm\common" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\libcpu\arm\cortex-m3" -include"G:\rt-thread\EL2\PRO\rts-el2\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


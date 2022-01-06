################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applications/oled_1309/uuzOLED_1309.c \
../applications/oled_1309/uuzUI_1309.c \
../applications/oled_1309/uuzUI_Cache.c \
../applications/oled_1309/uuzUI_date.c \
../applications/oled_1309/uuzUI_light.c \
../applications/oled_1309/uuzUI_main.c \
../applications/oled_1309/uuzUI_menu.c \
../applications/oled_1309/uuzUI_temp.c 

OBJS += \
./applications/oled_1309/uuzOLED_1309.o \
./applications/oled_1309/uuzUI_1309.o \
./applications/oled_1309/uuzUI_Cache.o \
./applications/oled_1309/uuzUI_date.o \
./applications/oled_1309/uuzUI_light.o \
./applications/oled_1309/uuzUI_main.o \
./applications/oled_1309/uuzUI_menu.o \
./applications/oled_1309/uuzUI_temp.o 

C_DEPS += \
./applications/oled_1309/uuzOLED_1309.d \
./applications/oled_1309/uuzUI_1309.d \
./applications/oled_1309/uuzUI_Cache.d \
./applications/oled_1309/uuzUI_date.d \
./applications/oled_1309/uuzUI_light.d \
./applications/oled_1309/uuzUI_main.d \
./applications/oled_1309/uuzUI_menu.d \
./applications/oled_1309/uuzUI_temp.d 


# Each subdirectory must supply rules for building sources it contributes
applications/oled_1309/%.o: ../applications/oled_1309/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F1 -DUSE_HAL_DRIVER -DSTM32F103xE -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\applications\Includes" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\packages\u8g2-c-latest\src" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\applications\light" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\applications\oled_1309" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\drivers" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rt-thread\components\drivers\include\drivers" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\drivers\include" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\drivers\include\config" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\libraries\CMSIS\Include" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\libraries\CMSIS\RTOS\Template" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\libraries\STM32F1xx_HAL_Driver\Inc\Legacy" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\applications" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\packages\LedBlink-latest" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\packages\agile_button-latest\inc" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\packages\u8g2-c-latest\port" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\packages\u8g2-c-latest" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rt-thread\components\drivers\include" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rt-thread\components\libc\compilers\minilibc" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rt-thread\include" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rt-thread\libcpu\arm\common" -I"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rt-thread\libcpu\arm\cortex-m3" -include"D:\SynologyDrive\A26-EL2-PRO\PRO\rts-el2-103\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


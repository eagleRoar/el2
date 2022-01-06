################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applications/oled_1602/uuzOLED_1602.c \
../applications/oled_1602/uuzUI_1602.c \
../applications/oled_1602/uuzUI_Cache.c \
../applications/oled_1602/uuzUI_date.c \
../applications/oled_1602/uuzUI_light.c \
../applications/oled_1602/uuzUI_main.c \
../applications/oled_1602/uuzUI_menu.c \
../applications/oled_1602/uuzUI_temp.c 

OBJS += \
./applications/oled_1602/uuzOLED_1602.o \
./applications/oled_1602/uuzUI_1602.o \
./applications/oled_1602/uuzUI_Cache.o \
./applications/oled_1602/uuzUI_date.o \
./applications/oled_1602/uuzUI_light.o \
./applications/oled_1602/uuzUI_main.o \
./applications/oled_1602/uuzUI_menu.o \
./applications/oled_1602/uuzUI_temp.o 

C_DEPS += \
./applications/oled_1602/uuzOLED_1602.d \
./applications/oled_1602/uuzUI_1602.d \
./applications/oled_1602/uuzUI_Cache.d \
./applications/oled_1602/uuzUI_date.d \
./applications/oled_1602/uuzUI_light.d \
./applications/oled_1602/uuzUI_main.d \
./applications/oled_1602/uuzUI_menu.d \
./applications/oled_1602/uuzUI_temp.d 


# Each subdirectory must supply rules for building sources it contributes
applications/oled_1602/%.o: ../applications/oled_1602/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F1 -DUSE_HAL_DRIVER -DSTM32F103xE -I"D:\3.Workspace\el2-release\applications\Includes" -I"D:\3.Workspace\el2-release\applications\light" -I"D:\3.Workspace\el2-release\applications\oled_1602" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\drivers" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include\drivers" -I"D:\3.Workspace\el2-release\drivers\include" -I"D:\3.Workspace\el2-release\drivers\include\config" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\RTOS\Template" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc\Legacy" -I"D:\3.Workspace\el2-release\applications" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\packages\LedBlink-latest" -I"D:\3.Workspace\el2-release\packages\agile_button-latest\inc" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest\port" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include" -I"D:\3.Workspace\el2-release\rt-thread\components\libc\compilers\minilibc" -I"D:\3.Workspace\el2-release\rt-thread\include" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\common" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\cortex-m3" -include"D:\3.Workspace\el2-release\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


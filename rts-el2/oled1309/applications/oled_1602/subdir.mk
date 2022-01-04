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
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\4.Workspace\rts-el2\drivers" -I"D:\4.Workspace\rts-el2\applications\oled_1602" -I"D:\4.Workspace\rts-el2\applications\light" -I"D:\4.Workspace\rts-el2\applications\Includes" -I"D:\4.Workspace\rts-el2\drivers\include" -I"D:\4.Workspace\rts-el2\drivers\include\config" -I"D:\4.Workspace\rts-el2\libraries" -I"D:\4.Workspace\rts-el2\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\4.Workspace\rts-el2\libraries\CMSIS\Include" -I"D:\4.Workspace\rts-el2\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\4.Workspace\rts-el2" -I"D:\4.Workspace\rts-el2\packages\LedBlink-latest" -I"D:\4.Workspace\rts-el2\packages\agile_button-latest\inc" -I"D:\4.Workspace\rts-el2\packages\u8g2-c-latest\port" -I"D:\4.Workspace\rts-el2\packages\u8g2-c-latest" -I"D:\4.Workspace\rts-el2\rt-thread\components\dfs\filesystems\devfs" -I"D:\4.Workspace\rts-el2\rt-thread\components\dfs\include" -I"D:\4.Workspace\rts-el2\rt-thread\components\drivers\include" -I"D:\4.Workspace\rts-el2\rt-thread\components\drivers\include\drivers" -I"D:\4.Workspace\rts-el2\rt-thread\components\libc\compilers\minilibc" -I"D:\4.Workspace\rts-el2\rt-thread\include" -I"D:\4.Workspace\rts-el2\rt-thread\libcpu\arm\common" -I"D:\4.Workspace\rts-el2\rt-thread\libcpu\arm\cortex-m3" -include"D:\4.Workspace\rts-el2\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


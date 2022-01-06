################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applications/main.c \
../applications/uuzADC.c \
../applications/uuzBTN.c \
../applications/uuzCalendar.c \
../applications/uuzDAC.c \
../applications/uuzDevice.c \
../applications/uuzGPIO.c \
../applications/uuzINIT.c \
../applications/uuzLED.c \
../applications/uuzOpt.c \
../applications/uuzPVD.c \
../applications/uuzRTC.c \
../applications/uuzSENSOR.c \
../applications/uuzTEMP.c \
../applications/uuzUI.c \
../applications/uuzUart.c \
../applications/uuzUartEvent.c 

OBJS += \
./applications/main.o \
./applications/uuzADC.o \
./applications/uuzBTN.o \
./applications/uuzCalendar.o \
./applications/uuzDAC.o \
./applications/uuzDevice.o \
./applications/uuzGPIO.o \
./applications/uuzINIT.o \
./applications/uuzLED.o \
./applications/uuzOpt.o \
./applications/uuzPVD.o \
./applications/uuzRTC.o \
./applications/uuzSENSOR.o \
./applications/uuzTEMP.o \
./applications/uuzUI.o \
./applications/uuzUart.o \
./applications/uuzUartEvent.o 

C_DEPS += \
./applications/main.d \
./applications/uuzADC.d \
./applications/uuzBTN.d \
./applications/uuzCalendar.d \
./applications/uuzDAC.d \
./applications/uuzDevice.d \
./applications/uuzGPIO.d \
./applications/uuzINIT.d \
./applications/uuzLED.d \
./applications/uuzOpt.d \
./applications/uuzPVD.d \
./applications/uuzRTC.d \
./applications/uuzSENSOR.d \
./applications/uuzTEMP.d \
./applications/uuzUI.d \
./applications/uuzUart.d \
./applications/uuzUartEvent.d 


# Each subdirectory must supply rules for building sources it contributes
applications/%.o: ../applications/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DSOC_FAMILY_STM32 -DSOC_SERIES_STM32F1 -DUSE_HAL_DRIVER -DSTM32F103xE -I"D:\3.Workspace\el2-release\applications\Includes" -I"D:\3.Workspace\el2-release\applications\light" -I"D:\3.Workspace\el2-release\applications\oled_1602" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\drivers" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include\drivers" -I"D:\3.Workspace\el2-release\drivers\include" -I"D:\3.Workspace\el2-release\drivers\include\config" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\Include" -I"D:\3.Workspace\el2-release\libraries\CMSIS\RTOS\Template" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc" -I"D:\3.Workspace\el2-release\libraries\STM32F1xx_HAL_Driver\Inc\Legacy" -I"D:\3.Workspace\el2-release\applications" -I"D:\3.Workspace\el2-release" -I"D:\3.Workspace\el2-release\packages\LedBlink-latest" -I"D:\3.Workspace\el2-release\packages\agile_button-latest\inc" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest\port" -I"D:\3.Workspace\el2-release\packages\u8g2-c-latest" -I"D:\3.Workspace\el2-release\rt-thread\components\drivers\include" -I"D:\3.Workspace\el2-release\rt-thread\components\libc\compilers\minilibc" -I"D:\3.Workspace\el2-release\rt-thread\include" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\common" -I"D:\3.Workspace\el2-release\rt-thread\libcpu\arm\cortex-m3" -include"D:\3.Workspace\el2-release\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


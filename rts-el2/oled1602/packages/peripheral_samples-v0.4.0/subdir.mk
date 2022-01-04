################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/peripheral_samples-v0.4.0/adc_vol_sample.c \
../packages/peripheral_samples-v0.4.0/hwtimer_sample.c \
../packages/peripheral_samples-v0.4.0/i2c_aht10_sample.c \
../packages/peripheral_samples-v0.4.0/iwdg_sample.c \
../packages/peripheral_samples-v0.4.0/led_blink_sample.c \
../packages/peripheral_samples-v0.4.0/pin_beep_sample.c \
../packages/peripheral_samples-v0.4.0/pwm_led_sample.c \
../packages/peripheral_samples-v0.4.0/rtc_sample.c \
../packages/peripheral_samples-v0.4.0/sd_sample.c \
../packages/peripheral_samples-v0.4.0/spi_w25q_sample.c \
../packages/peripheral_samples-v0.4.0/uart_dma_sample.c \
../packages/peripheral_samples-v0.4.0/uart_sample.c 

OBJS += \
./packages/peripheral_samples-v0.4.0/adc_vol_sample.o \
./packages/peripheral_samples-v0.4.0/hwtimer_sample.o \
./packages/peripheral_samples-v0.4.0/i2c_aht10_sample.o \
./packages/peripheral_samples-v0.4.0/iwdg_sample.o \
./packages/peripheral_samples-v0.4.0/led_blink_sample.o \
./packages/peripheral_samples-v0.4.0/pin_beep_sample.o \
./packages/peripheral_samples-v0.4.0/pwm_led_sample.o \
./packages/peripheral_samples-v0.4.0/rtc_sample.o \
./packages/peripheral_samples-v0.4.0/sd_sample.o \
./packages/peripheral_samples-v0.4.0/spi_w25q_sample.o \
./packages/peripheral_samples-v0.4.0/uart_dma_sample.o \
./packages/peripheral_samples-v0.4.0/uart_sample.o 

C_DEPS += \
./packages/peripheral_samples-v0.4.0/adc_vol_sample.d \
./packages/peripheral_samples-v0.4.0/hwtimer_sample.d \
./packages/peripheral_samples-v0.4.0/i2c_aht10_sample.d \
./packages/peripheral_samples-v0.4.0/iwdg_sample.d \
./packages/peripheral_samples-v0.4.0/led_blink_sample.d \
./packages/peripheral_samples-v0.4.0/pin_beep_sample.d \
./packages/peripheral_samples-v0.4.0/pwm_led_sample.d \
./packages/peripheral_samples-v0.4.0/rtc_sample.d \
./packages/peripheral_samples-v0.4.0/sd_sample.d \
./packages/peripheral_samples-v0.4.0/spi_w25q_sample.d \
./packages/peripheral_samples-v0.4.0/uart_dma_sample.d \
./packages/peripheral_samples-v0.4.0/uart_sample.d 


# Each subdirectory must supply rules for building sources it contributes
packages/peripheral_samples-v0.4.0/%.o: ../packages/peripheral_samples-v0.4.0/%.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"G:\rt-thread\EL2\PRO\rts-el2\drivers" -I"C:\RT-ThreadStudio\workspace\rts-el2\applications\light" -I"G:\rt-thread\EL2\PRO\rts-el2\applications\light" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\drivers\include\drivers" -I"G:\rt-thread\EL2\PRO\rts-el2\applications\Includes" -I"G:\rt-thread\EL2\PRO\rts-el2\applications\oled_1602" -I"G:\rt-thread\EL2\PRO\rts-el2\drivers\include" -I"G:\rt-thread\EL2\PRO\rts-el2\drivers\include\config" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries\CMSIS\Device\ST\STM32F1xx\Include" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries\CMSIS\Include" -I"G:\rt-thread\EL2\PRO\rts-el2\libraries\STM32F1xx_HAL_Driver\Inc" -I"G:\rt-thread\EL2\PRO\rts-el2" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\LedBlink-latest" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\agile_button-latest\inc" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\u8g2-c-latest\port" -I"G:\rt-thread\EL2\PRO\rts-el2\packages\u8g2-c-latest" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\dfs\filesystems\devfs" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\dfs\include" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\drivers\include" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\components\libc\compilers\minilibc" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\include" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\libcpu\arm\common" -I"G:\rt-thread\EL2\PRO\rts-el2\rt-thread\libcpu\arm\cortex-m3" -include"G:\rt-thread\EL2\PRO\rts-el2\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"


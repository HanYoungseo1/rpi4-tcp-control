//temperature_control.c

#include <stdio.h>
#include <softPwm.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define TEMP_CHANNEL 1
#define LED_GPIO 17

void temperature_control(int temperature)
{
    pinMode(LED_GPIO, OUTPUT);
    softPwmCreate(LED_GPIO, 0, 100);

    int fd = wiringPiI2CSetupInterface("/dev/i2c-1", 0x48);
    if (fd < 0)
    {
        perror("wiringPiI2CSetupInterface");
        return;
    }

    while (1)
    {
        wiringPiI2CWrite(fd, 0x00 | TEMP_CHANNEL); 
        delay(1);
        int value = wiringPiI2CRead(fd); 
        if (value < 0)
            break;

        printf("현재 ADC 값 : %d\n", value);

        if (value < temperature)
        {
            softPwmWrite(LED_GPIO, 100);
            printf("High Temperature (LED ON)\n");
        }
        else
        {
            softPwmWrite(LED_GPIO, 0);
            printf("Temperature OK (LED OFF)\n");
        }
        delay(500);
    }
}
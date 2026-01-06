//led_control.c

#include <string.h>
#include <softPwm.h>
#include <wiringPi.h>

#define LED_GPIO 17

void led_control(char* buf)
{
        pinMode(LED_GPIO, OUTPUT);
        softPwmCreate(LED_GPIO, 0, 100);
        if (strstr(buf, "on"))
                softPwmWrite(LED_GPIO, 100);
        else if (strstr(buf, "off"))
                softPwmWrite(LED_GPIO, 0);
        else if (strstr(buf, "high"))
                softPwmWrite(LED_GPIO, 100);
        else if (strstr(buf, "mid"))
                softPwmWrite(LED_GPIO, 50);
        else if (strstr(buf, "low"))
                softPwmWrite(LED_GPIO, 15);
}

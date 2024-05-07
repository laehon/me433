#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/pwm.h"

#define MOTORPIN 16

void set_angle(float angle) {
    float input = (angle / 180.0 * 0.10) + 0.0225;
    pwm_set_gpio_level(MOTORPIN, 62500 * input);
}

void pwm_16_init() {
    gpio_set_function(MOTORPIN, GPIO_FUNC_PWM); // Set the LED Pin to be PWM

    uint slice_num = pwm_gpio_to_slice_num(MOTORPIN); // Get PWM slice number

    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider

    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true); // turn on the PWM
}

int main() {

    stdio_init_all();
    pwm_16_init();

    set_angle(180.0);
    
    while (true) {
        for (int i = 0; i < 100; i++) {
            float angle = (float) i / 99.0 * 180.0;
            set_angle(angle);
            sleep_ms(20);
        }
        for (int i = 99; i >= 0; i--) {
            float angle = (float) i / 99.0 * 180.0;
            set_angle(angle);
            sleep_ms(20);
        }
    }
}

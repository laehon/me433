#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define LED_PIN 16
#define BUTTON_PIN 17

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Start!\n");

    adc_init(); 
    adc_gpio_init(26);
    adc_select_input(0);

    gpio_init(LED_PIN);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    
    while (true) {

        gpio_put(LED_PIN, 1);

        while (gpio_get(BUTTON_PIN) == 0) {}

        gpio_put(LED_PIN, 0);

        unsigned int n;
        scanf("%d", &n);

        uint16_t val;
        double voltage;

        for (int i = 0; i < n; i++) {
            val = adc_read();
            voltage = val*3.3/4095;
            printf("voltage: %f\n\r", voltage);
        }
    }
}

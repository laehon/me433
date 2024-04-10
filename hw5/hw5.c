#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#define ADDR _u(0b0100111)

#define REG_IODIR _u(0x00)
#define REG_GPIO _u(0x09)
#define REG_OLAT _u(0x0A)

#define LED_PIN 25

void chip_init() {
    uint8_t buf[2];

    buf[0] = REG_IODIR;
    buf[1] = 0b01111111;

    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

void set(char v) {
    uint8_t buf[2];

    buf[0] = REG_OLAT;
    buf[1] = v << 7;

    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

char read() {
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;

    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);

    if ((buf[0] & 0b1) == 0b1) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    stdio_init_all();

    i2c_init(i2c_default, 5000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    chip_init();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (true) {
        if (read() == 1) {
            set(1);
        } else {
            set(0);
        }
        gpio_put(LED_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_PIN, 0);
        sleep_ms(100);
    }
}

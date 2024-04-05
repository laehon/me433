#include <math.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

static void write_register(uint8_t channel, uint16_t val) {
    if (val > 1023)
        val = 1023; 
    uint8_t buf[2];
    buf[1] = (val << 2); 
    buf[0] = (channel << 7) | (0b111 << 4) | (val >> 6);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
    spi_write_blocking(spi_default, buf, 2);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    sleep_ms(1);
}

int main() {
    
    stdio_init_all();

    spi_init(spi_default, 10000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);

    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    
    
    uint16_t val = 0;
    double tri_val = 0;
    double tri_increment = 3.3/(500/2);
    
    double sin_val = 0;    

    while (true) {
        val = tri_val / 3.3 * 1024;
        write_register(0, val);
        tri_val += tri_increment;
        if (tri_val < 0)
            tri_val = 0;
        if (tri_val >= 3.3)
            tri_increment = -tri_increment;
        if (tri_val <= 0)
            tri_increment = -tri_increment;
        
        val = 512 + (512 * sin(sin_val));
        write_register(1, val);
        sin_val = sin_val + 3.1415/(500/4);
    }
}

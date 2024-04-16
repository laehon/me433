#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"

#define LED_PIN 25

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;
volatile int i = 0;
volatile char m[100];

void drawChar(int x, int y, char letter) {
    for(int i = 0; i < 5; i ++) {
        char c = ASCII[letter-32][i];
        
        for(int j = 0; j < 8; j++) {
            char bit  = (c >> j) & 0b1;

            if (bit == 0b1) {
                ssd1306_drawPixel(x+i, y+j, 1);
            } else {
                ssd1306_drawPixel(x+i, y+j, 0);
            }
        }
    }
}

void drawMessage(int x, int y, char* message) {
    int i = 0;
    while(message[i]) {
        drawChar(x+5*i, y, message[i]);
        i++;
    }
    ssd1306_update();
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if (ch == '\r') {
            ssd1306_clear();
            drawMessage(0, 0, m);
            i = 0;
        } else {
            m[i] = ch;
            i++;
        }
        chars_rxed++;
    }
}

int main() {

    stdio_init_all();

    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);

    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    i2c_init(i2c_default, 400*1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

    ssd1306_setup();
    sleep_ms(100);

    uart_puts(UART_ID, "\nHello, uart interrupts\n");

    i = 0;

    while (1) {
        gpio_put(LED_PIN, 1);
        sleep_ms(25);
        gpio_put(LED_PIN, 0);
        sleep_ms(25);
    }
}


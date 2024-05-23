#include <stdio.h>
#include <string.h>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define EN_PIN 17

#define MOTORPIN 16

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

void set_motor_speed(int i) {
    if (i > 25) {
        pwm_set_gpio_level(MOTORPIN,  0);
    } else if (i < 0) {
        pwm_set_gpio_level(MOTORPIN,  62500);
    } else {
        pwm_set_gpio_level(MOTORPIN,  62500* (25 - i) / 25);
    }
}

void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        if (ch == '\r') {
            uart_puts(UART_ID, m);
            m[i] = '\0';
            int speed = 0;
            sscanf(m, "%d", &speed);
            set_motor_speed(speed);
            i = 0;
            m[0] = 0;
        } else {
            m[i] = ch;
            i++;
        }
        chars_rxed++;
    }
}

void pwm_pin_init() {
    gpio_set_function(MOTORPIN, GPIO_FUNC_PWM); // Set the LED Pin to be PWM

    uint slice_num = pwm_gpio_to_slice_num(MOTORPIN); // Get PWM slice number

    float div = 40; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // divider

    uint16_t wrap = 62500; // when to rollover, must be less than 65535
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true); // turn on the PWM
    pwm_set_gpio_level(MOTORPIN,  62500);
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

    gpio_init(EN_PIN);
    gpio_set_dir(EN_PIN, GPIO_OUT);

    pwm_pin_init();

    uart_puts(UART_ID, "\nHello, uart interrupts\n");

    gpio_put(EN_PIN, 1);

    while (1) {
        
    }
}


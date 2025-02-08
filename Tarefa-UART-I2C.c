#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#include "libs/include/font.h"
#include "libs/include/ssd1306.h"
#include "libs/include/pio_config.h"
#include "libs/include/matriz_leds.h"


#define I2C_PORT i2c1 
#define I2C_SDA 14 
#define I2C_SCL 15
#define ENDERECO 0x3C
#define BUTTON_A 5
#define BUTTON_B 6


static volatile uint32_t last_time = 0;

typedef struct {
    uint led_green;
    uint led_blue;
    uint led_red;
} LED_PINS;


LED_PINS led_pins = {11,12,13};
static void gpio_irq_handler(uint gpio, uint32_t events);
ssd1306_t ssd;

int main()
{
    stdio_init_all();
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO,I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    np_init(7);

    gpio_init(led_pins.led_green);
    gpio_set_dir(led_pins.led_green, GPIO_OUT);
    gpio_put(led_pins.led_green, 0);

    gpio_init(led_pins.led_blue);
    gpio_set_dir(led_pins.led_blue, GPIO_OUT);
    gpio_put(led_pins.led_blue, 0);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);


    while (true) {
        if(stdio_usb_connected()){
            char c;
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "TECLA: ",2,4);
            printf("Digite um caractere para ser exibido no display\n");
            if(scanf("%c", &c) == 1 && c >= 'A'&& c <= 'z' ){
                ssd1306_draw_char(&ssd, c, 58, 4);
                ssd1306_send_data(&ssd);
            } else if(c >= '0' && c <= '9'){
                ssd1306_draw_char(&ssd, c, 58, 4);
                ssd1306_send_data(&ssd);
                int num = c - '0';
                np_set_leds(matriz[num], 0, 100, 100);
            } else{
                printf("Caractere inválido\n");
            }
        }
        sleep_ms(1000);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_time > 200000){
        last_time = current_time;
        char *led_status;
        ssd1306_fill(&ssd, false);
        np_set_leds(matriz[0], 0, 0, 0);
        if(gpio == BUTTON_A){
            gpio_put(led_pins.led_green, !gpio_get(led_pins.led_green));
            led_status = gpio_get(led_pins.led_green) ? "LED VERDE ON" : "LED VERDE OFF";
            ssd1306_draw_string(&ssd,led_status, 2, 4);
            ssd1306_send_data(&ssd);
        } else if (gpio == BUTTON_B){
            gpio_put(led_pins.led_blue, !gpio_get(led_pins.led_blue));
            led_status = gpio_get(led_pins.led_blue) ? "LED AZUL ON" : "LED AZUL OFF";
            ssd1306_draw_string(&ssd,led_status, 2, 4);
            ssd1306_send_data(&ssd);
        }
    }
}


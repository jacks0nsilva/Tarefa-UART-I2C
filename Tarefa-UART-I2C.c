#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "libs/font.h"
#include "libs/ssd1306.h"

#define I2C_PORT i2c1 
#define I2C_SDA 14 
#define I2C_SCL 15
#define ENDERECO 0x3C


int main()
{
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO,I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    bool cor = true;

    while (true) {
        if(stdio_usb_connected()){
            char c;
            printf("USB Conectado\n");
            printf("Digite um caractere para ser exibido no display\n");
            if(scanf("%c", &c) == 1 && c >= 'A'&& c <= 'z' ){
                ssd1306_draw_char(&ssd, c, 0, 0);
                ssd1306_send_data(&ssd);
            } else if(c >= '0' && c <= '9'){
                printf("Número de zero a nova digitado\n");
                printf("Chama função que imprime o número na matriz de LED 5X5\n");
            } else{
                printf("Caractere inválido\n");
            }
        }
        sleep_ms(1000);
    }
}

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
        ssd1306_fill(&ssd, !cor);
        ssd1306_draw_string(&ssd, "TESTE", 8, 10); // Desenha uma string para fins de testes
        ssd1306_send_data(&ssd);
        sleep_ms(1000);
    }
}

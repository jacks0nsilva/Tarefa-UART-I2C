#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

// Inclui as bibliotecas essnciais para o funcionamneto do programa
#include "libs/include/font.h"
#include "libs/include/ssd1306.h"
#include "libs/include/pio_config.h"
#include "libs/include/matriz_leds.h"


#define I2C_PORT i2c1 
#define I2C_SDA 14 
#define I2C_SCL 15
#define ENDERECO 0x3C
#define MATRIZ_LEDS 7

ssd1306_t ssd; // Estrutura para o display OLED

// Estrutura para definir o pino e se o mesmo é de entrada ou saída
typedef struct {
    uint pin;
    bool direction;
} GPIO_PIN;

// Varivável para controlar o tempo de debounce
static volatile uint32_t last_time = 0;

// Vetor de pinos GPIO
GPIO_PIN gpio_pins[4] = {
    {5, GPIO_IN}, // BOTÃO A
    {6, GPIO_IN}, // BOTÃO B
    {11, GPIO_OUT}, // LED VERDE
    {12, GPIO_OUT}, // LED AZUL
};

// Função para inicializar os pinos GPIO
void initialize_gpio(GPIO_PIN *pins){
    for(int i = 0; i < 4; i++){
        gpio_init(pins[i].pin);
        gpio_set_dir(pins[i].pin, pins[i].direction);
        // Se o pino for de saída, inicializa o pino em baixo nível
        if(pins[i].direction == GPIO_OUT){
            gpio_put(pins[i].pin, 0);
        }
        // Se o pino for de entrada, habilita o pull-up
        if(pins[i].direction == GPIO_IN){
            gpio_pull_up(pins[i].pin);
        }
    }
}

// Função para tratar a interrupção dos pinos GPIO
static void gpio_irq_handler(uint gpio, uint32_t events);


int main()
{
    stdio_init_all(); // Inicializa a comunicação serial

    
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o display em 400KHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Define a SDA como pino de comunicação I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Define a SCL como pino de comunicação I2C

    gpio_pull_up(I2C_SDA); // Habilita o pull-up na SDA
    gpio_pull_up(I2C_SCL); // Habilita o pull-up na SCL
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO,I2C_PORT); // Inicializa o display OLED
    ssd1306_config(&ssd); // Configura o display OLED
    ssd1306_send_data(&ssd); // Envia os dados para o display OLED

    ssd1306_fill(&ssd, false); // Limpa o display OLED



    np_init(MATRIZ_LEDS); // Inicializa a máquina de estados dos LEDs 5x5

    initialize_gpio(gpio_pins); // Inicializa os pinos GPIO (LEDs verde, azul e botões A e B)

    // Configura a interrupção dos botões A e B
    gpio_set_irq_enabled_with_callback(gpio_pins[0].pin, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(gpio_pins[1].pin, GPIO_IRQ_EDGE_FALL, true);

    

    // Exibe uma mensagem de inicialização no display OLED
    for(int i =5; i > 0; i-- ){
        char frase[20];
        sprintf(frase, "INICIANDO EM %d", i);
        ssd1306_draw_string(&ssd, frase, 10, 10);
        ssd1306_send_data(&ssd);
        sleep_ms(1000);
    }
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
  
    while (true) {
        // Verifica se o cabo USB está conectado
        if(stdio_usb_connected()){
            char c;
            ssd1306_draw_string(&ssd, "TECLA: ",2,4);

            printf("Digite um caractere para ser exibido no display\n");

            // Verifica se o caractere digitado é uma letra ou um número
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

// Função para tratamento da interrupção dos pinos GPIO
void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Pega o tempo atual

    // Verifica se o tempo de debounce foi atingido
    if(current_time - last_time > 200000){
        last_time = current_time;
        char *led_status;

        np_clear(); // Apaga a matriz de LEDs
        
        // Verifica qual botão foi pressionado e alterna o estado do LED correspondente
        if(gpio == gpio_pins[0].pin){
            gpio_put(gpio_pins[2].pin, !gpio_get(gpio_pins[2].pin));

            printf(gpio_get(gpio_pins[2].pin) ? "LED VERDE ACESO\n" : "LED VERDE APAGADO\n");
            led_status = gpio_get(gpio_pins[2].pin) ? "LED VERDE ON  " : "LED VERDE OFF";

            ssd1306_draw_string(&ssd,led_status, 2, 14);
            ssd1306_send_data(&ssd);
        } else if (gpio == gpio_pins[1].pin){
            gpio_put(gpio_pins[3].pin, !gpio_get(gpio_pins[3].pin));

            printf(gpio_get(gpio_pins[3].pin) ? "LED AZUL ACESO\n" : "LED AZUL APAGADO\n");
            led_status = gpio_get(gpio_pins[3].pin) ? "LED AZUL ON  " : "LED AZUL OFF";

            ssd1306_draw_string(&ssd,led_status, 2, 24);
            ssd1306_send_data(&ssd);
        }
    }
}


#include "matriz_leds.h"
#include "pio_config.h"

void draw_numbers(int number){
    np_set_leds(matriz[number], 0, 100, 100);
}
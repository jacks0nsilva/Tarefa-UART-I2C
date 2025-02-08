#include "matriz_leds.h"
#include "pio_config.h"

void draw_numbers(int number){
    switch (number)
    {
        case 0:
            np_set_leds(matriz[0],255,0,0);
            break;
        case 1:
            np_set_leds(matriz[1],0,255,0);
            break;
        case 2:
            np_set_leds(matriz[2],0,0,255);
            break;
        case 3:
            np_set_leds(matriz[3],255,255,0);
            break;
        case 4:
            np_set_leds(matriz[4],255,0,255);
            break;
        case 5:
            np_set_leds(matriz[5],0,255,255);
            break;
        case 6:
            np_set_leds(matriz[6],255,255,255);
            break;
        case 7:
            np_set_leds(matriz[7],255,0,0);
            break;
        case 8:
            np_set_leds(matriz[8],0,255,0);
            break;
        case 9: 
            np_set_leds(matriz[9],0,0,255);
            break;
        default:
            break;
    }
}
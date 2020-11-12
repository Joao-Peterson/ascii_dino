#ifndef _SPRITE_TREADMILL_
#define _SPRITE_TREADMILL_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ascii_screen.h"

/* ------------------------------------------------------- */

typedef struct treadmill_t treadmill_t;

/* ------------------------------------------------------- */

struct treadmill_t{
    unsigned int width;
    int *treadmill; 
    unsigned int spacing_min;
    unsigned int spacing_max;
    int dead_char;
    bool use_random_chars_from_range;
    int char_put;
    unsigned int char_range;

    void (*Shift)       (treadmill_t *mill, int how_many);
    void (*Populate)    (treadmill_t *mill, unsigned int zone_start, unsigned int zone_end);
    void (*Clear)       (treadmill_t *mill);
};

/* ------------------------------------------------------- */

treadmill_t *treadmill_new(unsigned int width, unsigned int spacing_min, unsigned int spacing_max, int dead_char, bool use_random_chars_from_range, int char_put, unsigned int char_range);

void treadmill_delete(treadmill_t *mill);

void treadmill_shift(treadmill_t *mill, int how_many);

void treadmill_populate(treadmill_t *mill, unsigned int zone_start, unsigned int zone_end);

void treadmill_clear(treadmill_t *mill);

int random_range(int a, int b);

/* ------------------------------------------------------- */

treadmill_t *treadmill_new(unsigned int width, unsigned int spacing_min, unsigned int spacing_max, int dead_char, bool use_random_chars_from_range, int char_put, unsigned int char_range){
    treadmill_t *mill = (treadmill_t*)malloc(sizeof(*mill));
    int *treadmill = (int*)malloc(width);

    memset(treadmill,dead_char,width);

    mill->width = width;
    mill->spacing_max = spacing_max;
    mill->spacing_min = spacing_min;
    mill->dead_char = dead_char;
    mill->use_random_chars_from_range = use_random_chars_from_range;
    mill->char_put = char_put;
    mill->char_range = char_range;
    mill->treadmill = treadmill;

    mill->Shift = treadmill_shift;
    mill->Populate = treadmill_populate;
    mill->Clear = treadmill_clear;

    return mill;
}

void treadmill_delete(treadmill_t *mill){
    free(mill->treadmill);
    free(mill);
}

void treadmill_shift(treadmill_t *mill, int how_many){
    if(how_many < 0){
        how_many = -how_many;

        for(int i = 0; i < how_many; i++){
            for(int j = 0; j < (mill->width - 1); j ++)
                mill->treadmill[j] = mill->treadmill[j+1];  

            mill->treadmill[mill->width - 1] = mill->dead_char;
        }
    }
    else{
        for(int i = 0; i < how_many; i++){
            for(int j = mill->width - 1; j > 0; j--)
                mill->treadmill[j] = mill->treadmill[j-1];  

            mill->treadmill[0] = mill->dead_char;
        }
    }
}

void treadmill_populate(treadmill_t *mill, unsigned int zone_start, unsigned int zone_end){
    unsigned int start = (zone_start <= zone_end) ? zone_start : zone_end;
    unsigned int end   = (zone_start <= zone_end) ? zone_end : zone_start;

    mill->Clear(mill);

    mill->treadmill[start] = random_range(mill->char_put, mill->char_put + mill->char_range);

    for(int i = start; i < end; i++){
        if(mill->treadmill[i]!=mill->dead_char){
            if(mill->use_random_chars_from_range)
                mill->treadmill[random_range(mill->spacing_min,mill->spacing_max)] = random_range(mill->char_put, mill->char_put + mill->char_range);
            else
                mill->treadmill[random_range(mill->spacing_min,mill->spacing_max)] = mill->char_put;
        }
    }
}

void treadmill_clear(treadmill_t *mill){
    memset(mill->treadmill,mill->dead_char,mill->width);
}

int random_range(int a, int b){
    return (rand() % (((a < b) ? b : a) - ((a < b) ? a : b))) + ((a < b) ? a : b);
}

#endif
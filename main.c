#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

#include "ascii_screen.h"
#include "win_res.h"

typedef struct{
    ASCII_COLOR_Typedef sky_color;
    ASCII_COLOR_Typedef ground_color;
    ASCII_COLOR_Typedef border_color;
    ASCII_COLOR_Typedef text_color;
    ASCII_COLOR_Typedef cloud_color;
    ASCII_COLOR_Typedef cacti_color;
    ASCII_COLOR_Typedef dino_color;
    ASCII_COLOR_Typedef ptero_color;
}COLOR_PALETTE;

const COLOR_PALETTE palette_normal = {
    .sky_color =        ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY,
    .ground_color =     ASCII_COLOR_BACKGROUND_YELLOW,
    .border_color =     ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .text_color =       ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .cloud_color =      ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_WHITE,
    .cacti_color =      ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_GREEN,
    .dino_color =       ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .ptero_color =      ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK
};

ascii_sprite **sprites_load(const char *name, unsigned int quantity, unsigned int width, unsigned int height, bool no_background, ASCII_COLOR_Typedef background_color, ASCII_COLOR_Typedef foreground_color){
    char **sprites_string_array = (char**)malloc(sizeof(char*)*quantity);
    char *buffer = (char*)malloc(strlen(name) + 1 + 4); // + 1 '\0' + 4 integer digits 

    for(int i = 0; i < quantity; i++){
        sprintf(buffer, "%s%i", name, i);
        sprites_string_array[i] = get_win_resource_binary_data(buffer);
    }

    ascii_sprite** sprite = ascii_sprite_array_new(width, height, quantity, sprites_string_array, no_background, background_color, foreground_color);

    for(int i = 0; i < quantity; i++)
        free(sprites_string_array[i]);
    free(sprites_string_array);

    return sprite;
}

const char *sounds_tag_array[] = {
    "hit0",
    "jump0",
    "jump1",
    "coin0",
    "coin1",
    "power0",
    "power1",
    "power2"
};

typedef enum{
    SOUND_HIT0,  
    SOUND_JUMP0, 
    SOUND_JUMP1, 
    SOUND_COIN0, 
    SOUND_COIN1, 
    SOUND_POWER0,
    SOUND_POWER1,
    SOUND_POWER2
}SOUND_Typedef;

void play_sound(SOUND_Typedef sound){
    PlaySoundA(sounds_tag_array[sound], GetModuleHandle(NULL), SND_RESOURCE);
}

int main(void) {

    float time_ratio = 1.0;
    float time;
    float time_counter = 0.0;
    clock_t time_stamp;
    clock_t time_elapsed;
    
    int sprite_select = 3;

    ascii_screen *console = ascii_screen_new(77, 17);
    COLOR_PALETTE *palette = &palette_normal;  

    ascii_sprite **rex_sprite   = sprites_load("rex",4,9,6,true,palette->sky_color, palette->dino_color);    
    ascii_sprite **cacti_sprite = sprites_load("cacti",4,15,false,4,palette->sky_color, palette->cacti_color);    
    
    play_sound(SOUND_COIN0);

    while(1)
    {
        time = time_elapsed * time_ratio;
        time_counter += time;
        time_stamp = clock();

        if(time_counter > 250.0){
            time_counter = 0.0;
        }

        console->Fill_screen(console, ' ', palette->sky_color);
        console->Draw_box(console,0,0,console->width - 1,console->height - 1, ASCII_LAYER_FOREGROUND, ASCII_BRUSH_ARRAY[3], palette->border_color, ' ', palette->sky_color);
        console->Draw_frame(console,1,1,7,3, ASCII_FRAME_CHARSET_THICK, palette->border_color);
        console->Put_string(console,2,2,5,"Score", palette->text_color);
        console->Draw_sprite(console,ASCII_ANCHOR_TOP_LEFT,(console->width - rex_sprite[0]->width) / 2, (console->height - rex_sprite[0]->height) / 2,rex_sprite[sprite_select]);
        console->Display_buffer(console);


        time_elapsed = clock() - time_stamp;
    }

    console->Delete(console);
    rex_sprite[0]->Delete_array(rex_sprite);
    cacti_sprite[0]->Delete_array(cacti_sprite);

    return 0;
}
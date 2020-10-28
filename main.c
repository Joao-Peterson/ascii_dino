#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#include "ascii_screen.h"
#include "win_res.h"

/* ------------------------- Typedef's - Enum's - Structs -------------------------- */

typedef struct{
    ASCII_COLOR_Typedef sky_color;
    ASCII_COLOR_Typedef ground_color;
    ASCII_COLOR_Typedef border_color;
    ASCII_COLOR_Typedef logo_color;
    ASCII_COLOR_Typedef text_color;
    ASCII_COLOR_Typedef cloud_color;
    ASCII_COLOR_Typedef cacti_color;
    ASCII_COLOR_Typedef dino_color;
    ASCII_COLOR_Typedef ptero_color;
}COLOR_PALETTE;

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

/* ------------------------- Globals ----------------------------------------------- */

const COLOR_PALETTE palette_normal = {
    .sky_color =        ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY,
    .ground_color =     ASCII_COLOR_BACKGROUND_YELLOW | ASCII_COLOR_FOREGROUND_YELLOW | ASCII_COLOR_FOREGROUND_INTENSITY,
    .border_color =     ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .logo_color =       ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .text_color =       ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .cloud_color =      ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_WHITE,
    .cacti_color =      ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_GREEN,
    .dino_color =       ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .ptero_color =      ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK
};

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

/* ------------------------- Prototypes -------------------------------------------- */

ascii_sprite **sprites_load(const char *name, unsigned int quantity, unsigned int width, unsigned int height, bool no_background, ASCII_COLOR_Typedef background_color, ASCII_COLOR_Typedef foreground_color);

void play_sound(SOUND_Typedef sound);

/* ------------------------- Main -------------------------------------------------- */

int main(void) {

    float time_ratio = 1.0;
    float time;
    clock_t time_stamp;
    clock_t time_elapsed;

    float text_blinker_timer = 0.0;
    bool text_blinker = 1;
    float ground_move_timer = 0.0;
    bool ground_move = 0;
    
    int sprite_select = 3;
    unsigned int current_screen = 0;
    char keyboard_input;

    const char *msg_main_screen = "Press space to Start - Game by Peterson - 2020.";

    ascii_screen *console = ascii_screen_new(77, 17);

    COLOR_PALETTE *palette = (COLOR_PALETTE*)&palette_normal;  

    ascii_sprite **rex_sprite   = sprites_load("rex",4,9,6,true,palette->sky_color, palette->dino_color);    
    ascii_sprite **cacti_sprite = sprites_load("cacti",4,15,false,4,palette->sky_color, palette->cacti_color);    
    ascii_sprite **logo_sprite  = sprites_load("logo",1,69,9,true,palette->sky_color,palette->logo_color);
    
    char *ground_ascii = get_win_resource_binary_data("ground0");
    ascii_canvas *ground = ascii_canvas_new(100,3,ground_ascii, false, palette->ground_color, palette->ground_color);
    free(ground_ascii);

    play_sound(SOUND_COIN0);

    /* ------------------------- Main Loop --------------------------------------------- */

    while(1)
    {
        // TIMING ...
        time = time_elapsed * time_ratio;
        text_blinker_timer += time;
        ground_move_timer += time;
        time_stamp = clock();

        if(text_blinker_timer > 400.0){ text_blinker_timer = 0.0; text_blinker ^= 1; }
        if(ground_move_timer  > 100.0){ ground_move_timer  = 0.0; ground_move = 1  ; }


        //INPUT DETECTION
        if(kbhit())
            keyboard_input = getch();
        else
            keyboard_input = 0;

        fflush(stdin);

        switch(keyboard_input){
            case ' ':
                current_screen = 1;
                break;

            default:
                break;
        }

        // GAME LOGIC
        switch(current_screen){
            case 0: // -------------- Main screen
                console->Draw_box(console,0,0,console->width - 1,console->height - 1,ASCII_LAYER_BACKGROUND | ASCII_LAYER_FOREGROUND, ASCII_BRUSH_ARRAY[2], palette->border_color, ' ', palette->sky_color);
                console->Draw_sprite(console, ASCII_ANCHOR_BOTTOM_LEFT, (console->width - logo_sprite[0]->width) / 2, 11, logo_sprite[0]);
                if(text_blinker)
                    console->Put_string(console, (console->width - strlen(msg_main_screen)) / 2, 12, strlen(msg_main_screen), (char*)msg_main_screen, palette->text_color);
                break;
            
            case 1: // -------------- Running screen
                console->Draw_box(console,0,0,console->width - 1,console->height - 1,ASCII_LAYER_BACKGROUND | ASCII_LAYER_FOREGROUND, ASCII_BRUSH_ARRAY[2], palette->border_color, ' ', palette->sky_color);
                if(ground_move){ ground->Shift(ground,1,0); ground_move = 0; }
                console->Draw_canvas(console,ASCII_ANCHOR_BOTTOM_LEFT, 1, console->height - 1, console->width - 2, ground->height, ground);
                //console->Draw_sprite(console,ASCII_ANCHOR_BOTTOM_LEFT,3,13,rex_sprite[0]);
                break;

            default:
                break;
        }




        // DISPLAY EVERYTHING
        console->Display_buffer(console);

        // TIMING ...
        time_elapsed = clock() - time_stamp;
    }

    console->Delete(console);
    rex_sprite[0]->Delete_array(rex_sprite);
    cacti_sprite[0]->Delete_array(cacti_sprite);

    return 0;
}


/* ------------------------- Implementations --------------------------------------- */


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

void play_sound(SOUND_Typedef sound){
    PlaySoundA(sounds_tag_array[sound], GetModuleHandle(NULL), SND_RESOURCE);
}

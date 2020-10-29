#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

#include "ascii_screen.h"
#include "win_res.h"

/* ------------------------- Typedef's - Enum's - Structs -------------------------- */

typedef struct{
    ASCII_COLOR_Typedef sky_color;
    ASCII_COLOR_Typedef menu_color;
    ASCII_COLOR_Typedef ground_back_color;
    ASCII_COLOR_Typedef ground_fore_color;
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
    .sky_color =          ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY,
    .menu_color =         ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_FOREGROUND_BLACK,
    .ground_back_color =  ASCII_COLOR_BACKGROUND_YELLOW,
    .ground_fore_color =  ASCII_COLOR_FOREGROUND_YELLOW | ASCII_COLOR_FOREGROUND_INTENSITY,
    .border_color =       ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .logo_color =         ASCII_COLOR_FOREGROUND_BLACK,
    .text_color =         ASCII_COLOR_FOREGROUND_BLACK,
    .cloud_color =        ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_WHITE,
    .cacti_color =        ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_GREEN,
    .dino_color =         ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK,
    .ptero_color =        ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY | ASCII_COLOR_FOREGROUND_BLACK
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

void play_sound_async(SOUND_Typedef sound);

int jump_function(float t2, int h, float t);

int random_range(int a, int b);

/* ------------------------- Main -------------------------------------------------- */

int main(void) {

    float time_ratio = 1.0;
    float timer;
    clock_t time_stamp;
    clock_t time_elapsed;

    float text_blinker_timer = 0.0;
    bool text_blinker = 1;
    float ground_move_timer = 0.0;
    bool ground_move = 0;
    float walk_move_timer = 0.0;
    bool walk_move = 0;
    float clouds_move_timer = 0.0;
    bool clouds_move = 0;

    float score_timer = 0.0;
    unsigned long score = 0; // 10 decimal places
    unsigned long score_last = 0; 
    const unsigned long score_gain = 100; 
    const int score_sound_limit = 10000;
    const char* score_msg = "Score:"; 
    char *score_buffer = (char*)malloc(strlen(score_msg)+10+1); 

    float jump_timer = 0.0;
    bool jump_flag = 0;
    bool in_air_flag = 0;

    const float t2_max = 2000.0;
    const float t2_min = 1250.0;
    const float t2_gain = 50.0;
    float t2 = 0.0;
    int max_height = 8;
    int pos_y = 0;

    const float cacti_delay = 2000.0;

    
    int sprite_select = 0;
    unsigned int current_screen = 0;
    char keyboard_input;

    const char *msg_main_screen = "Press space to Start - Game by Peterson - 2020.";

    ascii_screen *console = ascii_screen_new(77, 17);

    COLOR_PALETTE *palette = (COLOR_PALETTE*)&palette_normal;  

    ascii_sprite **rex_sprite   = sprites_load("rex",4,9,6,true,palette->sky_color, palette->dino_color);    
    ascii_sprite **cacti_sprite = sprites_load("cacti",4,15,false,4,palette->sky_color, palette->cacti_color);    
    ascii_sprite **logo_sprite  = sprites_load("logo",1,69,9,true,palette->menu_color,palette->logo_color | palette->menu_color);
    
    char *ground_ascii = get_win_resource_binary_data("ground0");
    ascii_canvas *ground = ascii_canvas_new(100,3,ground_ascii, false, palette->ground_fore_color | palette->ground_back_color, palette->ground_fore_color | palette->ground_back_color);
    free(ground_ascii);
    char *clouds_ascii = get_win_resource_binary_data("cloud0");
    ascii_canvas *clouds = ascii_canvas_new(150,10,clouds_ascii, true, palette->sky_color, palette->cloud_color);
    free(clouds_ascii);

    srand(time(NULL));

    play_sound_async(SOUND_COIN0);

    /* ------------------------- Main Loop --------------------------------------------- */

    while(1)
    {
        // TIMING ...
        timer = time_elapsed * time_ratio;
        text_blinker_timer += timer;
        ground_move_timer += timer;
        walk_move_timer += timer;
        clouds_move_timer += timer;

        if(current_screen == 1){ score_timer += timer; }
        if(in_air_flag){ jump_timer += timer; }
        
        time_stamp = clock();

        if(text_blinker_timer   > 400.0)    { text_blinker_timer = 0.0; text_blinker ^= 1   ; }
        if(ground_move_timer    > 50.0)     { ground_move_timer  = 0.0; ground_move = 1     ; }
        if(walk_move_timer      > 100.0)    { walk_move_timer    = 0.0; walk_move = 1       ; }
        if(clouds_move_timer    > 800.0)    { clouds_move_timer  = 0.0; clouds_move = 1     ; }
        if(score_timer          > 100.0)    { score_timer        = 0.0; score += score_gain ; if(score >= (score_last + score_sound_limit)){ play_sound_async(random_range(SOUND_COIN0, SOUND_POWER2)); score_last = score; } }

        //INPUT DETECTION
        if(kbhit())
            keyboard_input = getch();
        else
            keyboard_input = 0;

        fflush(stdin);

        switch(keyboard_input){
            case ' ':
                switch(current_screen){
                    case 0:
                        current_screen = 1;
                        break;

                    case 1:
                        switch(in_air_flag){
                            case 0:
                                jump_flag = 1;
                                break;

                            case 1:
                                if(t2 >= 0.0 && t2 < t2_max) 
                                    t2 += t2_gain;
                                break;
                        }
                        break;
                }
                break;

            default:
                break;
        }

        // GAME LOGIC
        switch(current_screen){
            case 0: // -------------- Main screen
                console->Draw_box(console,0,0,console->width - 1,console->height - 1,ASCII_LAYER_BACKGROUND | ASCII_LAYER_FOREGROUND, ASCII_BRUSH_ARRAY[2], palette->border_color, ' ', palette->menu_color);
                console->Draw_sprite(console, ASCII_ANCHOR_BOTTOM_LEFT, (console->width - logo_sprite[0]->width) / 2, 11, logo_sprite[0]);
                if(text_blinker)
                    console->Put_string(console, (console->width - strlen(msg_main_screen)) / 2, 12, strlen(msg_main_screen), (char*)msg_main_screen, palette->text_color | palette->menu_color);
                break;
            
            case 1: // -------------- Running screen
                console->Draw_box(console,0,0,console->width - 1,console->height - 1,ASCII_LAYER_BACKGROUND | ASCII_LAYER_FOREGROUND, ASCII_BRUSH_ARRAY[2], palette->border_color, ' ', palette->sky_color);
                if(ground_move){ ground->Shift(ground,1,0); ground_move = 0; }
                console->Draw_canvas(console,ASCII_ANCHOR_BOTTOM_LEFT, 1, console->height - 1, console->width - 2, ground->height, ground);
                if(clouds_move){ clouds->Shift(clouds,1,0); clouds_move = 0; }
                console->Draw_canvas(console,ASCII_ANCHOR_TOP_LEFT,1,1,console->width - 2, clouds->height, clouds);
                
                // dinosaur
                if(walk_move){ if(sprite_select == 1){ sprite_select = 2; }else{ sprite_select = 1; } walk_move = 0;} 
                
                if(jump_flag){ in_air_flag = 1; jump_flag = 0; t2 = t2_min; }

                if(in_air_flag){
                    pos_y = jump_function(t2,max_height,jump_timer);
                    if(jump_timer > t2/2 && pos_y == 0){ in_air_flag = 0; jump_timer = 0; jump_flag = 0; } 

                    if(pos_y >= 1){
                        rex_sprite[0]->Array_recolour(rex_sprite, ASCII_ANCHOR_TOP_LEFT, 0, rex_sprite[0]->height - 1, rex_sprite[0]->width, 1, ASCII_LAYER_FOREGROUND | ASCII_LAYER_BACKGROUND, palette->sky_color);
                        console->Draw_sprite(console,ASCII_ANCHOR_BOTTOM_LEFT,3,console->height - ground->height - pos_y,rex_sprite[0]);
                    }
                    else{
                        rex_sprite[0]->Array_recolour(rex_sprite, ASCII_ANCHOR_TOP_LEFT, 0, rex_sprite[0]->height - 1, rex_sprite[0]->width, 1, ASCII_LAYER_FOREGROUND | ASCII_LAYER_BACKGROUND, palette->ground_back_color);
                        console->Draw_sprite(console,ASCII_ANCHOR_BOTTOM_LEFT,3,console->height - ground->height - pos_y,rex_sprite[sprite_select]);
                    }
                }
                else{
                    rex_sprite[0]->Array_recolour(rex_sprite, ASCII_ANCHOR_TOP_LEFT, 0, rex_sprite[0]->height - 1, rex_sprite[0]->width, 1, ASCII_LAYER_FOREGROUND | ASCII_LAYER_BACKGROUND, palette->ground_back_color);
                    console->Draw_sprite(console,ASCII_ANCHOR_BOTTOM_LEFT,3,console->height - ground->height,rex_sprite[sprite_select]);
                }

                // cacti
                
                

                // score
                console->Draw_frame(console,console->width - 2, 1, console->width - 2 - strlen(score_msg) - 10 - 2 + 1, 1 + 2, ASCII_FRAME_CHARSET_THICK, palette->border_color);
                sprintf(score_buffer,"%s%010u",score_msg,score);
                console->Put_string(console,console->width - 3 - strlen(score_msg) - 10 + 1, 2, strlen(score_msg) + 10, score_buffer, palette->sky_color | palette->text_color);

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

void play_sound_async(SOUND_Typedef sound){
    PlaySoundA(sounds_tag_array[sound], GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
}

int jump_function(float t2, int h, float t){
    float a = -(4.0*h)/pow(t2,2);
    float pos_y = a*(t-0)*(t-t2);

    if(pos_y <= 0.0)
        return 0;
    else
        return (int) pos_y; 
} 

int random_range(int a, int b){
    return (rand() % (((a < b) ? b : a) - ((a < b) ? a : b))) + ((a < b) ? a : b);
} 
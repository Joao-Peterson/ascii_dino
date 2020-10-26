#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>

#include "ascii_screen.h"
#include "win_res.h"

// char *get_win_error(){
//     DWORD error_code = GetLastError();

//     char *error_msg = malloc(sizeof(*error_msg)*200);
    
//     FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, // format as error from system
//                     NULL, // aditional parameter
//                     error_code, // DWORD error code
//                     MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // language to format string
//                     (LPSTR)error_msg, // string, cast before passing
//                     200, // size
//                     NULL // aditional parameters
//                     );

//     return error_msg;
// }

// 0xB0
// 0xB1
// 0xB2
// 0xDB


int main(void) {

    float time_ratio = 1.0;
    float time;
    float time_counter = 0.0;
    clock_t time_stamp;
    clock_t time_elapsed;
    
    ASCII_COLOR_Typedef sky = ASCII_COLOR_BACKGROUND_BLUE | ASCII_COLOR_BACKGROUND_INTENSITY;

    int sprite_select = 1;

    char **rex_sprites_string = (char**)malloc(sizeof(char*)*4);
    rex_sprites_string[0] = get_win_resource_binary_data("rex_0");
    rex_sprites_string[1] = get_win_resource_binary_data("rex_1");
    rex_sprites_string[2] = get_win_resource_binary_data("rex_2");
    rex_sprites_string[3] = get_win_resource_binary_data("rex_3");


    ascii_sprite** rex_sprite = ascii_sprite_new(9,6, 4, rex_sprites_string, true, sky, sky | ASCII_COLOR_FOREGROUND_GREEN | ASCII_COLOR_FOREGROUND_INTENSITY);

    for(int i = 0; i < 4; i++)
        free(rex_sprites_string[i]);
    free(rex_sprites_string);

    ascii_screen *console = ascii_screen_new(77, 17);


    while(1)
    {
        time = time_elapsed * time_ratio;
        time_counter += time;

        time_stamp = clock();

        if(time_counter > 250.0){
            if(sprite_select == 1)
                sprite_select = 2;
            else
                sprite_select = 1;
                
            time_counter = 0.0;
        }

        console->Fill_screen(console, ' ', sky);
        console->Draw_sprite(console,ANCHOR_TOP_LEFT,(console->width - rex_sprite[0]->width) / 2, (console->height - rex_sprite[0]->height) / 2,rex_sprite[sprite_select]);
        console->Display_buffer(console);
                
        time_elapsed = clock() - time_stamp;
    }

    console->Delete(console);
    rex_sprite[0]->Delete(rex_sprite);

    return 0;
}
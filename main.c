#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>

#include "ascii_screen.h"

char *get_win_error()
{
    DWORD error_code = GetLastError();

    char *error_msg = malloc(sizeof(*error_msg)*200);
    
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, // format as error from system
                    NULL, // aditional parameter
                    error_code, // DWORD error code
                    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // language to format string
                    (LPSTR)error_msg, // string, cast before passing
                    200, // size
                    NULL // aditional parameters
                    );

    return error_msg;
}


int main(){

    // 0xB0
    // 0xB1
    // 0xB2
    // 0xDB

    ascii_screen *console = ascii_screen_new(77, 17);

    float time_ratio = 1.0;
    float time;
    clock_t time_stamp;
    clock_t time_elapsed;

    while(1)
    {
        time = time_elapsed * time_ratio;
        time_stamp = clock();

        memset(console->buffer,0xB0,console->size);
        console->Display_buffer(console);
        
        

        // char *msg = get_win_error();
        // free(msg);

        time_elapsed = clock() - time_stamp;
    }

    return 0;
}
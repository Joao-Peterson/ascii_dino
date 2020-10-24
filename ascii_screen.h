#include <stdlib.h>
#include <Windows.h>

/* ------------------ */

typedef struct ascii_screen ascii_screen;

typedef struct ascii_sprite ascii_sprite;

/* ------------------ */

ascii_screen *ascii_screen_new(int width, int height);

void ascii_screen_display_buffer(ascii_screen* screen);


ascii_sprite *ascii_sprite_new(char *sprite_string);

/* ------------------ */

struct ascii_screen{
    int w;
    int h;
    int size;
    char *buffer;

    HANDLE hscreenbuffer;

    void (*Display_buffer) (ascii_screen* screen);
} ;

struct ascii_sprite{
    char* stream;
};

/* ------------------ */

ascii_screen *ascii_screen_new(int width, int height){

    ascii_screen *scr = (ascii_screen*)malloc(sizeof(*scr));

    int w_width  = width;
    int w_height = height;
    int b_width  = width;
    int b_height = height;

    SMALL_RECT w_size = { 0, 0, w_width, w_height};
    COORD b_size = {b_width, b_height};

    char *buffer = (char*)calloc(b_width * b_height + 1, sizeof(char));

    HANDLE hscreenbuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(hscreenbuffer);
    SetConsoleScreenBufferSize(hscreenbuffer, b_size);
    SetConsoleWindowInfo(hscreenbuffer, TRUE, &w_size);
    SetConsoleMode(hscreenbuffer, ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT);

    scr->w = width;
    scr->h = height;
    scr->size = width * height;
    scr->hscreenbuffer = hscreenbuffer;
    scr->buffer = buffer;
    scr->Display_buffer = ascii_screen_display_buffer;

    return scr;
}

void ascii_screen_display_buffer(ascii_screen* screen){
    LPDWORD a;
    screen->buffer[screen->size] = '\0';
    WriteConsoleOutputCharacterA(screen->hscreenbuffer, screen->buffer, screen->size, (COORD){0,0}, a);
}

void ascii_screen_draw_sprite(ascii_screen *screen, ascii_sprite *sprite){
    int i,j,k;

    while(sprite->stream[i] != '\0'){

        sprite->stream[i];

        i++;
    }
}



ascii_sprite *ascii_sprite_new(char *sprite_string){
    ascii_sprite *sprite = (ascii_sprite*)malloc(sizeof(ascii_sprite));

    sprite->stream = sprite_string;

    return sprite;
}


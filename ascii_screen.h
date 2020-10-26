#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

/* ------------------ */

typedef struct ascii_screen ascii_screen;

typedef struct ascii_sprite ascii_sprite;

/* ------------------ */

typedef enum{
    ANCHOR_TOP_LEFT         = 1,   
    ANCHOR_TOP_RIGHT        = 2,   
    ANCHOR_BOTTOM_LEFT      = 3,   
    ANCHOR_BOTTOM_RIGHT     = 4,
    TL  =   1, 
    TR  =   2,
    BL  =   3,
    BR  =   4
}ANCHOR_Typedef;

typedef enum{
    ASCII_COLOR_FOREGROUND_BLUE         = 0x01,
    ASCII_COLOR_FOREGROUND_GREEN        = 0x02,
    ASCII_COLOR_FOREGROUND_RED          = 0x04,
    
    ASCII_COLOR_FOREGROUND_BLACK        = 0x00,
    ASCII_COLOR_FOREGROUND_WHITE        = 0x07,
    ASCII_COLOR_FOREGROUND_INTENSITY    = 0x08,
    
    ASCII_COLOR_FOREGROUND_CYAN         = 0x01 | 0x02,
    ASCII_COLOR_FOREGROUND_YELLOW       = 0x02 | 0x04,
    ASCII_COLOR_FOREGROUND_MAGENTA      = 0x01 | 0x04,


    ASCII_COLOR_BACKGROUND_BLUE         = 0x10,
    ASCII_COLOR_BACKGROUND_GREEN        = 0x20,
    ASCII_COLOR_BACKGROUND_RED          = 0x40,
    
    ASCII_COLOR_BACKGROUND_BLACK        = 0x00,
    ASCII_COLOR_BACKGROUND_WHITE        = 0x70,
    ASCII_COLOR_BACKGROUND_INTENSITY    = 0x80,   
    
    ASCII_COLOR_BACKGROUND_CYAN         = 0x10 | 0x20,
    ASCII_COLOR_BACKGROUND_YELLOW       = 0x20 | 0x40,
    ASCII_COLOR_BACKGROUND_MAGENTA      = 0x10 | 0x40

}ASCII_COLOR_Typedef;

/* ------------------ */

ascii_screen *ascii_screen_new(unsigned int width, unsigned int height);

void ascii_screen_delete(ascii_screen *screen);

void ascii_screen_display_buffer(ascii_screen* screen);

void ascii_screen_draw_sprite(ascii_screen *screen, ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite);

void ascii_screen_fill_screen(ascii_screen *screen, char character, ASCII_COLOR_Typedef color);

void ascii_screen_clear(ascii_screen *screen);

ascii_sprite **ascii_sprite_new(unsigned int width, unsigned int height, unsigned int files_qty, char **array, bool no_background, ASCII_COLOR_Typedef background_color, ASCII_COLOR_Typedef font_color);

void ascii_sprite_delete(ascii_sprite **sprite_array);

/* ------------------ */

struct ascii_screen{
    int width;
    int height;
    int size;
    CHAR_INFO *stream;
    CHAR_INFO **buffer;
    HANDLE hscreenbuffer;

    void (*Display_buffer) (ascii_screen* screen);
    void (*Delete) (ascii_screen *screen);
    void (*Draw_sprite) (ascii_screen *screen, ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite);
    void (*Fill_screen) (ascii_screen *screen, char character, ASCII_COLOR_Typedef color);
    void (*Clear) (ascii_screen *screen);
} ;

struct ascii_sprite{
    int width;
    int height;
    int sprite_qty;
    WORD background_color;
    bool no_background;
    WORD font_color;
    CHAR_INFO **sprite;

    void (*Delete) (ascii_sprite **sprite_array);
};

/* ------------------ */

ascii_screen *ascii_screen_new(unsigned int width, unsigned int height){

    ascii_screen *scr = (ascii_screen*)malloc(sizeof(*scr));

    int w_width  = width;
    int w_height = height;
    int b_width  = width;
    int b_height = height;

    SMALL_RECT w_size = { 0, 0, w_width, w_height};
    COORD b_size = {b_width, b_height};

    CHAR_INFO *buffer_stream = (CHAR_INFO*)calloc(b_height * b_width + 1, sizeof(CHAR_INFO)); // char string stream
    memset(buffer_stream, 0x20, width * height);

    CHAR_INFO **buffer_mat = (CHAR_INFO**)calloc(b_height, sizeof(CHAR_INFO*)); // pointer array for acessing the stream

    for(int i = 0; i < b_height; i++) // pointers of the array
        buffer_mat[i] = buffer_stream + (i * b_width);

    HANDLE hscreenbuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(hscreenbuffer);
    SetConsoleScreenBufferSize(hscreenbuffer, b_size);
    SetConsoleWindowInfo(hscreenbuffer, TRUE, &w_size);
    SetConsoleMode(hscreenbuffer, ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT);

    scr->width = width;
    scr->height = height;
    scr->size = width * height;
    scr->hscreenbuffer = hscreenbuffer;
    scr->stream = buffer_stream;
    scr->buffer = buffer_mat;
    scr->Delete = ascii_screen_delete;
    scr->Display_buffer = ascii_screen_display_buffer;
    scr->Draw_sprite = ascii_screen_draw_sprite;
    scr->Fill_screen = ascii_screen_fill_screen;
    scr->Clear = ascii_screen_clear;

    return scr;
}

void ascii_screen_delete(ascii_screen *screen){
    CloseHandle(screen->hscreenbuffer);

    free(screen->buffer);
    free(screen->stream);
    free(screen);
}

void ascii_screen_display_buffer(ascii_screen* screen){
    SMALL_RECT a = {.Top = 0, .Left = 0, .Bottom = screen->height, .Right = screen->width};
    screen->stream[screen->size].Char.AsciiChar = '\0';
    WriteConsoleOutputA(screen->hscreenbuffer, screen->stream, (COORD){screen->width, screen->height}, (COORD){0,0}, &a);
}

void ascii_screen_draw_sprite(ascii_screen *screen, ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite){ 

    int delta_x, delta_y;

    switch(anchor){
        case ANCHOR_TOP_LEFT:
            delta_x = x;
            delta_y = y;
            break;
            
        case ANCHOR_TOP_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = y;
            break;
            
        case ANCHOR_BOTTOM_LEFT:
            delta_x = x;
            delta_y = (y - sprite->height);
            break;
            
        case ANCHOR_BOTTOM_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = (y - sprite->height);
            break;
    }


    for(int i = 0; i < sprite->height; i++){
        for(int j = 0; j < sprite->width; j++){
            
            if(((i + delta_y) >= 0 && (i + delta_y) < screen->height) && ((j + delta_x) >= 0 && (j + delta_x) < screen->width)) // inside the screen
                if(sprite->sprite[i][j].Char.AsciiChar != ' ' || sprite->no_background == 0) // when ' ', space, dont print, except when no_background is 0, then print
                    screen->buffer[i + delta_y][j + delta_x] = sprite->sprite[i][j];

        }
    }    
}

void ascii_screen_fill_screen(ascii_screen *screen, char character, ASCII_COLOR_Typedef color){
    CHAR_INFO clear_pixel = {.Char.AsciiChar = character, .Attributes = color};

    for(int i = 0; i < screen->height; i++)
        for(int j = 0; j < screen->width; j++)
            screen->buffer[i][j] = clear_pixel;        
}

void ascii_screen_clear(ascii_screen *screen){
    ascii_screen_fill_screen(screen, ' ', 0);
}




ascii_sprite **ascii_sprite_new(unsigned int width, unsigned int height, unsigned int files_qty, char **array, bool no_background, ASCII_COLOR_Typedef background_color, ASCII_COLOR_Typedef font_color){
    
    ascii_sprite **sprite_array = (ascii_sprite**)calloc(files_qty,sizeof(ascii_sprite*));
    ascii_sprite *sprite_cursor;

    char *sprite_string;

    for(int i = 0; i < files_qty; i++){ // for each file
        
        // alocate the sprite struct
        sprite_array[i] = (ascii_sprite*)calloc(1,sizeof(ascii_sprite));
        sprite_cursor = sprite_array[i];
        

        sprite_string = array[i];

        sprite_cursor->sprite = (CHAR_INFO**)calloc(height, sizeof(CHAR_INFO*));

        for(int j = 0; j < height; j ++){ // for each line
        
            sprite_cursor->sprite[j] = (CHAR_INFO*)calloc(width, sizeof(CHAR_INFO));

            int is_inline = 1;
            int is_insprite = 1;

            for(int k = 0; k < width; k++){ // for each column
                if( !is_insprite ){
                    sprite_cursor->sprite[j][k].Char.AsciiChar = ' ';
                    sprite_cursor->sprite[j][k].Attributes = (WORD) background_color;
                }
                else if( (sprite_string[k] != '\n' && sprite_string[k] != '\0') && is_inline){ // reads until a '\n' or '\0' occur, in this case fill with ' ' spaces;
                    sprite_cursor->sprite[j][k].Char.AsciiChar = sprite_string[k];
                    sprite_cursor->sprite[j][k].Attributes = (WORD) font_color;
                }
                else if( sprite_string[k] == '\0' ){
                    sprite_cursor->sprite[j][k].Char.AsciiChar = ' ';
                    sprite_cursor->sprite[j][k].Attributes = (WORD) background_color;
                    is_inline = 0;
                    is_insprite = 0;
                }
                else{
                    sprite_cursor->sprite[j][k].Char.AsciiChar = ' ';
                    sprite_cursor->sprite[j][k].Attributes = (WORD) background_color;
                    is_inline = 0;
                }
            }

            if (is_insprite){ // dont run pointer if sprite ended
                int k = 0;
    
                while(sprite_string[k] != '\n' && sprite_string[k] != '\0'){ // gets a pointer to next line
                    sprite_string++;
                }
                
                sprite_string++; // point to next line
            }
        }

        sprite_cursor->sprite_qty = files_qty;
        sprite_cursor->width = width;
        sprite_cursor->height = height;
        sprite_cursor->no_background = no_background;
        sprite_cursor->Delete = ascii_sprite_delete;
    }

    return sprite_array;
}

void ascii_sprite_delete(ascii_sprite **sprite_array){
    int s_qty = sprite_array[0]->sprite_qty;
    int height = sprite_array[0]->height;

    for (int i = 0; i < s_qty; i++){
        
        for(int j = 0; j < height; j++){
            free(sprite_array[i]->sprite[j]);    
        }
        
        free(sprite_array[i]->sprite);
        free(sprite_array[i]);
    }

    free(sprite_array);
}

// IMPLEMENT A RE COLOURING FUNCTION AND SEPARATE SPRITE FUNCTIONS BY ARRAY/SINGLE.
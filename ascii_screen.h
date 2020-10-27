#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

/* ------------------ */

typedef struct ascii_screen ascii_screen;

typedef struct ascii_sprite ascii_sprite;

/* ------------------ */

typedef enum{
    ASCII_ANCHOR_TOP_LEFT         = 1,   
    ASCII_ANCHOR_TOP_RIGHT        = 2,   
    ASCII_ANCHOR_BOTTOM_LEFT      = 3,   
    ASCII_ANCHOR_BOTTOM_RIGHT     = 4,
    TL  =   1, 
    TR  =   2,
    BL  =   3,
    BR  =   4
}ASCII_ANCHOR_Typedef;

typedef enum{
    ASCII_LAYER_BACKGROUND  = 0x01,
    ASCII_LAYER_FOREGROUND  = 0x02,
    ASCII_LAYER_FONT        = 0x02
}ASCII_LAYER_Typedef;

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

const char ASCII_LUMINESCENCE_ARRAY[] = {
    '.',
    ',',
    '-',
    '~',
    ':',
    ';',
    '=',
    '!',
    '*',
    '#',
    '$',
    '@'
};

// in hex because of the file encoding not supporting extended ascii
const char ASCII_BRUSH_ARRAY[] = {    
    0xB0,
    0xB1,
    0xB2,
    0xDB
};

const char ASCII_BOX_LINE_THICK[] = {
    0xCD, // horizontal
    0xBA, // vertical
    0xC9, // top left corner
    0xBB, // top right corner
    0xC8, // bottom left corner
    0xBC, // bottom right corner
    0xB9, // 3 dir. left
    0xCA, // 3 dir. up
    0xCC, // 3 dir. right
    0xCB, // 3 dir. down
    0xCE, // 4 directinal
};

const char ASCII_BOX_LINE_THIN[] = {
    0xC4, // horizontal
    0xB3, // vertical
    0xDA, // top left corner
    0xBF, // top right corner
    0xC0, // bottom left corner
    0xD9, // bottom right corner
    0xB4, // 3 dir. left
    0xC1, // 3 dir. up
    0xC3, // 3 dir. right
    0xC2, // 3 dir. down
    0xC5, // 4 directinal
};

typedef struct{
    char horizontal;
    char vertical;
    char top_left_corner;
    char top_right_corner;
    char bottom_left_corner;
    char bottom_right_corner;
    char three_directinal_left;
    char three_directinal_up;
    char three_directinal_right;
    char three_directinal_down;
    char four_directinal;    
}ASCII_FRAME_CHARSET;

const ASCII_FRAME_CHARSET ASCII_FRAME_CHARSET_THICK = {
    .horizontal              = ASCII_BOX_LINE_THICK[0],
    .vertical                = ASCII_BOX_LINE_THICK[1],
    .top_left_corner         = ASCII_BOX_LINE_THICK[2],
    .top_right_corner        = ASCII_BOX_LINE_THICK[3],
    .bottom_left_corner      = ASCII_BOX_LINE_THICK[4],
    .bottom_right_corner     = ASCII_BOX_LINE_THICK[5],
    .three_directinal_left   = ASCII_BOX_LINE_THICK[6],
    .three_directinal_right  = ASCII_BOX_LINE_THICK[7],
    .three_directinal_up     = ASCII_BOX_LINE_THICK[8],
    .three_directinal_down   = ASCII_BOX_LINE_THICK[9],
    .four_directinal         = ASCII_BOX_LINE_THICK[10]
};

const ASCII_FRAME_CHARSET ASCII_FRAME_CHARSET_THIN = {
    .horizontal              = ASCII_BOX_LINE_THIN[0],
    .vertical                = ASCII_BOX_LINE_THIN[1],
    .top_left_corner         = ASCII_BOX_LINE_THIN[2],
    .top_right_corner        = ASCII_BOX_LINE_THIN[3],
    .bottom_left_corner      = ASCII_BOX_LINE_THIN[4],
    .bottom_right_corner     = ASCII_BOX_LINE_THIN[5],
    .three_directinal_left   = ASCII_BOX_LINE_THIN[6],
    .three_directinal_right  = ASCII_BOX_LINE_THIN[7],
    .three_directinal_up     = ASCII_BOX_LINE_THIN[8],
    .three_directinal_down   = ASCII_BOX_LINE_THIN[9],
    .four_directinal         = ASCII_BOX_LINE_THIN[10]
};

/* ------------------ */

ascii_screen *ascii_screen_new(unsigned int width, unsigned int height);

void ascii_screen_delete(ascii_screen *screen);

void ascii_screen_display_buffer(ascii_screen* screen);

void ascii_screen_draw_sprite(ascii_screen *screen, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite);

void ascii_screen_fill_screen(ascii_screen *screen, char character, ASCII_COLOR_Typedef color);

void ascii_screen_draw_line(ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, char character, ASCII_COLOR_Typedef color);

void ascii_screen_draw_box(ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ASCII_LAYER_Typedef layers, char border_character, ASCII_COLOR_Typedef border_color, char fill_character, ASCII_COLOR_Typedef fill_color);

void ascii_screen_draw_frame(ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ASCII_FRAME_CHARSET char_set, ASCII_COLOR_Typedef color);

void ascii_screen_put_string(ascii_screen *screen, unsigned int x, unsigned int y, unsigned int size, char *string, ASCII_COLOR_Typedef color);

void ascii_screen_clear(ascii_screen *screen);


ascii_sprite **ascii_sprite_array_new(unsigned int width, unsigned int height, unsigned int files_qty, char **array, bool no_background, ASCII_COLOR_Typedef background_color, ASCII_COLOR_Typedef font_color);

void ascii_sprite_recolour(ascii_sprite *sprite, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, ASCII_LAYER_Typedef layers, ASCII_COLOR_Typedef color);

void ascii_sprite_array_recolour(ascii_sprite **sprite_array, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, ASCII_LAYER_Typedef layers, ASCII_COLOR_Typedef color);

void ascii_sprite_substitute_char(ascii_sprite *sprite, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char **char_matrix);

void ascii_sprite_array_substitute_char(ascii_sprite **sprite_array, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char **char_matrix);

void ascii_sprite_array_delete(ascii_sprite **sprite_array);

/* ------------------ */

struct ascii_screen{
    int width;
    int height;
    int size;
    CHAR_INFO *stream;
    CHAR_INFO **buffer;
    HANDLE hscreenbuffer;

    void (*Display_buffer)  (ascii_screen* screen);
    void (*Delete)          (ascii_screen *screen);
    void (*Draw_sprite)     (ascii_screen *screen, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite);
    void (*Fill_screen)     (ascii_screen *screen, char character, ASCII_COLOR_Typedef color);
    void (*Draw_line)       (ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, char character, ASCII_COLOR_Typedef color);
    void (*Draw_box)        (ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ASCII_LAYER_Typedef layers, char border_character, ASCII_COLOR_Typedef border_color, char fill_character, ASCII_COLOR_Typedef fill_color);
    void (*Draw_frame)      (ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ASCII_FRAME_CHARSET char_set, ASCII_COLOR_Typedef color);
    void (*Put_string)      (ascii_screen *screen, unsigned int x, unsigned int y, unsigned int size, char *string, ASCII_COLOR_Typedef color);
    void (*Clear)           (ascii_screen *screen);
} ;

struct ascii_sprite{
    int width;
    int height;
    int sprite_qty;
    WORD background_color;
    bool no_background;
    WORD font_color;
    CHAR_INFO **sprite;

    void (*Recolour)                (ascii_sprite *sprite, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, ASCII_LAYER_Typedef layers, ASCII_COLOR_Typedef color);
    void (*Array_recolour)          (ascii_sprite **sprite_array, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, ASCII_LAYER_Typedef layers, ASCII_COLOR_Typedef color);
    void (*Substitute_char)         (ascii_sprite *sprite, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char **char_matrix);
    void (*Array_substitute_char)   (ascii_sprite **sprite_array, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char **char_matrix);
    void (*Delete_array)            (ascii_sprite **sprite_array);
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
    scr->Draw_line = ascii_screen_draw_line;
    scr->Draw_box = ascii_screen_draw_box;
    scr->Draw_frame = ascii_screen_draw_frame;
    scr->Put_string = ascii_screen_put_string;
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

void ascii_screen_draw_sprite(ascii_screen *screen, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite){ 

    int delta_x, delta_y;

    switch(anchor){
        case ASCII_ANCHOR_TOP_LEFT:
            delta_x = x;
            delta_y = y;
            break;
            
        case ASCII_ANCHOR_TOP_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = y;
            break;
            
        case ASCII_ANCHOR_BOTTOM_LEFT:
            delta_x = x;
            delta_y = (y - sprite->height);
            break;
            
        case ASCII_ANCHOR_BOTTOM_RIGHT:
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

void ascii_screen_draw_line(ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, char character, ASCII_COLOR_Typedef color){
    int delta_x = x2 - x1;    
    int delta_y = y2 - y1;    
    float a = (float) delta_y / (float ) delta_x;
    float b = (float)y1 - a*(float)x1;

    for(int i = (x1 < x2 ? x1 : x2); i < delta_x; i++){
        screen->buffer[(int)(a*i + b)][i + (x1 < x2 ? x1 : x2)].Char.AsciiChar = character;        
        screen->buffer[(int)(a*i + b)][i + (x1 < x2 ? x1 : x2)].Attributes = color;        
    }
}

void ascii_screen_draw_box(ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ASCII_LAYER_Typedef layers, char border_character, ASCII_COLOR_Typedef border_color, char fill_character, ASCII_COLOR_Typedef fill_color){
    CHAR_INFO border_char = {.Char.AsciiChar = border_character, .Attributes = border_color};
    CHAR_INFO fill_char = {.Char.AsciiChar = fill_character, .Attributes = fill_color};

    for(int i = (y1 < y2 ? y1 : y2); i < ((y1 < y2 ? y2 : y1) + 1); i++){   
        for(int j = (x1 < x2 ? x1 : x2); j < ((x1 < x2 ? x2 : x1) + 1); j++){   
            if(i == y1 || i == y2 || j == x1 || j == x2){
                if(!(layers & ASCII_LAYER_FOREGROUND) && (layers & ASCII_LAYER_BACKGROUND)){
                    screen->buffer[i][j] = fill_char;
                }
                else if(layers & ASCII_LAYER_FOREGROUND){
                    screen->buffer[i][j] = border_char;
                }
            }
            else if(layers & ASCII_LAYER_BACKGROUND){
                screen->buffer[i][j] = fill_char;
            }
        }
    }
}

void ascii_screen_draw_frame(ascii_screen *screen, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, ASCII_FRAME_CHARSET char_set, ASCII_COLOR_Typedef color){

    CHAR_INFO frame_char = {.Attributes = color};

    int a = x1, b = x2;

    if( x1 > x2 ){
        x1 = x2;
        x2 = a;
    }

    if( y1 > y2 ){
        y1 = y2;
        y2 = b;
    }

    for(int i = y1; i <= y2; i++){   
        for(int j = x1; j <= x2; j++){   
            if( i == y1 && j == x1 ){
                frame_char.Char.AsciiChar = char_set.top_left_corner;
            }
            else if( i == y2 && j == x1 ){
                frame_char.Char.AsciiChar = char_set.bottom_left_corner;
            }
            else if( i == y1 && j == x2 ){
                frame_char.Char.AsciiChar = char_set.top_right_corner;
            }
            else if( i == y2 && j == x2 ){
                frame_char.Char.AsciiChar = char_set.bottom_right_corner;
            }
            else if( j == x1 || j == x2 ){
                frame_char.Char.AsciiChar = char_set.vertical;
            }
            else if( i == y1 || i == y2 ){
                frame_char.Char.AsciiChar = char_set.horizontal;
            }
            else
                continue;

            screen->buffer[i][j] = frame_char;
        }
    }
}

void ascii_screen_put_string(ascii_screen *screen, unsigned int x, unsigned int y, unsigned int size, char *string, ASCII_COLOR_Typedef color){
    for(int i = 0; i < size; i++){
        if(i >= 0 && i <= screen->width){
            if(string[i] == '\0')
                return;

            CHAR_INFO character = {.Attributes = color, .Char.AsciiChar = string[i]};
            screen->buffer[y][x+i] = character;
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




ascii_sprite **ascii_sprite_array_new(unsigned int width, unsigned int height, unsigned int files_qty, char **array, bool no_background, ASCII_COLOR_Typedef background_color, ASCII_COLOR_Typedef font_color){
    
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

        sprite_cursor->Recolour = ascii_sprite_recolour;
        sprite_cursor->Array_recolour = ascii_sprite_array_recolour;
        sprite_cursor->Substitute_char = ascii_sprite_substitute_char;
        sprite_cursor->Array_substitute_char = ascii_sprite_array_substitute_char;
        sprite_cursor->Delete_array = ascii_sprite_array_delete;
    }

    return sprite_array;
}

void ascii_sprite_recolour(ascii_sprite *sprite, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, ASCII_LAYER_Typedef layers, ASCII_COLOR_Typedef color){

    int delta_x, delta_y;

    switch(anchor){
        case ASCII_ANCHOR_TOP_LEFT:
            delta_x = x;
            delta_y = y;
            break;
            
        case ASCII_ANCHOR_TOP_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = y;
            break;
            
        case ASCII_ANCHOR_BOTTOM_LEFT:
            delta_x = x;
            delta_y = (y - sprite->height);
            break;
            
        case ASCII_ANCHOR_BOTTOM_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = (y - sprite->height);
            break;
    }


    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            
            if(((i + delta_y) >= 0 && (i + delta_y) < sprite->height) && ((j + delta_x) >= 0 && (j + delta_x) < sprite->width)){ // recolour inside the sprite
        
                if((sprite->sprite[i + delta_y][j + delta_x].Char.AsciiChar == ' ') && (layers|ASCII_LAYER_BACKGROUND)) 
                    sprite->sprite[i + delta_y][j + delta_x].Attributes = color;
                else if((sprite->sprite[i + delta_y][j + delta_x].Char.AsciiChar != ' ') && (layers|ASCII_LAYER_FONT))
                    sprite->sprite[i + delta_y][j + delta_x].Attributes = color;
                    
            }
        }
    }    
}

void ascii_sprite_array_recolour(ascii_sprite **sprite_array, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, ASCII_LAYER_Typedef layers, ASCII_COLOR_Typedef color){
    int sprite_qty = (*sprite_array)->sprite_qty;
    
    for(int k = 0; k < sprite_qty; k++){
        ascii_sprite_recolour(sprite_array[k], anchor, x, y, width, height, layers, color);
    }
}

void ascii_sprite_substitute_char(ascii_sprite *sprite, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char **char_matrix){

    int delta_x, delta_y;

    switch(anchor){
        case ASCII_ANCHOR_TOP_LEFT:
            delta_x = x;
            delta_y = y;
            break;
            
        case ASCII_ANCHOR_TOP_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = y;
            break;
            
        case ASCII_ANCHOR_BOTTOM_LEFT:
            delta_x = x;
            delta_y = (y - sprite->height);
            break;
            
        case ASCII_ANCHOR_BOTTOM_RIGHT:
            delta_x = (x - sprite->width);
            delta_y = (y - sprite->height);
            break;
    }


    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            
            if(((i + delta_y) >= 0 && (i + delta_y) < sprite->height) && ((j + delta_x) >= 0 && (j + delta_x) < sprite->width)){ // recolour inside the sprite
                    sprite->sprite[i + delta_y][j + delta_x].Char.AsciiChar = char_matrix[i][j];                    
            }
        }
    }    
}

void ascii_sprite_array_substitute_char(ascii_sprite **sprite_array, ASCII_ANCHOR_Typedef anchor, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char **char_matrix){
    int sprite_qty = (*sprite_array)->sprite_qty;

    for(int k = 0; k < sprite_qty; k++){
        ascii_sprite_substitute_char(sprite_array[k], anchor, x, y, width, height, char_matrix);
    }
}

void ascii_sprite_array_delete(ascii_sprite **sprite_array){
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


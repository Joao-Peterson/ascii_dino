#include <stdlib.h>
#include <stdarg.h>
#include <Windows.h>
#include <string.h>

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

/* ------------------ */

ascii_screen *ascii_screen_new(unsigned int width, unsigned int height);

void ascii_screen_delete(ascii_screen *screen);

void ascii_screen_display_buffer(ascii_screen* screen);

void ascii_screen_draw_sprite(ascii_screen *screen, ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite);

void ascii_screen_clear(ascii_screen *screen);

ascii_sprite **ascii_sprite_new(unsigned int width, unsigned int height, unsigned int files_qty, ...);

void ascii_sprite_delete(ascii_sprite *sprite_array);

/* ------------------ */

struct ascii_screen{
    int w;
    int h;
    int size;
    char *buffer_stream;
    char **buffer_mat;
    HANDLE hscreenbuffer;

    void (*Display_buffer) (ascii_screen* screen);
    void (*Delete) (ascii_screen *screen);
    void (*Draw_sprite) (ascii_screen *screen, ANCHOR_Typedef anchor, unsigned int x, unsigned int y, ascii_sprite *sprite);
    void (*Clear) (ascii_screen *screen);
} ;

struct ascii_sprite{
    int width;
    int height;
    int s_qty;
    char** sprite_mat;

    void (*Delete) (ascii_sprite *sprite_array);
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

    char *buffer_stream = (char*)calloc(b_height * b_width + 1, sizeof(char)); // char string stream
    memset(buffer_stream, 0x20, width * height);

    char **buffer_mat = (char**)calloc(b_height, sizeof(char*)); // pointer array for acessing the stream

    for(int i = 0; i < b_height; i++) // pointers of the array
        buffer_mat[i] = buffer_stream + (i * b_width);

    HANDLE hscreenbuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    SetConsoleActiveScreenBuffer(hscreenbuffer);
    SetConsoleScreenBufferSize(hscreenbuffer, b_size);
    SetConsoleWindowInfo(hscreenbuffer, TRUE, &w_size);
    SetConsoleMode(hscreenbuffer, ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_PROCESSED_OUTPUT);

    scr->w = width;
    scr->h = height;
    scr->size = width * height;
    scr->hscreenbuffer = hscreenbuffer;
    scr->buffer_stream = buffer_stream;
    scr->buffer_mat = buffer_mat;
    scr->Delete = ascii_screen_delete;
    scr->Display_buffer = ascii_screen_display_buffer;
    scr->Draw_sprite = ascii_screen_draw_sprite;
    scr->Clear = ascii_screen_clear;

    return scr;
}

void ascii_screen_delete(ascii_screen *screen){
    CloseHandle(screen->hscreenbuffer);

    free(screen->buffer_mat);
    free(screen->buffer_stream);
    free(screen);
}

void ascii_screen_display_buffer(ascii_screen* screen){
    unsigned long a;
    screen->buffer_stream[screen->size] = '\0';
    WriteConsoleOutputCharacterA(screen->hscreenbuffer, screen->buffer_stream, screen->size, (COORD){0,0}, &a);
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
            
            if(((i + delta_y) >= 0 && (i + delta_y) < screen->h) && ((j + delta_x) >= 0 && (j + delta_x) < screen->w)) // inside the screen
                if(sprite->sprite_mat[i][j] != ' ') // when ' ', space, dont print
                    screen->buffer_mat[i + delta_y][j + delta_x] = sprite->sprite_mat[i][j];

        }
    }    
}

void ascii_screen_clear(ascii_screen *screen){
    memset(screen->buffer_stream, 0x20, screen->size);
}



ascii_sprite **ascii_sprite_new(unsigned int width, unsigned int height, unsigned int files_qty, ...){
    va_list args;
    va_start(args,files_qty);

    ascii_sprite **sprite_array = (ascii_sprite**)calloc(files_qty,sizeof(ascii_sprite*));
    ascii_sprite *sprite_cursor;

    char *sprite_string;

    for(int i = 0; i < files_qty; i++){ // for each file
        
        // alocate the sprite struct
        sprite_array[i] = (ascii_sprite*)calloc(1,sizeof(ascii_sprite));
        sprite_cursor = sprite_array[i];
        

        sprite_string = va_arg(args, char*);

        sprite_cursor->sprite_mat = (char**)calloc(height, sizeof(char*));

        for(int j = 0; j < height; j ++){ // for each line
        
            sprite_cursor->sprite_mat[j] = (char*)calloc(width, sizeof(char));

            int is_inline = 1;
            int is_insprite = 1;

            for(int k = 0; k < width; k++){ // for each column
                if( !is_insprite ){
                    sprite_cursor->sprite_mat[j][k] = ' ';
                }
                else if( (sprite_string[k] != '\n' && sprite_string[k] != '\0') && is_inline){ // reads until a '\n' or '\0' occur, in this case fill with ' ' spaces;
                    sprite_cursor->sprite_mat[j][k] = sprite_string[k];
                }
                else if( sprite_string[k] == '\0' ){
                    sprite_cursor->sprite_mat[j][k] = ' ';
                    is_inline = 0;
                    is_insprite = 0;
                }
                else{
                    sprite_cursor->sprite_mat[j][k] = ' ';
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

        sprite_cursor->s_qty = files_qty;
        sprite_cursor->width = width;
        sprite_cursor->height = height;
        sprite_cursor->Delete = ascii_sprite_delete;
    }

    va_end(args);

    return sprite_array;
}

void ascii_sprite_delete(ascii_sprite *sprite_array){
    ascii_sprite *cursor;
    for (int i = 0; i < sprite_array->s_qty; i++){
        cursor = sprite_array + i;

        for(int j = 0; j < cursor->height; j++){
            free(cursor->sprite_mat[j]);    
        }
        
        free(cursor->sprite_mat);
        free(cursor);
    }
}

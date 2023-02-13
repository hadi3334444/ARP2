#include "./../include/processA_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <bmpfile.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
//size of shared memory object in bytes
const int SIZE = 1600*600*sizeof(int);
bmpfile_t *bitmap;
rgb_pixel_t pixel = {0, 0, 100, 0};
//shared memory object name
const char* name = "shm";
//file descriptor of shared memory
int fd;
//shared memory object pointer
int* ptr;
//a flag to save only if this is true
bool flag_to_save = false;
//variables to create bitmap, so we initialize here to avoid repeating inside the function 
int width = 1600;
int height = 600;
int depth = 4;
//function to update shared memory each time the object is moved
void update_shared_memory(bmpfile_t *bitmap)
{
    for(int i = 0; i < 1600; i++)
    {
        for(int j = 0; j < 600; j++)
        {
         rgb_pixel_t *pixel=bmp_get_pixel(bitmap ,i, j);
         ptr[i*599+j]= pixel->red;
        }
    }
}

//function to create bitmap and save the bitmap if p is pressed
void bitmap_create(int i_circle, int j_circle)
{
    bitmap = bmp_create(width, height, depth);
    int radius =30;
    for(int i = -radius; i <= radius; i++) 
    {
        for(int j = -radius; j <= radius; j++) 
        {
            //checking if the point is within the circle
            if(sqrt(pow(i,2) + pow(j,2)) < radius) {
            //color circle
            bmp_set_pixel(bitmap, (i_circle)*(width/(COLS - BTN_SIZE_X -2)) + i, (j_circle)*(height/LINES) + j, pixel);
            }
        }
    }
    if(flag_to_save == true)
    {
        //save the bitmap file with the name of printed
	bmp_save(bitmap,"out/printed" );
	/*set the flag to false so that next time it is called does not
	 change the saved bitmap*/
	flag_to_save = false;
    }
}

int main(int argc, char *argv[])
{
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    // configure the size of the shared memory object
    ftruncate(fd, SIZE);
    //Memory map shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);

    // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

    // Initialize UI
    init_console_ui();
    bitmap_create(circle.x, circle.y);
    //update shared memory
    update_shared_memory(bitmap);

    // Infinite loop
    while (TRUE)
    {
        // Get input in non-blocking mode
        int cmd = getch();
        // If user resizes screen, re-draw UI...
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }
        // Else, if user presses print button
        else if(cmd == KEY_MOUSE) {
            if(getmouse(&event) == OK) {
                if(check_button_pressed(print_btn, &event)) {
                    //put flag to true to save bitmap
                    flag_to_save = true;
                    //create bitmap and update shared memory
                    bitmap_create(circle.x, circle.y);
		     //update shared memory  
		     update_shared_memory(bitmap);
                    mvprintw(LINES - 1, 1, "Print button pressed");
                    refresh();
                    sleep(1);
                    for(int j = 0; j < COLS - BTN_SIZE_X - 2; j++) {
                        mvaddch(LINES - 1, j, ' ');
                    }
                }
            }
        }
        // If input is an arrow key, move circle accordingly...
        else if(cmd == KEY_LEFT || cmd == KEY_RIGHT || cmd == KEY_UP || cmd == KEY_DOWN) {
            move_circle(cmd);
            draw_circle();
            //create bitmap and update shared memory
            bitmap_create(circle.x, circle.y);
            //update shared memory
            update_shared_memory(bitmap);
        }
    }    
    endwin();
    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xcb/xcb.h>

#include "config.h"

int main(int argc, char *argv[]) {

    // connect X-Server
    int screen_number = -1;
    xcb_connection_t *xcb_connection = xcb_connect(NULL, &screen_number);
    if (screen_number < 0) {
        fprintf(stderr, "ERROR: while connecting to x-server\n");
        return screen_number;
    }

    // get last screen
    const xcb_setup_t *xcb_setup = xcb_get_setup(xcb_connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_setup);
    for (int i=0; i < screen_number; ++i) {
        xcb_screen_next(&iter);
    }
    xcb_screen_t *xcb_screen = iter.data;

    // get first screen
    /* xcb_screen_t xcb_screen = xcb_setup_roots_iterator(xcb_get_setup(xcb_connection)).data; */

    // report screen information
    printf("dimension: %dx%d\n", xcb_screen->width_in_pixels, xcb_screen->height_in_pixels);

    // create window
    xcb_window_t xcb_window = xcb_generate_id(xcb_connection);
    xcb_create_window(xcb_connection, XCB_COPY_FROM_PARENT, xcb_window, xcb_screen->root, 0, 0, 150, 150, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, xcb_screen->root_visual, 0, NULL);
    xcb_map_window(xcb_connection, xcb_window);
    xcb_flush(xcb_connection);

    pause();

    // disconnect X-Server
    xcb_disconnect(xcb_connection);



    return 0;
}

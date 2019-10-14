void monocle() {
    int temp_border_width = wm_global.border_width;
    wm_global.border_width = 0;
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        wm_client_draw(client, 0, 0, wm_global.screen_width, wm_global.screen_height);
    }
    wm_global.border_width = temp_border_width;
}

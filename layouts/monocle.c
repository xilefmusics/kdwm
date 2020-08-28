void monocle() {
    wm_client_t *client = NULL;
    while (client = wm_client_get_next(client)){
        wm_client_draw(client, wm_global.x, wm_global.y, wm_global.w, wm_global.h, false);
    }
}

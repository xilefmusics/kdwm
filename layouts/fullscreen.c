void fullscreen(int x, int y, int w, int h) {
  wm_client_t *client = NULL;
  while (client = wm_client_get_next(client))
    wm_client_draw(client, x-2*GAPS-OFFSET_LEFT, y-2*GAPS-OFFSET_TOP, w+4*GAPS+OFFSET_LEFT+OFFSET_RIGHT, h+4*GAPS+OFFSET_TOP+OFFSET_BOTTOM, false);
}

void wm_retag(int tag_mask) {
  for (int i = 0; i < LENGTH(wm_on_retag); i++)
    wm_on_retag[i](tag_mask);
  if (tag_mask == wm_global.tag_mask)
    return;
  wm_clients_unmap(wm_global.tag_mask);
  wm_global.tag_mask = tag_mask;
  if (!wm_global.client_list.active_client || wm_global.client_list.active_client && !(wm_global.client_list.active_client->tag_mask & tag_mask))
    wm_global.client_list.active_client = wm_client_get_next(NULL);
  wm_clients_map();
  wm_clients_arrange();
  wm_client_focus(wm_global.client_list.active_client);
}

void wm_focus_next() {
  wm_client_t *client = wm_global.client_list.active_client;
  wm_client_focus(wm_client_get_next(wm_global.client_list.active_client));
  wm_client_set_border_color(client);
  wm_client_set_border_color(wm_global.client_list.active_client);
}

void wm_focus_prev() {
  wm_client_t *client = wm_global.client_list.active_client;
  wm_client_focus(wm_client_get_prev(wm_global.client_list.active_client));
  wm_client_set_border_color(client);
  wm_client_set_border_color(wm_global.client_list.active_client);
}

void wm_kill_active_client() {
  if (wm_global.client_list.active_client)
    wm_client_send_XEvent(wm_global.client_list.active_client, wm_global.atoms[WM_DELETE_WINDOW]);
}

void wm_spawn(char *name) {
  char buffer[256];
  strcpy(buffer, name);
  strcat(buffer, " &");
  system(buffer);
}

void wm_set_tag_mask_of_focused_client(int tag_mask){
  if (wm_global.client_list.active_client && !(tag_mask & wm_global.tag_mask)) {
    wm_global.client_list.active_client->tag_mask = tag_mask;
    XUnmapWindow(wm_global.display, wm_global.client_list.active_client->window);
    wm_client_find_new_focus(wm_global.client_list.active_client);
    wm_clients_arrange();
  }
}

void wm_add_tag_to_tag_mask(int tag) {
  for (int i = 0; i < LENGTH(wm_on_add_tag_to_tag_mask); i++)
    wm_on_add_tag_to_tag_mask[i](tag);
  wm_global.tag_mask = wm_global.tag_mask | tag;
  wm_retag(wm_global.tag_mask);
}

void wm_add_tag_to_client(int tag) {
  if (wm_global.client_list.active_client)
    wm_global.client_list.active_client->tag_mask = wm_global.client_list.active_client->tag_mask | tag;
}

void wm_rehead() {
  wm_client_rehead(wm_global.client_list.active_client);
  wm_clients_arrange();
}

void wm_client_up() {
  wm_client_t *prev = wm_client_get_prev(wm_global.client_list.active_client);
  if (prev) {
    wm_client_swap(prev, wm_global.client_list.active_client);
    wm_clients_arrange();
  }
}

void wm_client_down() {
  wm_client_t *next = wm_client_get_next(wm_global.client_list.active_client);
  if (next) {
    wm_client_swap(wm_global.client_list.active_client, next);
    wm_clients_arrange();
  }
}

void wm_change_layout(int layout) {
  wm_global.current_layout = layout;
  wm_clients_arrange();
}

void wm_change_master_width(int percent) {
  wm_global.master_width = wm_global.master_width + percent;
  wm_global.master_width = wm_global.master_width > 5 ? wm_global.master_width : 5;
  wm_global.master_width = wm_global.master_width < 95 ? wm_global.master_width : 95;
  wm_global.master_width = percent ? wm_global.master_width : MASTER_WIDTH;
  wm_clients_arrange();
}


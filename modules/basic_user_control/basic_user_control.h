void wm_retag(int tag_mask);
void wm_focus_next();
void wm_focus_prev();
void wm_kill_active_client();
void wm_spawn(char *name);
void wm_set_tag_mask_of_focused_client(int tag_mask);
void wm_add_tag_to_tag_mask(int tag);
void wm_add_tag_to_client(int tag);
void wm_rehead();
void wm_client_up();
void wm_client_down();
void wm_change_layout(int layout);
void wm_change_master_width(int percent);
void move_tag_mask_to_next_monitor();
void move_tag_mask_to_prev_monitor();

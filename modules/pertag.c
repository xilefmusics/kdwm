static int pertag_layouts[NUM_OF_TAGS+1] = {0};
static int pertag_master_widths[NUM_OF_TAGS+1] = {0};
static int pertag_window_of_active_client[NUM_OF_TAGS+1] = {0};

int pertag_get_index(int tag_mask) {
    int index = -1;
    while (tag_mask && index < NUM_OF_TAGS) {
        ++index;
        tag_mask = tag_mask >> 1;
    }
    return index;
}

void pertag_configure(int tag_mask) {
    //store and restore layout
    pertag_layouts[pertag_get_index(wm_global.tag_mask)] = wm_global.current_layout;
    wm_global.current_layout = pertag_layouts[pertag_get_index(tag_mask)];
    // store and restore master width
    pertag_master_widths[pertag_get_index(wm_global.tag_mask)] = wm_global.master_width;
    wm_global.master_width = pertag_master_widths[pertag_get_index(tag_mask)];
    if (wm_global.master_width == 0) {
        wm_global.master_width = MASTER_WIDTH;
    }
    // store and restore active client (store window because of dangling pointers)
    if (wm_global.client_list.active_client) {
        pertag_window_of_active_client[pertag_get_index(wm_global.tag_mask)] = wm_global.client_list.active_client->window;
    }
    wm_global.client_list.active_client = wm_client_find(pertag_window_of_active_client[pertag_get_index(tag_mask)]);
}

void pertag_retag(int tag_mask) {
    pertag_configure(tag_mask);
    wm_retag(tag_mask);
}

void pertag_add_tag_to_tag_mask(int tag_mask) {
    pertag_configure(tag_mask);
    wm_add_tag_to_tag_mask(tag_mask);
}

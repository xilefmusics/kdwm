static int pertag_layouts[NUM_OF_TAGS] = {0};
static int pertag_master_widths[NUM_OF_TAGS] = {0};
static wm_client_t *pertag_active_client[NUM_OF_TAGS] = {0};

int pertag_get_index(int tag_mask) {
    int index = 0;
    while (tag_mask && index < NUM_OF_TAGS) {
        ++index;
        tag_mask = tag_mask >> 1;
    }
}

void pertag_configure(int tag_mask) {
    pertag_layouts[pertag_get_index(wm_global.tag_mask)] = wm_global.current_layout;
    pertag_master_widths[pertag_get_index(wm_global.tag_mask)] = wm_global.master_width;
    pertag_active_client[pertag_get_index(wm_global.tag_mask)] = wm_global.client_list.active_client;
    wm_global.current_layout = pertag_layouts[pertag_get_index(tag_mask)];
    wm_global.master_width = pertag_master_widths[pertag_get_index(tag_mask)];
    wm_global.client_list.active_client = pertag_active_client[pertag_get_index(tag_mask)];
    if (wm_global.master_width == 0) {
        wm_global.master_width = MASTER_WIDTH;
    }
}

void pertag_retag(int tag_mask) {
    pertag_configure(tag_mask);
    wm_retag(tag_mask);
}

void pertag_set_tag_mask_of_focused_client(int tag_mask) {
    pertag_configure(tag_mask);
    wm_set_tag_mask_of_focused_client(tag_mask);
}

void pertag_add_tag_to_tag_mask(int tag_mask) {
    pertag_configure(tag_mask);
    wm_add_tag_to_tag_mask(tag_mask);
}

void pertag_add_tag_to_client(int tag_mask) {
    pertag_configure(tag_mask);
    wm_add_tag_to_client(tag_mask);
}

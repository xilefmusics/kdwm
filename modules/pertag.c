static int pertag_layouts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
static int pertag_master_widths[9] = {MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH, MASTER_WIDTH};

int pertag_get_index(int tag_mask) {
    switch (tag_mask) {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 16: return 4;
        case 32: return 5;
        case 64: return 6;
        case 128: return 7;
        case 256: return 8;
        default: return 9;
    }
}

void pertag_configure(int tag_mask) {
    pertag_layouts[pertag_get_index(wm_global.tag_mask)] = wm_global.current_layout;
    pertag_master_widths[pertag_get_index(wm_global.tag_mask)] = wm_global.master_width;
    wm_global.current_layout = pertag_layouts[pertag_get_index(tag_mask)];
    wm_global.master_width = pertag_master_widths[pertag_get_index(tag_mask)];
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

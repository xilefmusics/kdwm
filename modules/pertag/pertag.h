int pertag_layouts[NUM_OF_TAGS+1] = {0};
int pertag_master_widths[NUM_OF_TAGS+1] = {0};
int pertag_window_of_active_client[NUM_OF_TAGS+1] = {0};

int pertag_get_index(int tag_mask);
void pertag_configure(int tag_mask);
void pertag_retag(int tag_mask);
void pertag_add_tag_to_tag_mask(int tag_mask);



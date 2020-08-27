typedef struct multimon_monitor {
    int x, y, w, h;
    int tag_mask;
    int active_tag_mask;
    struct multimon_monitor *next;
    struct multimon_monitor *prev;
} multimon_monitor_t;

typedef struct multimon_monitor_list {
    multimon_monitor_t *head_monitor;
    int size;
} multimon_monitor_list_t;
multimon_monitor_list_t multimon_monitor_list;

multimon_monitor_t *multimon_get_monitor(int tag_mask);
void multimon_monitor_update();
void multimon_init();

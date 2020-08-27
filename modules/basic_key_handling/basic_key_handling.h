typedef enum {NONE, STRING, INTEGER} basic_key_handling_arg_types_t;

typedef struct basic_key_handling_keybinding {
    int mod;
    int keysym;
    void (*func)();
    basic_key_handling_arg_types_t arg_type;
    char *arg;
} basic_key_handling_keybinding_t;

void basic_key_handling_on_key_press(XEvent *event);
void basic_key_handling_keys_grap();
void basic_key_handling_init();

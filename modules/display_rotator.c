enum display_rotator_state_t {NORMAL, NORMAL2, LEFT, LEFT2, INVERTED, INVERTED2,  RIGHT, RIGHT2};
enum display_rotator_state_t display_rotator_state = NORMAL;

void rotate_display(char *display) {
    char command[128];
    strcpy(command, "xrandr --output ");
    strcat(command, display);
    strcat(command, " --rotate ");
    switch (display_rotator_state) {
        case NORMAL:
            display_rotator_state = NORMAL2;
            return;
        case NORMAL2:
            display_rotator_state = LEFT;
            strcat(command, "left");
            break;
        case LEFT:
            display_rotator_state = LEFT2;
            return;
        case LEFT2:
            display_rotator_state = INVERTED;
            strcat(command, "inverted");
            break;
        case INVERTED:
            display_rotator_state = INVERTED2;
            return;
        case INVERTED2:
            display_rotator_state = RIGHT;
            strcat(command, "right");
            break;
        case RIGHT:
            display_rotator_state = RIGHT2;
            return;
        case RIGHT2:
            display_rotator_state = NORMAL;
            strcat(command, "normal");
            break;
    }
    system(command);
}

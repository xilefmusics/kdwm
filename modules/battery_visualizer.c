XColor battery_visualizer_colors[10];

void battery_visualizer_init() {
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#ff0000", &battery_visualizer_colors[0], &battery_visualizer_colors[0]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#ff3300", &battery_visualizer_colors[1], &battery_visualizer_colors[1]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#ff6600", &battery_visualizer_colors[2], &battery_visualizer_colors[2]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#ff9900", &battery_visualizer_colors[3], &battery_visualizer_colors[3]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#ffCC00", &battery_visualizer_colors[4], &battery_visualizer_colors[4]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#ffff00", &battery_visualizer_colors[5], &battery_visualizer_colors[5]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#00ff00", &battery_visualizer_colors[6], &battery_visualizer_colors[6]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#00cc00", &battery_visualizer_colors[7], &battery_visualizer_colors[7]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#006600", &battery_visualizer_colors[8], &battery_visualizer_colors[8]);
    XAllocNamedColor(wm_global.display, wm_global.colormap, "#003300", &battery_visualizer_colors[9], &battery_visualizer_colors[9]);
}


void show_battery() {
    FILE *fp = fopen("/sys/class/power_supply/BAT0/capacity", "r");
    if (!fp || !wm_global.client_list.active_client) {
        return;
    }
    int percentage;
    fscanf(fp, "%d", &percentage);
    fclose(fp);
    int index = 0;
    if (percentage == 100) {
        index = 9;
    } else {
        index = (int) (percentage / 10);
    }
    wm_client_t *client = wm_global.client_list.active_client;
    logsi("x", client->x);
    logsi("y", client->y);
    logsi("w", client->w);
    logsi("h", client->h);
    wm_client_draw(client, client->x, client->y, client->w, client->h, true);
    XColor temp = wm_global.border_color_active;
    wm_global.border_color_active = battery_visualizer_colors[index];
    wm_client_set_border_color(client);
    wm_global.border_color_active = temp;
}

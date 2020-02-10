long get_brightness()
{
    FILE *fp;
    char prog_out[10];
    long br;

    // get brightness
    fp = popen("/usr/bin/brightnessctl g", "r");

    if (fp == NULL) {
        br = 1024;
    }

    if(fgets(prog_out, sizeof(prog_out), fp) != NULL) {
        br = strtol(prog_out, NULL, 10);
    }
    else {
        br = 1024;
    }

    pclose(fp);

    return br;
}

void brightness_increase()
{
    long brigthness = get_brightness();

    if(brigthness < 86) {
        system("brightnessctl set +2%");
    }
    else if(brigthness < 256) {
        system("brightnessctl set +5%");
    }
    else {
        system("brightnessctl set +10%");
    }
}

void brigthness_decrease()
{
    long brigthness = get_brightness();

    if(brigthness < 86) {
        system("brightnessctl set 2%- -n");
    }
    else if(brigthness < 256) {
        system("brightnessctl set 5%- -n");
    }
    else {
        system("brightnessctl set 10%- -n");
    }
}
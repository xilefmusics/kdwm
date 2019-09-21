#include "config.h"
#include "window_manager.h"





int main(int argc, char *argv[]) {
    wm_init();
    wm_start();
    wm_tini();

    return 0;
}

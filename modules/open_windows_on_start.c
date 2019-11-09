void open_windows_on_start() {
    Window root_return, parent_return, *children_return;
    unsigned int nchildren_return;
    bool client_added = false;
    if (XQueryTree(wm_global.display, wm_global.root_window, &root_return, &parent_return, &children_return, &nchildren_return)) {
        for (int i = 0; i < nchildren_return; i++) {
            wm_client_add(children_return[i]);
            client_added = true;
        }
        if (client_added) {
            wm_clients_arrange();
            wm_client_focus(wm_global.client_list.head_client);
        }
    }
}

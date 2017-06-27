#include "sqlite_module.h"

#include <sqlite3.h>

bool
plugin_call(ModuleContext *context) {
    sqlite3 *c;

    if (sqlite3_open(":memory:", &c) == 0) {
        sqlite3_close(c);
    }
    return true;
}
const char *
plugin_name() {
    return "sqlite_plugin_module";
}

double
plugin_version() {
    return 1.02;
}
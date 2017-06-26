#include <plugin_api.h>
#include <dynamic_library_module.h>

extern "C" {
bool
plugin_call(ModuleContext *context) {
    return true;
}

const char *
plugin_name() {
    return "dynamic_module_library";
}

double
plugin_version() {
    return 1.02;
}

}

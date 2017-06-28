#include <plugin_api.h>
#include <dynamic_library_module.h>

extern "C" {
__declspec(dllexport) bool
plugin_call(ModuleContext *context) {
    return true;
}

__declspec(dllexport) const char *
plugin_name() {
    return "dynamic_module_library";
}

__declspec(dllexport) double
plugin_version() {
    return 1.02;
}

__declspec(dllexport) PluginSpec::ModuleType
plugin_type() {
    return PluginSpec::DynamicLibrary;
}

__declspec(dllexport) PluginSpec::ModuleStage
plugin_stage() {
    return PluginSpec::FirstInput;
}

}

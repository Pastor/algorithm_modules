#include <windows.h>
#include "dynamic_library_module.h"

extern "C" {
typedef bool (__stdcall *plugin_call_fun)(ModuleContext *context);
typedef const char *(__stdcall *plugin_name_fun)();
typedef double (__stdcall *plugin_version_fun)();
}

struct DynamicLibraryModulePrivate {
    DynamicLibraryModulePrivate(const std::string &library_path)
            : h_library(LoadLibraryA(library_path.c_str())) {
        if (is_loaded()) {
            plugin_name = (plugin_name_fun)GetProcAddress(h_library, "plugin_name");
            plugin_call = (plugin_call_fun)GetProcAddress(h_library, "plugin_call");
            plugin_version = (plugin_version_fun)GetProcAddress(h_library, "plugin_version");
        }
    }

    ~DynamicLibraryModulePrivate() {
        if (h_library != nullptr)
            FreeLibrary(h_library);
        h_library = nullptr;
        plugin_name = nullptr;
        plugin_call = nullptr;
        plugin_version = nullptr;
    }

    bool is_loaded() const {
        return h_library != nullptr && h_library != INVALID_HANDLE_VALUE;
    }

    std::string name() const {
        return plugin_name == nullptr ? "unloaded" : (*plugin_name)();
    }

    double version() const {
        return plugin_version == nullptr ? 0.0 : (*plugin_version)();
    }

    bool execute(std::shared_ptr<ModuleContext> context) {
        if (plugin_call != nullptr && *context) {
            return (*plugin_call)(context.get());
        }
        return false;
    }
private:
    HMODULE h_library = nullptr;
    plugin_call_fun    plugin_call = nullptr;
    plugin_name_fun    plugin_name = nullptr;
    plugin_version_fun plugin_version = nullptr;
};


DynamicLibraryModule::DynamicLibraryModule(const std::string &library_path)
        : d(new DynamicLibraryModulePrivate(library_path)) {

}

DynamicLibraryModule::~DynamicLibraryModule() {}

bool
DynamicLibraryModule::execute(std::shared_ptr<ModuleContext> context) {
    return d->execute(context);
}

bool
DynamicLibraryModule::is_loaded() const {
    return d->is_loaded();
}

std::string
DynamicLibraryModule::name() const {
    return d->name();
}

double
DynamicLibraryModule::version() const {
    return d->version();
}

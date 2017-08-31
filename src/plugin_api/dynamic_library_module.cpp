#include <windows.h>
#include "dynamic_library_module.h"

extern "C" {
typedef bool (__stdcall *plugin_call_fun)(ModuleContext *context);
typedef const char *(__stdcall *plugin_name_fun)();
typedef double (__stdcall *plugin_version_fun)();
typedef PluginSpec::ModuleStage (__stdcall *plugin_stage_fun)();
typedef PluginSpec::ModuleType (__stdcall *plugin_type_fun)();
}

struct DynamicLibraryModulePrivate {
    explicit DynamicLibraryModulePrivate(const std::string &library_path)
            : h_library(LoadLibraryA(library_path.c_str())) {
        if (is_loaded()) {
            plugin_name = (plugin_name_fun)GetProcAddress(h_library, "plugin_name");
            plugin_call = (plugin_call_fun)GetProcAddress(h_library, "plugin_call");
            plugin_version = (plugin_version_fun)GetProcAddress(h_library, "plugin_version");
            plugin_stage = (plugin_stage_fun)GetProcAddress(h_library, "plugin_stage");
            plugin_type = (plugin_type_fun)GetProcAddress(h_library, "plugin_type");

            _spec.plugin_version = version();
            _spec.plugin_type = PluginSpec::DynamicLibrary;
            _spec.plugin_name = name();
            _spec.plugin_file_path = library_path;
            _spec.plugin_type = plugin_type != nullptr ? (*plugin_type)() : PluginSpec::UnknownModule;
            _spec.plugin_stage = plugin_stage != nullptr ? (*plugin_stage)() : PluginSpec::UnknownInput;
        }
    }
    DynamicLibraryModulePrivate() = delete;
    DynamicLibraryModulePrivate(const DynamicLibraryModulePrivate &) = delete;
    DynamicLibraryModulePrivate(const DynamicLibraryModulePrivate &&) = delete;
    DynamicLibraryModulePrivate &operator=(const DynamicLibraryModulePrivate &) = delete;

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

    bool execute(const std::shared_ptr<ModuleContext> &context) {
        if (plugin_call != nullptr && context) {
            return (*plugin_call)(context.get());
        } else {
            fprintf(stdout, "Plugin.Call not found in library\n");
        }
        return false;
    }

    const PluginSpec &spec() const {
        return _spec;
    }

private:
    HMODULE h_library = nullptr;
    plugin_call_fun    plugin_call = nullptr;
    plugin_name_fun    plugin_name = nullptr;
    plugin_version_fun plugin_version = nullptr;
    plugin_type_fun    plugin_type = nullptr;
    plugin_stage_fun   plugin_stage = nullptr;
    PluginSpec         _spec;
};


DynamicLibraryModule::DynamicLibraryModule(const std::string &library_path)
        : d(new DynamicLibraryModulePrivate(library_path)) {

}

DynamicLibraryModule::~DynamicLibraryModule() = default;

bool
DynamicLibraryModule::execute(std::shared_ptr<ModuleContext> context) {
    return d->execute(context);
}

bool
DynamicLibraryModule::is_loaded() const {
    return d->is_loaded();
}

const PluginSpec &
DynamicLibraryModule::spec() const {
    return d->spec();
}

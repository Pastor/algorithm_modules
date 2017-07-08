#include <windows.h>
#include "system_process_module.h"

struct SystemProcessModulePrivate {

};

SystemProcessModule::SystemProcessModule(const std::string &file_path, const std::string &name,
                                         const std::string &description, double version)
        : DefaultModule(PluginSpec::SystemProcess, name, description, version, file_path) {

}

SystemProcessModule::~SystemProcessModule() {

}

static char * parse_environment(std::shared_ptr<ModuleContext> context) {
    auto current_environment = GetEnvironmentStrings();
    std::map<std::string, std::string> environment;

    if (context->bool_value(Constants::SystemProcessModule_PutEnvironment)) {
        for (auto it = current_environment; *it != 0; it += strlen(it) + 1) {
            auto element = std::string(it);
            auto index = element.find('=');
            if (index == std::string::npos) {
                environment[element] = "";
            } else {
                environment[element.substr(0, index)] = element.substr(index + 1);
            }
        }
    }

    if (current_environment != nullptr)
        FreeEnvironmentStrings(current_environment);

    for (auto it = context->entries().begin(); it != context->entries().end(); ++it) {
        environment[std::string("CONTEXT_") + it->first] = it->second;
    }

    if (context->property(Constants::Debug_ModuleLevel) == "Debug") {
        for (auto it = environment.begin(); it != environment.end(); ++it) {
            fprintf(stdout, "%s = %s\n", it->first.c_str(), it->second.c_str());
        }
    }
    return nullptr;
}

bool
SystemProcessModule::execute(std::shared_ptr<ModuleContext> context) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    bool ret;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    parse_environment(context);
    SetLastError(0);
    auto file_path_name = (char *)spec().plugin_file_path.c_str();
    fprintf(stdout, "[SystemProcessModule] Execute \"%s\"\n", file_path_name);
    ret = CreateProcessA(nullptr, file_path_name, nullptr, nullptr,
                        FALSE, 0, nullptr, nullptr, &si, &pi) == TRUE;
    if (ret) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    } else {
        fprintf(stderr, "[SystemProcessModule] CreateProcess error: %d\n", (unsigned int)GetLastError());
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return ret;
}

#include <windows.h>
#include <strsafe.h>
#include "system_process_module.h"

struct SystemProcessModulePrivate {

};

SystemProcessModule::SystemProcessModule(const std::string &file_path, const std::string &name,
                                         const std::string &description, double version)
        : DefaultModule(PluginSpec::SystemProcess, name, description, version, file_path) {

}

SystemProcessModule::~SystemProcessModule() = default;

char *
SystemProcessModule::parse_environment(const std::shared_ptr<ModuleContext> &context) {
    auto current_environment = GetEnvironmentStrings();
    std::map<std::string, std::string> environment;

    if (context->bool_value(Constants::SystemProcessModule_PutEnvironment)) {
        for (auto it = current_environment; *it != 0; it += strlen(it) + 1) {
            auto element = std::string(it);
            auto index = element.find('=');
            if (index != std::string::npos) {
                environment[element.substr(0, index)] = element.substr(index + 1);
            }
        }
    }

    if (current_environment != nullptr)
        FreeEnvironmentStrings(current_environment);

    for (const auto &it : context->entries()) {
        environment[std::string("CONTEXT_") + it.first] = it.second;
    }

    if (context->property(Constants::Debug_ModuleLevel) == "Debug") {
        for (auto &it : environment) {
            fprintf(stdout, "%s = %s\n", it.first.c_str(), it.second.c_str());
        }
    }

    int allocated = 256;
    int length = 0;
    auto result_environment = (char *) LocalAlloc(LPTR, allocated);
    for (auto &it : environment) {
        std::string plain_component = it.first + std::string("=") + it.second;
        if (length + plain_component.size() + 1 >= allocated) {
            allocated += plain_component.size() + 256;
            auto new_environment = (char *) LocalAlloc(LPTR, allocated);
            CopyMemory(new_environment, result_environment, length);
            LocalFree(result_environment);
            result_environment = new_environment;
        }
        CopyMemory(result_environment + length, plain_component.c_str(), plain_component.size());
        length += plain_component.size();
        result_environment[length] = '\0';
        length++;
    }
    result_environment[length] = '\0';
    return result_environment;
}

bool
SystemProcessModule::execute(const std::shared_ptr<ModuleContext> &context) {
    STARTUPINFO si{};
    PROCESS_INFORMATION pi{};
    bool ret;
    char *environment;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    environment = parse_environment(context);
    SetLastError(0);
    auto file_path_name = (char *)spec().plugin_file_path.c_str();
    fprintf(stdout, "[SystemProcessModule] Execute \"%s\"\n", file_path_name);
    ret = CreateProcessA(nullptr, file_path_name, nullptr, nullptr,
                        FALSE, 0, environment, nullptr, &si, &pi) == TRUE;
    if (ret) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    } else {
        fprintf(stderr, "[SystemProcessModule] CreateProcess error: %d\n", (unsigned int)GetLastError());
    }
    if (environment != nullptr) {
        LocalFree(environment);
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return ret;
}

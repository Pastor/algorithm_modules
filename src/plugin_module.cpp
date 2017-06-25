#include <string>
#include <plugin_module.h>

ScriptModule::ScriptModule() {}

const std::string &
ModuleContext::property(const std::string &key) {
    return _properties[key];
}

void
ModuleContext::set_property(const std::string &key, const std::string &value) {
    _properties[key] = value;
}

int
ModuleContext::int_value(const std::string &key) {
    return std::stoi(property(key), nullptr, 10);
}

bool
ModuleContext::bool_value(const std::string &key) {
    return property(key) == std::string("true");
}

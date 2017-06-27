#ifndef ALGORITHM_PLUGIN_SPEC_H
#define ALGORITHM_PLUGIN_SPEC_H

#include <string>
#include <vector>
#include <tinyxml2.h>

struct PluginSpec final {
    enum ModuleType {
        DynamicLibrary,
        PythonScript,
        UnknownModule
    };

    std::string plugin_file_path;
    std::string plugin_name;
    double plugin_version;
    std::string plugin_description;
    ModuleType plugin_type = UnknownModule;

    void toXml(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document) const;
    void fromXml(const tinyxml2::XMLElement *element);

    bool is_valid() const;
};

typedef std::vector<PluginSpec> PluginSpecList;

class PluginSpecController final {
    PluginSpecList _list;
    const std::string _file_name;

public:
    PluginSpecController(const std::string &file_name)
            : _file_name(file_name) {};

    void write();

    void write(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document);

    void read();

    void read(const tinyxml2::XMLDocument &document);

    void add(const PluginSpec &spec);

    const PluginSpecList &specs() const {
        return _list;
    }
};

#endif //ALGORITHM_PLUGIN_SPEC_H

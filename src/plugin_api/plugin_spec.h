#ifndef ALGORITHM_PLUGIN_SPEC_H
#define ALGORITHM_PLUGIN_SPEC_H

#include <string>
#include <vector>
#include <memory>
#include <tinyxml2.h>

#if defined(EXPORT_API_LIBRARY)
#define API_LIBRARY_API __declspec(dllexport)
#else
#define API_LIBRARY_API __declspec(dllimport)
#endif

class ModuleContext;

struct API_LIBRARY_API PluginSpec final {
    enum ModuleType {
        DynamicLibrary,
        PythonScript,
        UnknownModule
    };

    enum ModuleStage {
        FirstInput = 1,
        ProcessingInput = 2,
        UnknownInput
    };

    std::string plugin_file_path;
    std::string plugin_name;
    double plugin_version;
    std::string plugin_description;
    ModuleType plugin_type = UnknownModule;
    ModuleStage plugin_stage = UnknownInput;
    std::shared_ptr<ModuleContext> plugin_context;
    std::string plugin_input_stream;

    std::string plugin_stage_text() const;
    std::string plugin_type_text() const;

    void toXml(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document) const;
    void fromXml(const tinyxml2::XMLElement *element);

    void dump(const char * prefix) const;

    bool is_valid() const;
};

typedef std::vector<PluginSpec> PluginSpecList;

class API_LIBRARY_API PluginSpecController final {
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

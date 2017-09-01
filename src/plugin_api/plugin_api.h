#ifndef ALGORITHM_PLUGIN_API_H
#define ALGORITHM_PLUGIN_API_H

#include <string>
#include <memory>
#include <map>
#include <plugin_spec.h>

namespace Constants {
    const std::string Database_Hostname = "Database.Connection.Hostname";
    const std::string Database_Database = "Database.Connection.Database";
    const std::string Database_Username = "Database.Connection.Username";
    const std::string Database_Password = "Database.Connection.Password";
    const std::string Database_Schema = "Database.Connection.Schema";
    const std::string Database_Port = "Database.Connection.Port";

    /**Входная таблица при вторичной обработки */
    const std::string Database_Stream_Input = "Database.Stream.Input";
    /**Выходная таблица */
    const std::string Database_Stream_Output = "Database.Stream.Output";
    /**Входной файл при первичной обработке */
    const std::string Stream_Input = "Stream.Input";
    /**Помещать Environment вызывающего процесса */
    const std::string SystemProcessModule_PutEnvironment = "Module.SystemProcess.Put_Environment";

    /** Debug */
    const std::string Debug_ModuleLevel = "Debug.Module_Level";
}

class API_LIBRARY_API ModuleContext {
    std::map<std::string, std::string> _properties;
    std::shared_ptr<ModuleContext> _parent;
public:
    ModuleContext();

    ModuleContext(const std::shared_ptr<ModuleContext> &parent);

    virtual ~ModuleContext() {}

    virtual const std::string &property(const std::string &key);

    virtual void set_property(const std::string &key, const std::string &value);

    int int_value(const std::string &key);

    bool bool_value(const std::string &key);

    operator bool() const;

    const std::map<std::string, std::string> &entries() const;

    void toXml(tinyxml2::XMLElement *root, tinyxml2::XMLDocument &document) const;

    void fromXml(const tinyxml2::XMLElement *element);

    void copy_to(ModuleContext &context) const;
};

class API_LIBRARY_API Module {
public:
    Module() {}

    virtual ~Module() {}

    virtual const PluginSpec &spec() const = 0;

    virtual bool execute(const std::shared_ptr<ModuleContext> &context) = 0;
};

class API_LIBRARY_API DefaultModule : public Module {
public:
    DefaultModule(const PluginSpec &spec);

    DefaultModule(PluginSpec::ModuleType type,
                 const std::string name,
                 const std::string &description,
                 double version,
                 const std::string &file_path = std::string());

    virtual ~DefaultModule() {}

    virtual const PluginSpec &spec() const {
        return _spec;
    }

private:
    PluginSpec _spec;
};

#endif //ALGORITHM_PLUGIN_API_H

#ifndef ALGORITHM_PLUGIN_CONTROLLER_SERVICE_H
#define ALGORITHM_PLUGIN_CONTROLLER_SERVICE_H

#include <string>
#include <plugin_api.h>
#include <tinyxml2.h>
#include <libpq-fe.h>
#include <algorithm>
#include <windows.h>
#include <mutex>

#include "sql_module.h"

struct BaseConnection {
    BaseConnection(const std::shared_ptr<ModuleContext> &context);
    ~BaseConnection();

    bool is_connected();

    bool reconnect();

    PGresult                      *execute(const std::string &query);
    std::shared_ptr<ModuleContext> copy_context() const;
    void                           load_context(const tinyxml2::XMLElement *element);
    void                           refresh();
private:
    std::shared_ptr<ModuleContext> _context;
    std::string _hostname;
    std::string _database;
    std::string _username;
    std::string _password;
    std::string _port;
    std::recursive_mutex _mutex;
    PGconn *_c;
};

/*
 * Список задач
 SELECT
   pm.table_name,
   pm.status,
   um.user_name,
   um.schema_name
 FROM
   "processing"."processing_module" pm
   LEFT JOIN "processing"."user_module" um
     ON pm.module_id = um.id;
 */
struct ControllerService final {
public:
    ControllerService(const PluginSpec &spec, std::shared_ptr<BaseConnection> &c);

    ~ControllerService();

    std::string __module_name(const PluginSpec &spec) const;

    bool is_user_exists(const std::string &user_name_text = std::string());

    bool create_user(const PluginSpec &spec);

    bool update_password();

    bool update_complete(bool success);

    bool register_module_processing(const PluginSpec &spec, const std::string &stream_input, const std::string &stage);

    void generate_uuid(std::string &uuid_text) const;

    operator bool();

    std::string user_name;
    std::string schema_name;
    std::string module_name;
    std::string module_id;
    std::string processing_module_id;
    std::string user_password;
    std::string output_table;
    bool user_exists = false;
private:
    std::shared_ptr<BaseConnection> c;
};

#endif //ALGORITHM_PLUGIN_CONTROLLER_SERVICE_H

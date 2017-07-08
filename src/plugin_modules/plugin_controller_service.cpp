#include <mutex>
#include "plugin_controller_service.h"

#pragma comment(lib, "Rpcrt4.lib")

BaseConnection::BaseConnection(const std::shared_ptr<ModuleContext> &context)
        : _context(new ModuleContext), _c(nullptr) {
    context->copy_to(*_context);
    refresh();
}

BaseConnection::~BaseConnection() {
    std::lock_guard<std::recursive_mutex> locker(_mutex);
    if (_c != nullptr) {
        PQfinish(_c);
    }
    _c = nullptr;
}


PGresult *
BaseConnection::execute(const std::string &query) {
    std::lock_guard<std::recursive_mutex> locker(_mutex);
    if (_c != nullptr)
        return PQexec(_c, query.c_str());
    if (reconnect())
        return PQexec(_c, query.c_str());
    fprintf(stderr, "[BaseConnection] Can't connection to database");
    return nullptr;
}

bool
BaseConnection::reconnect() {
    std::lock_guard<std::recursive_mutex> locker(_mutex);
    if (_c != nullptr) {
        PQfinish(_c);
    }
    _c = nullptr;
    _c = PQsetdbLogin(_hostname.c_str(),
                      _port.c_str(),
                      "",
                      "",
                      _database.c_str(),
                      _username.c_str(),
                      _password.c_str());
    return is_connected();
}

bool
BaseConnection::is_connected() {
    std::lock_guard<std::recursive_mutex> locker(_mutex);
    return _c != nullptr && PQstatus(_c) == CONNECTION_OK;
}

std::shared_ptr<ModuleContext>
BaseConnection::copy_context() const {
    std::shared_ptr<ModuleContext> context(new ModuleContext);
    _context->copy_to(*context);
    return context;
}

void
BaseConnection::load_context(const tinyxml2::XMLElement *element) {
    _context->fromXml(element);
    refresh();
}

void
BaseConnection::refresh() {
    _hostname = _context->property(Constants::Database_Hostname);
    _database = _context->property(Constants::Database_Database);
    _username = _context->property(Constants::Database_Username);
    _password = _context->property(Constants::Database_Password);
    _port = _context->property(Constants::Database_Port);
}


ControllerService::ControllerService(const PluginSpec &spec, std::shared_ptr<BaseConnection> &_c) : c(_c) {
    module_name = __module_name(spec);
    user_name = std::string("Module_") + module_name;
    schema_name = std::string("Module_") + module_name;
}

ControllerService::~ControllerService() {
}

std::string
ControllerService::__module_name(const PluginSpec &spec) const {
    static char buffer[20];
    sprintf(buffer, "%02.2f", spec.plugin_version);
    return std::string("v") + buffer + std::string("_") + spec.plugin_name;
}

bool
ControllerService::is_user_exists(const std::string &user_name_text) {
    auto ret = false;
    auto error = true;
    std::string temp_user_name = user_name_text;
    if (temp_user_name.empty())
        temp_user_name = user_name;
    auto result = c->execute(std::string("SELECT 1 FROM pg_roles WHERE rolname = '") +
                             temp_user_name + (std::string("'")));
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        if (PQntuples(result) > 0) {
            auto n = PQgetvalue(result, 0, 0);
            ret = std::strtol(n, nullptr, 10) > 0;
        } else {
            error = false;
        }
    }
    if (!ret && error) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
    }
    PQclear(result);
    user_exists = ret;
    return ret;
}

bool
ControllerService::create_user() {
    bool ret = false;

    generate_uuid(user_password);
    auto result = c->execute(std::string("CREATE USER \"") + user_name +
                             std::string("\" WITH PASSWORD '") + user_password +
                             std::string("'"));
    if (PQresultStatus(result) == PGRES_COMMAND_OK) {
        ret = true;
    }
    if (!ret) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
    }
    PQclear(result);

    result = c->execute(std::string(
            "INSERT INTO \"processing\".\"user_module\"(\"user_name\", \"user_password\", \"schema_name\") VALUES('") +
                        user_name + std::string("', '") +
                        user_password + std::string("', '") +
                        schema_name + std::string("') RETURNING id"));
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        if (PQntuples(result) > 0) {
            module_id = PQgetvalue(result, 0, 0);
            ret = true;
        }
    }
    if (!ret) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
    }
    PQclear(result);

    if (ret) {
        result = c->execute(std::string("CREATE SCHEMA IF NOT EXISTS \"") + schema_name +
                            std::string("\" AUTHORIZATION \"") + user_name + std::string("\""));
        if (PQresultStatus(result) == PGRES_COMMAND_OK) {
            ret = true;
            user_exists = true;
        }
    }
    if (!ret) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
    }
    PQclear(result);
    return ret;
}

bool
ControllerService::update_password() {
    auto ret = false;
    auto result = c->execute(std::string(
            "SELECT id, user_password FROM \"processing\".\"user_module\" WHERE user_name = '") +
                             user_name + std::string("'"));
    if (PQresultStatus(result) == PGRES_TUPLES_OK) {
        if (PQntuples(result) > 0) {
            user_password = PQgetvalue(result, 0, 1);
            module_id = PQgetvalue(result, 0, 0);
            ret = true;
        }
    }
    if (!ret) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
    }
    user_exists = ret;
    PQclear(result);
    return ret;
}

bool
ControllerService::update_complete() {
    bool ret = false;
    auto result = c->execute(std::string(
            "UPDATE \"processing\".\"processing_module\" SET complete_at = now() WHERE id = ")
                             + processing_module_id);
    if (PQresultStatus(result) == PGRES_COMMAND_OK) {
        ret = true;
    }
    if (!ret) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
        output_table = "";
    }
    PQclear(result);
    return ret;
}

bool
ControllerService::create_output_table(const std::string &stream_input, const std::string &stage) {
    bool ret = false;

    generate_uuid(output_table);
    auto result = c->execute(std::string("CREATE TABLE \"") + schema_name + std::string("\".\"") +
                             output_table + std::string("\"(") +
                             std::string("   id          BIGSERIAL NOT NULL PRIMARY KEY, ") +
                             std::string("   created_at  TIMESTAMP NOT NULL DEFAULT now() ") +
                             std::string(")"));
    if (PQresultStatus(result) == PGRES_COMMAND_OK) {
        ret = true;
    }
    if (!ret) {
        fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
        output_table = "";
    }
    PQclear(result);
    if (ret) {
        ret = false;
        result = c->execute(std::string("ALTER TABLE \"") + schema_name + std::string("\".") +
                            std::string("\"") + output_table + std::string("\" OWNER TO ") +
                            std::string("\"") + user_name + std::string("\""));
        if (PQresultStatus(result) == PGRES_COMMAND_OK) {
            ret = true;
        }
        if (!ret) {
            fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
            output_table = "";
        }
        PQclear(result);
    }
    if (ret) {
        ret = false;
        result = c->execute(std::string(
                "INSERT INTO \"processing\".\"processing_module\"(stream_input, stage, module_id, table_name) VALUES(")
                            + std::string("'") + stream_input + std::string("', ")
                            + std::string("'") + stage + std::string("', ")
                            + std::string("'") + module_id + std::string("', ")
                            + std::string("'") + output_table + std::string("'")
                            + std::string(") RETURNING id"));
        if (PQresultStatus(result) == PGRES_TUPLES_OK) {
            if (PQntuples(result) > 0) {
                processing_module_id = PQgetvalue(result, 0, 0);
                ret = true;
            }
        }
        if (!ret) {
            fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
            output_table = "";
        }
        PQclear(result);
    }
    return ret;
}

void
ControllerService::generate_uuid(std::string &uuid_text) const {
    UUID uuid;
    if (UuidCreate(&uuid) == RPC_S_OK) {
        LPCSTR result_uuid;
        UuidToStringA(&uuid, (RPC_CSTR *) &result_uuid);
        uuid_text = std::string(result_uuid);
        RpcStringFreeA((RPC_CSTR *) &result_uuid);
        std::transform(uuid_text.begin(), uuid_text.end(), uuid_text.begin(), ::toupper);
    }
}

ControllerService::operator bool() {
    return c && c->is_connected();
}


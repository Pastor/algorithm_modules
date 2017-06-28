#include <memory>
#include <thread>
#include <tinyxml2.h>
#include <libpq-fe.h>
#include <dynamic_library_module.h>
#include <algorithm>
#include <python_plugin_module.h>
#include <windows.h>
#include "module_controller.h"

#pragma comment(lib, "Rpcrt4.lib")

class ControllerService {
public:
    ControllerService(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) : c(nullptr) {
        const auto &hostname = context->property(Constants::Database_Hostname);
        const auto &database = context->property(Constants::Database_Database);
        const auto &username = context->property(Constants::Database_Username);
        const auto &password = context->property(Constants::Database_Password);
        const auto &port = context->property(Constants::Database_Port);
        c = PQsetdbLogin(hostname.c_str(), port.c_str(), "", "", database.c_str(), username.c_str(), password.c_str());

        module_name = __module_name(spec);
        user_name = std::string("User_") + module_name;
        schema_name = std::string("Schema_") + module_name;
    }

    ~ControllerService() {
        if (c != nullptr) {
            PQfinish(c);
        }
        c = nullptr;
    }

    std::string __module_name(const PluginSpec &spec) const {
        static char buffer[20];
        sprintf(buffer, "%.2f", spec.plugin_version);
        return spec.plugin_name + "_v" + buffer;
    }

    bool is_user_exists(const std::string &user_name_text = std::string()) {
        auto ret = false;
        auto error = true;
        std::string temp_user_name = user_name_text;
        if (temp_user_name.empty())
            temp_user_name = user_name;
        auto result = PQexec(c, (std::string("SELECT 1 FROM pg_roles WHERE rolname = '") +
                                 temp_user_name + (std::string("'"))).c_str());
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

    bool create_user() {
        bool ret = false;

        generate_uuid(user_password);
        auto result = PQexec(c,
                             (std::string("CREATE USER \"") + user_name +
                              std::string("\" WITH PASSWORD '") + user_password +
                              std::string("'")).c_str());
        if (PQresultStatus(result) == PGRES_COMMAND_OK) {
            ret = true;
        }
        if (!ret) {
            fprintf(stderr, "[ModuleController] Error: %s\n", PQresultErrorMessage(result));
        }
        PQclear(result);

        result = PQexec(c, (std::string(
                "INSERT INTO \"processing\".\"user_module\"(\"user_name\", \"user_password\", \"schema_name\") VALUES('") +
                            user_name + std::string("', '") +
                            user_password + std::string("', '") +
                            schema_name + std::string("') RETURNING id")).c_str());
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
            result = PQexec(c,
                            (std::string("CREATE SCHEMA IF NOT EXISTS \"") + schema_name +
                             std::string("\" AUTHORIZATION \"") + user_name + std::string("\"")).c_str());
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

    bool update_password() {
        auto ret = false;
        auto result = PQexec(c,
                             (std::string(
                                     "SELECT id, user_password FROM \"processing\".\"user_module\" WHERE user_name = '") +
                              user_name + std::string("'")).c_str());
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

    bool update_complete() {
        bool ret = false;
        auto result = PQexec(c,
                             (std::string(
                                     "UPDATE \"processing\".\"processing_module\" SET complete_at = now() WHERE id = ")
                              + processing_module_id).c_str());
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

    bool create_output_table(const std::string &stream_input, const std::string &stage) {
        bool ret = false;

        generate_uuid(output_table);
        auto result = PQexec(c,
                             (std::string("CREATE TABLE \"") + schema_name + std::string("\".\"") +
                              output_table + std::string("\"(") +
                              std::string("   id          BIGSERIAL NOT NULL PRIMARY KEY, ") +
                              std::string("   created_at  TIMESTAMP NOT NULL DEFAULT now() ") +
                              std::string(")")).c_str());
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
            result = PQexec(c,
                            (std::string("ALTER TABLE \"") + schema_name + std::string("\".") +
                             std::string("\"") + output_table + std::string("\" OWNER TO ") +
                             std::string("\"") + user_name + std::string("\"")).c_str());
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
            result = PQexec(c, (std::string(
                    "INSERT INTO \"processing\".\"processing_module\"(stream_input, stage, module_id, table_name) VALUES(")
                                + std::string("'") + stream_input + std::string("', ")
                                + std::string("'") + stage + std::string("', ")
                                + std::string("'") + module_id + std::string("', ")
                                + std::string("'") + output_table + std::string("'")
                                + std::string(") RETURNING id")).c_str());
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

    void generate_uuid(std::string &uuid_text) const {
        UUID uuid;
        if (UuidCreate(&uuid) == RPC_S_OK) {
            LPCSTR result_uuid;
            UuidToStringA(&uuid, (RPC_CSTR *) &result_uuid);
            uuid_text = std::string(result_uuid);
            RpcStringFreeA((RPC_CSTR *) &result_uuid);
            std::transform(uuid_text.begin(), uuid_text.end(), uuid_text.begin(), ::toupper);
        }
    }

    operator bool() {
        return c != nullptr && PQstatus(c) == CONNECTION_OK;
    }

    std::string user_name;
    std::string schema_name;
    std::string module_name;
    std::string module_id;
    std::string processing_module_id;
    std::string user_password;
    std::string output_table;
    bool user_exists = false;
private:
    PGconn *c;
};

static std::shared_ptr<Module>
load_from_spec(const PluginSpec &spec) {
    switch (spec.plugin_type) {
        case PluginSpec::DynamicLibrary:
            return std::shared_ptr<Module>(new DynamicLibraryModule(spec.plugin_file_path));
        case PluginSpec::PythonScript:
            return std::shared_ptr<Module>(
                    new PythonFileScriptModule(
                            spec.plugin_file_path, spec.plugin_name, spec.plugin_description, spec.plugin_version));
        default:
            return std::shared_ptr<Module>(nullptr);
    }
}

static void
module_call(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) {
    auto module = load_from_spec(spec);
    if (module) {
        ControllerService service(spec, context);

        fprintf(stdout, "[ModuleController] Module %s loaded successful\n", spec.plugin_name.c_str());
        if (service) {
            const auto user_name_text = service.user_name;
            if (!service.is_user_exists()) {
                fprintf(stdout, "[ModuleController] User %s not exists\n", user_name_text.c_str());
                if (service.create_user()) {
                    fprintf(stdout, "[ModuleController] User %s created\n", user_name_text.c_str());
                } else {
                    fprintf(stderr, "[ModuleController] User %s can't create\n", user_name_text.c_str());
                }
            } else {
                if (!service.update_password()) {
                    fprintf(stderr, "[ModuleController] Can't retrieved password \n");
                }
            }
            if (service.user_exists) {
                context->set_property(Constants::Database_Username, service.user_name);
                context->set_property(Constants::Database_Password, service.user_password);
                context->set_property(Constants::Database_Schema, service.schema_name);

                auto stream_input = context->property(Constants::Database_Stream_Input);
                if (spec.plugin_stage == PluginSpec::FirstInput)
                    stream_input = context->property(Constants::Stream_Input);
                const auto stage = spec.plugin_stage_text();
                if (service.create_output_table(stream_input, stage)) {
                    context->set_property(Constants::Database_Stream_Output, service.output_table);
                    module->execute(context);
                    service.update_complete();
                }
            }
        } else {
            fprintf(stderr, "[ModuleController] Can't connecting to system database\n");
        }
    } else {
        fprintf(stderr, "[ModuleController] Module %s not loaded\n", spec.plugin_name.c_str());
    }
}

void
ModuleController::execute(const PluginSpec &spec, std::shared_ptr<ModuleContext> context) {
    if (spec.is_valid() && context) {
        std::shared_ptr<ModuleContext> module_context(_context);
        context->copy_to(*module_context);
        if (spec.plugin_context) {
            spec.plugin_context->copy_to(*module_context);
        }
        std::thread caller(module_call, spec, module_context);
        caller.join();
    } else {
        fprintf(stderr, "[ModuleController] Module %s not valid or invalid context.\n", spec.plugin_name.c_str());
    }
}

ModuleController::ModuleController(const std::shared_ptr<ModuleContext> &context)
        : _context(context) {}

void
ModuleController::load(const std::string &file_path) {
    tinyxml2::XMLDocument document;

    document.LoadFile(file_path.c_str());
    const auto configuration_xml = document.FirstChildElement("Configuration");
    if (configuration_xml == nullptr)
        return;
    for (auto node = configuration_xml->FirstChild(); node; node = node->NextSibling()) {
        const auto element = node->ToElement();
        if (element == nullptr)
            continue;
        fprintf(stdout, "ModuleController. Name: %s\n", element->Name());
        if (std::strcmp(element->Name(), "Properties") == 0) {
            _context->fromXml(element);
        }
    }
}

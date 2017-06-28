#include "postgresql_module.h"
#include <libpq-fe.h>

bool
plugin_call(ModuleContext *context) {
    const auto &hostname = context->property(Constants::Database_Hostname);
    const auto &database = context->property(Constants::Database_Database);
    const auto &username = context->property(Constants::Database_Username);
    const auto &password = context->property(Constants::Database_Password);
    const auto &port = context->property(Constants::Database_Port);
    bool ret = false;

    fprintf(stdout, "[PostgreSqlModule] Try connect to database by user %s.\n", username.c_str());
    auto c = PQsetdbLogin(hostname.c_str(), port.c_str(), "", "", database.c_str(), username.c_str(), password.c_str());
    if (c != nullptr) {
        if (PQstatus(c) == CONNECTION_OK) {
            ret = true;
            fprintf(stdout, "[PostgreSqlModule] Connected.\n");
        } else {
            const auto message = PQerrorMessage(c);
            fprintf(stderr, "[PostgreSqlModule] Connection error: %s\n", message);
        }
        PQfinish(c);
    } else {
        fprintf(stderr, "[PostgreSqlModule] Can't connected\n");
    }
    return ret;
}

const char *
plugin_name() {
    return "PostgreSqlModule";
}

double
plugin_version() {
    return 1.02;
}

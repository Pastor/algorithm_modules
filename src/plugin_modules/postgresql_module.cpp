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
    auto c = PQsetdbLogin(hostname.c_str(), port.c_str(), "", "", database.c_str(), username.c_str(), password.c_str());
    if (c != nullptr) {
        if (PQstatus(c) == CONNECTION_OK) {
            ret = true;
        } else {
            const auto message = PQerrorMessage(c);
            fprintf(stderr, "Connection error: %s\n", message);
        }
        PQfinish(c);
    } else {
        fprintf(stderr, "I can't connect to database\n");
    }
    return ret;
}
const char *
plugin_name() {
    return "postgresql_plugin_module";
}

double
plugin_version() {
    return 1.02;
}

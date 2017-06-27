#include "postgresql_context.h"
#include <libpq-fe.h>

struct PostgreSqlContextPrivate {

    PostgreSqlContextPrivate() : c(nullptr) {}

    void close() {
        if (c != nullptr) {
            PQfinish(c);
            c = nullptr;
        }
    }

    bool is_connected() {
        return c != nullptr && PQstatus(c) == CONNECTION_OK;
    }

    PGconn *c;
};

PostgreSqlContext::PostgreSqlContext()
        : SqlContext(SqlContext::PostgreSQL), d(new PostgreSqlContextPrivate) {
}

PostgreSqlContext::PostgreSqlContext(std::shared_ptr<ModuleContext> parent)
        : SqlContext(SqlContext::PostgreSQL, parent), d(new PostgreSqlContextPrivate) {
}

PostgreSqlContext::~PostgreSqlContext() {
    d->close();
}

bool
PostgreSqlContext::connect() const {
    if (is_connected())
        d->close();
    d->c = PQsetdbLogin("localhost", "5432", "", "", "operational_db", "dcm", "12345678");
    return is_connected();
}

bool
PostgreSqlContext::is_connected() const {
    return d->is_connected();
}

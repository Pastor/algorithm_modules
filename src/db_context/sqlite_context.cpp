#include "sqlite_context.h"

#include <sqlite3.h>

struct SqliteContextPrivate {

    SqliteContextPrivate() : c(nullptr) {}

    bool is_connected() {
        return c != nullptr;
    }

    void close() {
        if (c != nullptr) {
            sqlite3_close(c);
        }
        c = nullptr;
    }

    sqlite3 *c;
};

SqliteContext::SqliteContext()
        : SqlContext(SqlContext::SQLite), d(new SqliteContextPrivate) {

}

SqliteContext::SqliteContext(std::shared_ptr<ModuleContext> parent)
        : SqlContext(SqlContext::SQLite, parent), d(new SqliteContextPrivate) {

}

SqliteContext::~SqliteContext() {
    d->close();
}

bool
SqliteContext::is_connected() const {
    return d->is_connected();
}

bool
SqliteContext::connect() const {
    if (is_connected())
        d->close();
    return sqlite3_open(":memory:", &d->c) == 0 && is_connected();
}

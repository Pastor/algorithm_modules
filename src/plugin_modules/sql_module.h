#ifndef ALGORITHM_SQL_CONTEXT_H
#define ALGORITHM_SQL_CONTEXT_H

#include <plugin_api.h>

#if defined(EXPORT_CONTEXT_LIBRARY)
#define CONTEXT_LIBRARY_API __declspec(dllexport)
#else
#define CONTEXT_LIBRARY_API __declspec(dllimport)
#endif

/*
CREATE TABLE "Schema_${ModuleName}_${ModuleVersion}"."${Table_UUID}"(
   id            BIGSERIAL NOT NULL PRIMARY KEY,
   created_at    TIMESTAMP NOT NULL DEFAULT now()
);
INSERT INTO "public"."module_processing"("stream_input", "schema_name", "table_name")
       VALUES ('UUID', 'Schema_${ModuleName}_${Version}', '${Table_UUID}');

...
UPDATE "public"."module_processing" SET complete_at = now() WHERE id = ${ID}
*/

#endif //ALGORITHM_SQL_CONTEXT_H

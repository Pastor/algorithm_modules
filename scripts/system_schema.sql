DROP SCHEMA "processing";
CREATE SCHEMA "processing";

DROP TABLE IF EXISTS "processing"."user_module";
CREATE TABLE "processing"."user_module"(
  id            BIGSERIAL NOT NULL PRIMARY KEY,

  user_name     TEXT NOT NULL,
  schema_name   TEXT NOT NULL,
  user_password TEXT NOT NULL,
  created_at    TIMESTAMP NOT NULL DEFAULT now()
);

DROP TABLE IF EXISTS "processing"."processing_module";
CREATE TABLE "processing"."processing_module"(
  id            BIGSERIAL NOT NULL PRIMARY KEY,

  stream_input  TEXT NOT NULL,
  stage         TEXT NOT NULL,
  module_id     BIGINT NOT NULL REFERENCES "processing"."user_module"(id) ON DELETE RESTRICT,
  table_name    TEXT NOT NULL,
  complete_at   TIMESTAMP DEFAULT NULL,
  created_at    TIMESTAMP NOT NULL DEFAULT now()
);

--INSERT INTO "processing"."processing_module"(stream_input, stage, module_id, table_name) VALUES() RETURNING id

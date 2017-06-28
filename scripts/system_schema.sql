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



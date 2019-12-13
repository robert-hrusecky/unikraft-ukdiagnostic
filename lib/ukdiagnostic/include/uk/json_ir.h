#ifndef JSON_IR_H_
#define JSON_IR_H_

#include <stddef.h>
#include <stdint.h>

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INT,
    JSON_FLOAT,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL,
    JSON_ERROR,
} json_value_t;

struct json_object {
    char* key;
    struct json_value* value;
    struct json_object* next;
};

struct json_array {
    struct json_value** values;
    size_t size;
};

struct json_value {
    json_value_t type;
    union {
        struct json_object* object;
        struct json_array* array;
        char* string;
        int64_t integer;
        double floating;
    };
};

struct json_value* create_json_value(json_value_t type);

void free_json_object(struct json_object* object);
void free_json_array(struct json_array* array);
void free_json_value(struct json_value* value);

void json_object_insert(struct json_value* object_value, const char* key, struct json_value* value);
struct json_value* json_object_lookup(struct json_value* object_value, const char* key);

size_t to_json(char* buff, size_t len, struct json_value* value);

struct json_value* json_wrap_int(int64_t integer);
struct json_value* json_wrap_string(const char* string);

#endif

#include <uk/json_ir.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct json_value* create_json_value(json_value_t type) {
    struct json_value* value = malloc(sizeof *value);
    value->type = type;
    switch (type) {
        case JSON_OBJECT:
            value->object = NULL;
            break;
        case JSON_ARRAY:
            value->array = NULL;
            break;
        case JSON_STRING:
            value->string = NULL;
            break;
        case JSON_INT:
            value->integer = 0;
            break;
        case JSON_FLOAT:
            value->floating = 0.0;
            break;
    }
    return value;
}


void free_json_object(struct json_object* object) {
    if (object == NULL) return;
    free(object->key);
    free_json_value(object->value);
    free_json_object(object->next);
    free(object);
}

void free_json_array(struct json_array* array) {
    if (array == NULL) return;

    for (size_t i = 0; i < array->size; i++) {
        free_json_value(array->values[i]);
    }
    free(array->values);

    free(array);
}

void free_json_value(struct json_value* value) {
    if (value == NULL) return;
    switch (value->type) {
        case JSON_OBJECT:
            free_json_object(value->object);
            break;
        case JSON_ARRAY:
            free_json_array(value->array);
            break;
        case JSON_STRING:
            free(value->string);
            break;
        default:
            break;
    }
    free(value);
}

void json_object_insert(struct json_value* object_value, const char* key, struct json_value* value) {
    if (object_value->type != JSON_OBJECT) {
        return;
    }
    struct json_object* curr = object_value->object;
    struct json_object* insert = malloc(sizeof *insert); 
    insert->key = malloc((strlen(key) + 1) * sizeof *insert->key);
    strcpy(insert->key, key);
    insert->value = value;
    insert->next = NULL;
    if (curr == NULL) {
        object_value->object = insert; 
    } else {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = insert;
    }
}

struct json_value* json_object_lookup(struct json_value* object_value, const char* key) {
    if (object_value->type != JSON_OBJECT) {
        return NULL;
    }
    for (struct json_object* curr = object_value->object; curr != NULL; curr = curr->next) {
        if (strcmp(curr->key, key) == 0)
            return curr->value;
    }
    return NULL;
}


static inline size_t minus0(size_t len, size_t size) {
    return len == 0 ? 0 : len - size;
}

size_t object_to_json(struct json_object* object, char* buff, size_t len) {
    size_t size = 0;
    char* orig = buff;
    size += snprintf(buff, len, "{");
    for (struct json_object* curr = object; curr != NULL; curr = curr->next) {
        printf("key: %s\n", curr->key);
        size += snprintf(buff + size, minus0(len, size), "\"%s\":", curr->key);
        size += to_json(buff + size, minus0(len, size), curr->value);
        if (curr->next != NULL) {
            size += snprintf(buff + size, minus0(len, size), ",");
        }
    }
    size += snprintf(buff + size, minus0(len, size), "}");
    return size;
}

size_t to_json(char* buff, size_t len, struct json_value* value) {
    switch(value->type) {
        case JSON_OBJECT:
            return object_to_json(value->object, buff, len);
        case JSON_INT:
            return snprintf(buff, len, "%ld", value->integer);
        case JSON_STRING:
            return snprintf(buff, len, "\"%s\"", value->string);
        default:
            return 0;
    }
}

struct json_value* json_wrap_int(int64_t integer) {
    struct json_value* result = create_json_value(JSON_INT);
    result->integer = integer;
    return result;
}

struct json_value* json_wrap_string(const char* string) {
    struct json_value* result = create_json_value(JSON_STRING);
    result->string = malloc((strlen(string) + 1) * sizeof *result->string);
    strcpy(result->string, string);
    return result;
}

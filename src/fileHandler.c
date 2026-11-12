#include "fileHandler.h"

bool saveFile (char *request, char *response, char *filePath) {
    json_auto_t *root = json_object();
    json_auto_t *request_json;
    json_auto_t *response_json;
    // json_error_t error;

    JSON_INDENT(4);
    JSON_SORT_KEYS ;

    json_object_set_new(root, "request", json_string(request));
    json_object_set_new(root, "response", json_string(response));

    if (json_dump_file(root, filePath, JSON_INDENT(4) | JSON_SORT_KEYS) == -1) {
        fprintf(stderr, "Error saving JSON to file: \n" );
        return false;
    }
    return true;
}
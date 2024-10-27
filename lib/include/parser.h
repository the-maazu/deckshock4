/**
BSD 3-Clause License

Copyright (c) 2023 Open Source Patterns Inc.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/** @file parser.h
 *  Method declarations and constants
 */
#ifndef NANOJSONC_PARSER_H
#define NANOJSONC_PARSER_H

#ifndef NANOJSONC_KEY_SIZE
#define NANOJSONC_KEY_SIZE      128
#endif

#ifndef NANOJSONC_VALUE_SIZE
#define NANOJSONC_VALUE_SIZE    1024
#endif

#ifndef NANOJSONC_JSON_SIZE
#define NANOJSONC_JSON_SIZE     5120
#endif

/**
 * @brief Enumeration representing error codes in NanoJSONC.
 *
 * The `NanoJSONCError` enumeration defines a set of error codes used by NanoJSONC,
 * a lightweight C library for handling JSON data. Each error code represents a specific
 * error condition that may occur during the processing of JSON data.
 */
enum NanoJSONCError {
    NO_ERROR,
    KEY_OVERFLOW,
    VALUE_OVERFLOW,
    JSON_KEY_OVERFLOW,
    JSON_VALUE_OVERFLOW
};

/**
 * @brief Callback function signature for NanoJSONC operations.
 *
 * The `NanoJSONCCallback` typedef represents the function signature for a callback
 * function used in NanoJSONC operations. This callback is invoked during JSON parsing
 * to handle key-value pairs and nested structures.
 *
 * @param error The NanoJSONCError indicating the success or type of error encountered.
 * @param key The key of the current JSON element being processed.
 * @param value The value associated with the current key.
 * @param parentKey The key of the parent JSON object or array.
 * @param object A pointer to a user-defined object or data structure for additional context.
 *
 * @note The callback function is expected to handle and process the parsed JSON data,
 * providing flexibility for user-defined actions during JSON parsing operations.
 *
 * @see NanoJSONCError for a list of possible error codes.
 */
typedef void (*NanoJSONCCallback)(enum NanoJSONCError error, const char *const key, const char *const value, const char *const parentKey, void *object);

/**
 * @brief Parse a JSON array and invoke a callback for each element.
 *
 * This function parses a JSON array and invokes the specified callback function
 * for each element found within the array. It supports the parsing of deep
 * nested objects and arrays and can handle various data types such as strings,
 * numbers, booleans, and null values.
 *
 * @param json The JSON array to parse.
 * @param parentKey The parent key (if applicable).
 * @param object A pointer to the object being populated or processed.
 * @param callback The callback function to be invoked for each element.
 *
 * The callback function should have the following signature:
 * @code
 * void callback(enum NanoJSONCError error, const char *const key,
 *               const char *const value, const char *const parentKey,
 *               void *object);
 * @endcode
 *
 * @note The provided callback function should handle the processing or population
 *       of the object based on the parsed JSON elements.
 *
 * @see nanojsonc_parse_object
 */
void nanojsonc_parse_array(const char *json, const char *parentKey, void *object, NanoJSONCCallback callback);

/**
 * @brief Parse a JSON object and invoke a callback for each key-value pair.
 *
 * This function parses a JSON object and invokes the specified callback function
 * for each key-value pair within the object.
 *
 * @param json The JSON object to parse.
 * @param parentKey The parent key (if applicable).
 * @param object A pointer to the object being populated or processed.
 * @param callback The callback function to be invoked for each key-value pair.
 *
 * The callback function should have the following signature:
 * @code
 * void callback(enum NanoJSONCError error, const char *const key,
 *               const char *const value, const char *const parentKey,
 *               void *object);
 * @endcode
 *
 * @note The provided callback function should handle the processing or population
 *       of the object based on the parsed JSON key-value pairs.
 *
 * @see nanojsonc_parse_array
 */
void nanojsonc_parse_object(const char *json, const char *parentKey, void *object, NanoJSONCCallback callback);

/**
 * @brief Get a human-readable description for a NanoJSONCError.
 *
 * The `nanojsonc_error_desc` function returns a const char pointer containing a
 * human-readable description of the specified NanoJSONCError. This function is useful
 * for obtaining error details when handling errors in NanoJSONC operations.
 *
 * @param error The NanoJSONCError for which to get the description.
 * @return A const char pointer representing the human-readable error description.
 *
 * @note The returned pointer is statically allocated and should not be freed by the caller.
 * @see NanoJSONCError for a list of possible error codes.
 */
const char *nanojsonc_error_desc(enum NanoJSONCError error);

#endif //NANOJSONC_PARSER_H

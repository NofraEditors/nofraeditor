/**
 * @file jsonserializer.cpp
 * @author Krisna Pranav
 * @brief Json Serializaing Data Objects
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#include "common.h"
#include "jsonserializer.h"

struct JsonParser final {
    static Result parseObjectOrArray(String::CharPointerType t, SerializedData &rest) {
        skipCommentsAndWhitespaces(t);
    }
}

/**
 * @brief Supports File With Extension[JSON]
 * 
 * @param extension 
 * @return true 
 * @return false 
 */
bool JsonSerializer::supportsFileWithExtension(const String &extension) const {
    return extension.endsWithIgnoreCase("json");
}
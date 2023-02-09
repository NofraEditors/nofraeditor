/**
 * @file binaryserializer.h
 * @author Krisna Pranav
 * @brief Binary Serializer
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once

#include "Serializer.h"

class BinarySerializer final : public Serializer {
public:

    /**
     * @brief save to file 
     * 
     * @param file 
     * @param tree 
     * @return Result 
     */
    Result saveToFile(File file, const SerializedData &tree) const override;
    SerializedData loadFromFile(const File &file) const override;

    Result saveToString(String &string, const SerializedData &tree) const override;
    SerializedData loadFromString(const String &string) const override;

    bool supportsFileWithExtension(const String &extension) const override;
    bool supportsFileWithHeader(const String &header) const override;

};
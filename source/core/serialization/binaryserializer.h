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

#include "serializer.h"

class BinarySerializer final : public Serializer {
public:
    Result saveToFile(File file, const SerializedData &tree) const override;

    bool supportsFileWithExtension(const String &extension) const override;
    bool supportsFileWithHeader(const String &header) const override;
};
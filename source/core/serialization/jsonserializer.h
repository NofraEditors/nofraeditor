/**
 * @file jsonserializer.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-02-14
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once

#include "serializer.h"

class JsonSerializer final : public Serializer {
public:
    explicit JsonSerializer(bool allOnOneLine = false) noexcept;

private:
    bool allOnOneLine;
    StringArray headerComments;    
};
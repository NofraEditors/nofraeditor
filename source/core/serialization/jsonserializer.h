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

#include "Serializer.h"

class JsonSerializer final : public Serializer {
public:

    /**
     * @brief Construct a new Json Serializer object
     * 
     * @param allOnOneLine 
     */
    explicit JsonSerializer(bool allOnOneLine = false) noexcept;

    /**
     * @brief Set the Header Comments object
     * 
     * @param comments 
     */
    void setHeaderComments(StringArray comments) noexcept;

    /**
     * @brief SaveToFile
     * 
     * @param file 
     * @param tree 
     * @return Result 
     */
    Result saveToFile(File file, const SerializedData &tree) const override;

    /**
     * @brief LoadFromFile
     * 
     * @param file 
     * @return SerializedData 
     */
    SerializedData loadFromFile(const File &file) const override;

    /**
     * @brief SaveString
     * 
     * @param string 
     * @param tree 
     * @return Result 
     */
    Result saveToString(String &string, const SerializedData &tree) const override;
    
    /**
     * @brief load From string
     * 
     * @param string 
     * @return SerializedData 
     */
    SerializedData loadFromString(const String &string) const override;

    /**
     * @brief Supports File With Extension
     * 
     * @param extension 
     * @return true 
     * @return false 
     */
    bool supportsFileWithExtension(const String &extension) const override;

    /**
     * @brief Supports File With Header
     * 
     * @param header 
     * @return true 
     * @return false 
     */
    bool supportsFileWithHeader(const String &header) const override;

private:

    bool allOnOneLine;
    StringArray headerComments;

};
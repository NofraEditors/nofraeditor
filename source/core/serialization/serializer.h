/**
 * @file serializer.h
 * @author Krisna Pranav
 * @brief Serializer[SerilizeData]
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

class Serializable;
class SerializedData;

class Serializer {
public:

    virtual ~Serializer() = default;

    /**
     * @brief Save to file, save to string
     * 
     * @param file 
     * @param tree 
     * @return Result 
     */
    virtual Result saveToFile(File file, const SerializedData &tree) const = 0;
    virtual SerializedData loadFromFile(const File &file) const = 0;

    virtual Result saveToString(String &string, const SerializedData &tree) const = 0;
    virtual SerializedData loadFromString(const String &string) const = 0;


    /**
     * @brief Support file with extension or header
     * 
     * @param extension 
     * @return true 
     * @return false 
     */
    virtual bool supportsFileWithExtension(const String &extension) const = 0;
    virtual bool supportsFileWithHeader(const String &header) const = 0;

};
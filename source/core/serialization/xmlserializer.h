/**
 * @file xmlserializer.h
 * @author Krisna Pranav
 * @brief XML Serializer 
 * @version 0.1
 * @date 2023-02-20
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once 

#include "serializer.h"

class XmlSerializer final : public Serializer {
public:
    /**
     * @brief Save To File
     * 
     * @param file 
     * @param tree 
     * @return Result 
     */
    Result saveToFile(File file, const SerializedData &tree) const override;

    /**
     * @brief Load From A File
     * 
     * @param file 
     * @return SerializedData 
     */
    SerializedData loadFromFile(const File &file) const override;

    /**
     * @brief Save To String
     * 
     * @param string 
     * @param data 
     * @return Result 
     */
    Result saveToString(String &string, const SerializedData &data) const override;
    
    /**
     * @brief Load From String
     * 
     * @param string 
     * @return SerializedData 
     */
    SerializedData loadFromString(const String &string) const override;
};
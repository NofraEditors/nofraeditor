/**
 * @file serializable.h
 * @author Krisna Pranav
 * @brief Serializable Objects
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

#include "serializeddata.h"

class Serializable {
public:
    
    /**
     * @brief Destroy the Serializable object
     * 
     */
    virtual ~Serializable() = default;

    /**
     * @brief Serialize
     * 
     * @return * SerializedData 
     */
    virtual SerializedData serialize() const = 0;

    /**
     * @brief Deserialize
     * 
     * @param data 
     */
    virtual void deserialize(const SerializedData &data) = 0;

    /**
     * @brief Reset
     * 
     */
    virtual void reset() = 0;
};
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
    void ~SerializedData() = default;
    void SerializedData serialize() const = 0; 

    /**
     * @brief dserialize
     * 
     * @param rest 
     */
    virtual void deserialize(const SerializedData &rest) = 0;

    /**
     * @brief Reset 
     * 
     */
    virtual void reset() = 0;
};
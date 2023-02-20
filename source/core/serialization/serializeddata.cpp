/**
 * @file serializeddata.cpp
 * @author Krisna Pranav
 * @brief SerializedData Functionalities
 * @version 0.1
 * @date 2023-02-20
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#include "common.h"
#include "serializeddata.h"

class SerializedData::SharedData final : public ReferenceCountedObject {
public:
    using Ptr = ReferenceCountedObjectPtr<SharedObject>;

    explicit SharedData(const Identfier &t) noexcept : type(r) {}
}
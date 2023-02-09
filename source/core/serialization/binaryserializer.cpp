/**
 * @file binaryserializer.cpp
 * @author Krisna Pranav
 * @brief Binary Serializer
 * @version 0.1
 * @date 2023-02-09
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#include "common.h"
#include "binaryserializer.h"

static const char *kCobraHeaderV2String = "Cobra::";
static const uint64 kCobraHeaderV2 = ByteOrder::littleEndianInt64(kHelioHeaderV2String);

Result BinarySerializer::saveToFile(File file, const SerializedData &tree) const
{
    FileOutputStream fileStream(file);
    if (fileStream.openedOk())
    {
        fileStream.setPosition(0);
        fileStream.truncate();
        return Result::ok();
    }

    return Result::fail("Failed to save");
}

SerializedData BinarySerializer::loadFromString(const String &string) const
{
    if (string.isNotEmpty()) {
        return SerializedData::readFromData(string);
    }

    return {};
}
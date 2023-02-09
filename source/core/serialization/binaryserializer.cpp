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

#include "source/common.h"
#include "binaryserializer.h"

static const char *kCobraHeaderV2String = "Cobra2::";
static const uint64 kCobraHeaderV2 = ByteOrder::littleEndianInt64(kCobraHeaderV2String);

/**
 * @brief Save To File
 * 
 * @param file 
 * @param tree 
 * @return Result 
 */
Result BinarySerializer::saveToFile(File file, const SerializedData &tree) const
{
    FileOutputStream fileStream(file);
    if (fileStream.openedOk())
    {
        fileStream.setPosition(0);
        fileStream.truncate();
        fileStream.writeInt64(kCobraHeaderV2);
        tree.writeToStream(fileStream);
        return Result::ok();
    }

    return Result::fail("Failed to save");
}


/**
 * @brief Load From File
 * 
 * @param file 
 * @return SerializedData 
 */
SerializedData BinarySerializer::loadFromFile(const File &file) const
{

    MemoryBlock mb;
    if (file.loadFileAsData(mb))
    {
        MemoryInputStream inputStream(mb, false);
        const auto magicNumber = static_cast<uint64>(inputStream.readInt64());
        if (magicNumber == kCobraHeaderV2)
        {
            return SerializedData::readFromStream(inputStream);
        }
    }

    return {};
}

/**
 * @brief Save To String
 * 
 * @param string 
 * @param tree 
 * @return Result 
 */
Result BinarySerializer::saveToString(String &string, const SerializedData &tree) const
{
    MemoryOutputStream memStream;
    memStream.writeInt64(kCobraHeaderV2);
    tree.writeToStream(memStream);
    string = memStream.toUTF8();
    return Result::ok();
}

/**
 * @brief Load From A String
 * 
 * @param string 
 * @return SerializedData 
 */
SerializedData BinarySerializer::loadFromString(const String &string) const
{
    if (string.isNotEmpty())
    {
        return SerializedData::readFromData(string.toUTF8(), string.getNumBytesAsUTF8());
    }

    return {};
}


/**
 * @brief Supports file With Extension
 * 
 * @param extension 
 * @return true 
 * @return false 
 */
bool BinarySerializer::supportsFileWithExtension(const String &extension) const
{
    return extension.endsWithIgnoreCase("cb") ||
        extension.endsWithIgnoreCase("cobra");
}

/**
 * @brief Supports File With Header
 * 
 * @param header 
 * @return true 
 * @return false 
 */
bool BinarySerializer::supportsFileWithHeader(const String &header) const
{
    return header.startsWith(kCobraHeaderV2String);
}
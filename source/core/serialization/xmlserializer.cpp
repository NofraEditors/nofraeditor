/**
 * @file xmlserializer.cpp
 * @author Krisna Pranav
 * @brief XML Serializer
 * @version 0.1
 * @date 2023-02-20
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#include "common.h"
#include "xmlserializer.h"

/// deafult xml encoding
static const String xmlEncoding = "UTF-8";

/**
 * @brief Get the Xml Format object
 * 
 * @return XmlElement::TextFormat 
 */
static XmlElement::TextFormat getXmlFormat() {
    static XmlElement::TextFormat format;
    format.dtd = {};
    format.lineWrapLength = 120;
    format.customEncoding = xmlEncoding;
    return format;
}

/**
 * @brief save to file
 * 
 * @param file 
 * @param tree 
 * @return Result 
 */
Result XmlSerializer::saveToFile(File file, const SerializedData &tree) const {
    UniquePointer<XmlElement> xml(tree.writeToXml());
    if (xml != nullptr)
    {
        const auto saved = xml->writeTo(file, getXmlFormat());
        return saved ? Result::ok() : Result::fail({});
    }

    return Result::fail({});
}

/**
 * @brief load from file
 * 
 * @param file 
 * @return SerializedData 
 */
SerializedData XmlSerializer::loadFromFile(const File &file) const {
    XmlDocument document(file);
    UniquePointer<XmlElement> xml(document.getDocumentElement());
    if (xml != nullptr)
    {
        return SerializedData::readFromXml(*xml);
    }

    return {};
}

/**
 * @brief save to string
 * 
 * @param string 
 * @param tree 
 * @return Result 
 */
Result XmlSerializer::saveToString(String &string, const SerializedData &tree) const {
    UniquePointer<XmlElement> xml(tree.writeToXml());
    if (xml != nullptr)
    {
        string = xml->toString(getXmlFormat());
        return Result::ok();
    }

    return Result::fail({});
}

/**
 * @brief load from string
 * 
 * @param string 
 * @return SerializedData 
 */
SerializedData XmlSerializer::loadFromString(const String &string) const {
    XmlDocument document(string);
    UniquePointer<XmlElement> xml(document.getDocumentElement());
    return SerializedData::readFromXml(*xml);
}

/**
 * @brief supports file with extension
 * 
 * @param extension 
 * @return true 
 * @return false 
 */
bool XmlSerializer::supportsFileWithExtension(const String &extension) const {
    return extension.endsWithIgnoreCase("xml") || extension.endsWithIgnoreCase("nofra");
}

/**
 * @brief supports file with header
 * 
 * @param header 
 * @return true 
 * @return false 
 */
bool XmlSerializer::supportsFileWithHeader(const String &header) const {
    return header.startsWithIgnoreCase("<?xml");
}
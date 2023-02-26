/**
 * @file jsonserializer.cpp
 * @author Krisna Pranav
 * @brief Json Serializaing Data Objects
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#include "common.h"
#include "jsonserializer.h"

/**
 * @brief JsonParser
 * 
 */
struct JsonParser final {

    /**
     * @brief parseObjectOrArray
     * 
     * @param t 
     * @param result 
     * @return Result 
     */
    static Result parseObjectOrArray(String::CharPointerType t, SerializedData &result) {
        skipCommentsAndWhitespaces(t);

        switch (t.getAndAdvance())
        {
        case 0:      result = SerializedData(); return Result::ok();
        case '{':    return parseObject(t, result);
        case '[':    return parseArray(t, result, result.getType());
        }

        return createFail("Expected '{' or '['", &t);
    }

    /**
     * @brief parseStringProperty
     * 
     * @param quoteChar 
     * @param t 
     * @param propertyName 
     * @param tree 
     * @return Result 
     */
    static Result parseStringProperty(const juce_wchar quoteChar, String::CharPointerType &t, const Identifier &propertyName, SerializedData &tree) {
        String property;
        const auto r = parseString(quoteChar, t, property);
        if (r.wasOk())
        {
            tree.setProperty(propertyName, property);
        }

        return r;
    }

    /**
     * @brief parseString
     * 
     * @param quoteChar 
     * @param t 
     * @param result 
     * @return Result 
     */
    static Result parseString(const juce_wchar quoteChar, String::CharPointerType &t, String &result) {
        static MemoryOutputStream buffer(256);
        buffer.reset();

        for (;;)
        {
            auto c = t.getAndAdvance();

            if (c == quoteChar)
            {
                break;
            }

            if (c == '\\')
            {
                c = t.getAndAdvance();

                switch (c)
                {
                case '"':
                case '\'':
                case '\\':
                case '/':  break;

                case 'a':  c = '\a'; break;
                case 'b':  c = '\b'; break;
                case 'f':  c = '\f'; break;
                case 'n':  c = '\n'; break;
                case 'r':  c = '\r'; break;
                case 't':  c = '\t'; break;

                case 'u':
                {
                    c = 0;

                    for (int i = 4; --i >= 0;)
                    {
                        auto digitValue = CharacterFunctions::getHexDigitValue(t.getAndAdvance());
                        if (digitValue < 0) { return createFail("Syntax error in Unicode escape sequence"); }
                        c = (juce_wchar)((c << 4) + static_cast<juce_wchar> (digitValue));
                    }

                    break;
                }
                }
            }

            if (c == 0) { return createFail("Unexpected end-of-input in string constant"); }
            buffer.appendUTF8Char(c);
        }

        result = buffer.toUTF8();
        return Result::ok();
    }

    /**
     * @brief findNextNewline
     * 
     * @param t 
     */
    static void findNextNewline(String::CharPointerType &t) {
        juce_wchar c = 0;
        do { c = t.getAndAdvance(); } while (c != '\n' && c != '\r');
    }

    /**
     * @brief findEndOfMultilineComment
     * 
     * @param t 
     */
    static void findEndOfMultilineComment(String::CharPointerType &t) {
        juce_wchar c1 = 0;
        juce_wchar c2 = 0;
        do
        { 
            c1 = c2;
            c2 = t.getAndAdvance();
            if (c2 == 0) { return; }
        } while (c1 != '*' || c2 != '/');
    }

    /**
     * @brief skipCommentsAndWhitespaces
     * 
     * @param t 
     */
    static void skipCommentsAndWhitespaces(String::CharPointerType &t) {
        t = t.findEndOfWhitespace();
        auto t2 = t;
        switch (t2.getAndAdvance())
        {
        case '/':
            const auto c = t2.getAndAdvance();
            if (c == '/')
            {
                t = t2;
                findNextNewline(t);
                skipCommentsAndWhitespaces(t);
            }
            else if (c == '*')
            {
                t = t2;
                findEndOfMultilineComment(t);
                skipCommentsAndWhitespaces(t);
            }
        }
    }

    /**
     * @brief parseAny
     * 
     * @param t 
     * @param result 
     * @param nodeOrProperty 
     * @return Result 
     */
    static Result parseAny(String::CharPointerType &t, SerializedData &result, const Identifier &nodeOrProperty) {
        skipCommentsAndWhitespaces(t);
        auto t2 = t;

        switch (t2.getAndAdvance())
        {
        case '{':
            {
                t = t2;
                SerializedData child(nodeOrProperty);
                result.appendChild(child);
                return parseObject(t, child);
            }

        case '[':
            t = t2;
            return parseArray(t, result, nodeOrProperty);

        case '"':
            t = t2;
            return parseStringProperty('"', t, nodeOrProperty, result);

        case '\'':
            t = t2;
            return parseStringProperty('\'', t, nodeOrProperty, result);

        case '-':
            skipCommentsAndWhitespaces(t2);
            if (!CharacterFunctions::isDigit(*t2))
                break;

            t = t2;
            return parseNumberProperty(t, nodeOrProperty, result, true);

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return parseNumberProperty(t, nodeOrProperty, result, false);

        case 't':   
            if (t2.getAndAdvance() == 'r' && t2.getAndAdvance() == 'u' && t2.getAndAdvance() == 'e')
            {
                t = t2;
                result.setProperty(nodeOrProperty, true);
                return Result::ok();
            }
            break;

        case 'f':   
            if (t2.getAndAdvance() == 'a' && t2.getAndAdvance() == 'l'
                && t2.getAndAdvance() == 's' && t2.getAndAdvance() == 'e')
            {
                t = t2;
                result.setProperty(nodeOrProperty, false);
                return Result::ok();
            }
            break;

        case 'n':   
            if (t2.getAndAdvance() == 'u' && t2.getAndAdvance() == 'l' && t2.getAndAdvance() == 'l')
            {
                t = t2;
                return Result::ok();
            }
            break;

        default:
            break;
        }

        return createFail("Syntax error", &t);
    }

private:
    /**
     * @brief Create a Fail object
     * 
     * @param message 
     * @param location 
     * @return Result 
     */
    static Result createFail(const char *const message, const String::CharPointerType *location = nullptr) {
        String m(message);
        if (location != nullptr)
            m << ": \"" << String(*location, 20) << '"';

        return Result::fail(m);
    }

    /**
     * @brief parseNumberProperty 
     * 
     * @param t 
     * @param propertyName 
     * @param result 
     * @param isNegative 
     * @return Result 
     */
    static Result parseNumberProperty(String::CharPointerType &t, const Identifier &propertyName, SerializedData &result, const bool isNegative) {
        auto oldT = t;

        int64 intValue = t.getAndAdvance() - '0';
        jassert(intValue >= 0 && intValue < 10);

        for (;;)
        {
            auto previousChar = t;
            auto c = t.getAndAdvance();
            auto digit = ((int)c) - '0';

            if (isPositiveAndBelow(digit, 10))
            {
                intValue = intValue * 10 + digit;
                continue;
            }

            if (c == 'e' || c == 'E' || c == '.')
            {
                t = oldT;
                auto asDouble = CharacterFunctions::readDoubleValue(t);
                result.setProperty(propertyName, isNegative ? -asDouble : asDouble);
                return Result::ok();
            }

            if (CharacterFunctions::isWhitespace(c)
                || c == ',' || c == '}' || c == ']' || c == 0)
            {
                t = previousChar;
                break;
            }

            return createFail("Syntax error in number", &oldT);
        }

        auto correctedValue = isNegative ? -intValue : intValue;

        if ((intValue >> 31) != 0)
            result.setProperty(propertyName, correctedValue);
        else
            result.setProperty(propertyName, (int)correctedValue);

        return Result::ok();
    }

    /**
     * @brief parseObject
     * 
     * @param t 
     * @param result 
     * @return Result 
     */
    static Result parseObject(String::CharPointerType &t, SerializedData &result) {
        for (;;)
        {
            skipCommentsAndWhitespaces(t);

            auto oldT = t;
            auto c = t.getAndAdvance();

            if (c == '}') { break; }
            if (c == 0) { return createFail("Unexpected end-of-input in object declaration"); }
            if (c == '"')
            {
                String nodeNameVar;
                const auto r = parseString('"', t, nodeNameVar);
                if (r.failed()) { return r; }

                const Identifier nodeName(nodeNameVar);
                if (nodeName.isValid())
                {
                    skipCommentsAndWhitespaces(t);
                    oldT = t;

                    auto c2 = t.getAndAdvance();

                    if (c2 != ':') { return createFail("Expected ':', but found", &oldT); }

                    const auto r2 = parseAny(t, result, nodeName);
                    if (r2.failed()) { return r2; }

                    skipCommentsAndWhitespaces(t);
                    oldT = t;

                    auto nextChar = t.getAndAdvance();
                    if (nextChar == ',') { continue; }
                    if (nextChar == '}') { break; }
                }
            }

            return createFail("Expected object member declaration, but found", &oldT);
        }

        return Result::ok();
    }

    /**
     * @brief parseArray
     * 
     * @param t 
     * @param result 
     * @param nodeName 
     * @return Result 
     */
    static Result parseArray(String::CharPointerType &t, SerializedData &result, const Identifier &nodeName) {
        for (;;)
        {
            skipCommentsAndWhitespaces(t);

            auto oldT = t;
            auto c = t.getAndAdvance();

            if (c == ']') { break; }
            if (c == 0) { return createFail("Unexpected end-of-input in array declaration"); }

            t = oldT;
            auto r = parseAny(t, result, nodeName);

            if (r.failed()) { return r; }

            skipCommentsAndWhitespaces(t);
            oldT = t;

            auto nextChar = t.getAndAdvance();
            if (nextChar == ',') { continue; }
            if (nextChar == ']') { break; }
            return createFail("Expected object array item, but found", &oldT);
        }

        return Result::ok();
    }
};

/**
 * @brief JsonFormatter
 * 
 */
struct JsonFormatter final
{
    /**
     * @brief Write
     * 
     * @param out 
     * @param tree 
     * @param headerComments 
     * @param indentLevel 
     * @param oneLine 
     * @param maximumDecimalPlaces 
     */
    static void write(OutputStream &out, const SerializedData &tree, const StringArray &headerComments,
        int indentLevel, bool oneLine, int maximumDecimalPlaces)
    {
        out << '{';
        if (!oneLine) { out << newLine; }

        if (headerComments.size() > 0)
        {
            if (!oneLine) { writeSpaces(out, indentLevel + indentSize); }
            out << "/*";
            if (!oneLine) { out << newLine; }

            for (const auto &comment : headerComments)
            {
                if (!oneLine)
                {
                    writeSpaces(out, indentLevel + indentSize);
                    out << " *";
                }

                out << ' ' << comment;
                if (!oneLine) { out << newLine; }
            }

            if (!oneLine) { writeSpaces(out, indentLevel + indentSize); }
            out << " */";
            if (!oneLine) { out << newLine; }
        }

        if (!oneLine) { writeSpaces(out, indentLevel + indentSize); }
        out << '"';
        writeString(out, tree.getType());
        out << "\": ";
        writeObject(out, tree, indentLevel + indentSize, oneLine, maximumDecimalPlaces);

        if (!oneLine) { out << newLine; }
        out << '}';
    }

    /**
     * @brief writeObject
     * 
     * @param out 
     * @param tree 
     * @param indentLevel 
     * @param allOnOneLine 
     * @param maximumDecimalPlaces 
     */
    static void writeObject(OutputStream &out, const SerializedData &tree,
        int indentLevel, bool allOnOneLine, int maximumDecimalPlaces)
    {
        out << '{';
        if (!allOnOneLine) { out << newLine; }

        const int numProperties = tree.getNumProperties();
        const int numChildren = tree.getNumChildren();
        for (int i = 0; i < numProperties; ++i)
        {
            const auto propertyName(tree.getPropertyName(i));

            if (!allOnOneLine) { writeSpaces(out, indentLevel + indentSize); }
            out << '"';
            writeString(out, propertyName);
            out << "\": ";
            writeProperty(out, tree.getProperty(propertyName), maximumDecimalPlaces);

            const bool shouldHaveComma = i < numProperties - 1 || numChildren > 0;
            if (shouldHaveComma)
            {
                if (allOnOneLine) { out << ", "; } else { out << ',' << newLine; }
            }
            else if (!allOnOneLine)
            {
                out << newLine;
            }
        }

        using GroupedChildren = FlatHashMap<Identifier, Array<SerializedData>, IdentifierHash>;
        GroupedChildren children;
        for (const auto &child : tree)
        {
            if (!children.contains(child.getType()))
            {
                children[child.getType()] = Array<SerializedData>(child);
                continue;
            }

            children.at(child.getType()).add(child);
        }

        for (auto i = children.begin(); i != children.end(); ++i)
        {
            const auto &childrenType = i->first;
            const auto &childGroupOfSameType = i->second;

            if (!allOnOneLine)
            {
                writeSpaces(out, indentLevel + indentSize);
            }

            out << '"';
            writeString(out, childrenType);
            out << "\": ";

            if (childGroupOfSameType.size() == 1)
            {
                writeObject(out, childGroupOfSameType.getFirst(),
                    indentLevel + indentSize, allOnOneLine, maximumDecimalPlaces);
            }
            else
            {
                writeArray(out, childGroupOfSameType,
                    indentLevel + indentSize, allOnOneLine, maximumDecimalPlaces);
            }

            if (i != children.end() && std::next(i) != children.end())
            {
                if (allOnOneLine) { out << ", "; } else { out << ',' << newLine; }
            }
            else if (!allOnOneLine)
            {
                out << newLine;
            }
        }

        if (!allOnOneLine)
        {
            writeSpaces(out, indentLevel);
        }

        out << '}';
    }

    /**
     * @brief writeProperty
     * 
     * @param out 
     * @param v 
     * @param maximumDecimalPlaces 
     */
    static void writeProperty(OutputStream& out, const var &v, int maximumDecimalPlaces = 6)
    {
        if (v.isString())
        {
            out << '"';
            writeString(out, v.toString().getCharPointer());
            out << '"';
        }
        else if (v.isVoid())
        {
            out << "null";
        }
        else if (v.isUndefined())
        {
            out << "undefined";
        }
        else if (v.isBool())
        {
            out << (static_cast<bool> (v) ? "true" : "false");
        }
        else if (v.isInt() || v.isInt64())
        {
            out << v.toString();
        }
        else if (v.isDouble())
        {
            out << String(static_cast<double> (v), maximumDecimalPlaces);
        }
        else
        {
            jassertfalse;
            out << v.toString();
        }
    }

    /**
     * @brief writeEscapedChar
     * 
     * @param out 
     * @param value 
     */
    static void writeEscapedChar(OutputStream &out, const unsigned short value) {
        out << "\\u" << String::toHexString((int)value).paddedLeft('0', 4);
    }

    /**
     * @brief writeString
     * 
     * @param out 
     * @param t 
     */
    static void writeString(OutputStream &out, String::CharPointerType t) {
        for (;;)
        {
            auto c = t.getAndAdvance();

            switch (c)
            {
            case 0:  return;

            case '\"':  out << "\\\""; break;
            case '\\':  out << "\\\\"; break;
            case '\a':  out << "\\a";  break;
            case '\b':  out << "\\b";  break;
            case '\f':  out << "\\f";  break;
            case '\t':  out << "\\t";  break;
            case '\r':  out << "\\r";  break;
            case '\n':  out << "\\n";  break;

            default:
                if (c >= 32 && c < 127)
                {
                    out << (char)c;
                }
                else
                {
                    if (CharPointer_UTF16::getBytesRequiredFor(c) > 2)
                    {
                        CharPointer_UTF16::CharType chars[2];
                        CharPointer_UTF16 utf16(chars);
                        utf16.write(c);

                        for (int i = 0; i < 2; ++i)
                        {
                            writeEscapedChar(out, (unsigned short)chars[i]);
                        }
                    }
                    else
                    {
                        writeEscapedChar(out, (unsigned short)c);
                    }
                }

                break;
            }
        }
    }

    /**
     * @brief writeSpaces
     * 
     * @param out 
     * @param numSpaces 
     */
    static void writeSpaces(OutputStream &out, int numSpaces) {
        out.writeRepeatedByte(' ', (size_t)numSpaces);
    }

    /**
     * @brief writeArray
     * 
     * @param out 
     * @param array 
     * @param indentLevel 
     * @param allOnOneLine 
     * @param maximumDecimalPlaces 
     */
    static void writeArray(OutputStream &out, const Array<SerializedData> &array,
        int indentLevel, bool allOnOneLine, int maximumDecimalPlaces)
    {
        out << '[';

        if (!array.isEmpty())
        {
            if (!allOnOneLine) { out << newLine; }

            for (int i = 0; i < array.size(); ++i)
            {
                if (!allOnOneLine) { writeSpaces(out, indentLevel + indentSize); }

                writeObject(out, array.getReference(i),
                    indentLevel + indentSize, allOnOneLine, maximumDecimalPlaces);

                if (i < array.size() - 1)
                {
                    if (allOnOneLine) { out << ", "; } else { out << ',' << newLine; }
                }
                else if (!allOnOneLine)
                {
                    out << newLine;
                }
            }

            if (!allOnOneLine) { writeSpaces(out, indentLevel); }
        }

        out << ']';
    }

    enum { indentSize = 2 };
};

/**
 * @brief fakeRoot
 * 
 */
static const Identifier fakeRoot = "root";

/**
 * @brief Construct a new Json Serializer:: Json Serializer object
 * 
 * @param allOnOneLine 
 */
JsonSerializer::JsonSerializer(bool allOnOneLine) noexcept :
    allOnOneLine(allOnOneLine) {}

/**
 * @brief setHeaderComments
 * 
 * @param comments 
 */
void JsonSerializer::setHeaderComments(StringArray comments) noexcept {
    this->headerComments = comments;
}

/**
 * @brief saveToFile
 * 
 * @param file 
 * @param tree 
 * @return Result 
 */
Result JsonSerializer::saveToFile(File file, const SerializedData &tree) const {
    FileOutputStream fileStream(file);
    if (fileStream.openedOk())
    {
        fileStream.setPosition(0);
        fileStream.truncate();

        JsonFormatter::write(fileStream, tree, this->headerComments, 0, this->allOnOneLine, 6);
        return Result::ok();
    }

    return Result::fail("Failed to save");
}

/**
 * @brief loadFromFile
 * 
 * @param file 
 * @return SerializedData 
 */
SerializedData JsonSerializer::loadFromFile(const File &file) const {
    const String text(file.loadFileAsString());
    SerializedData root(fakeRoot);
    const auto result = JsonParser::parseObjectOrArray(text.getCharPointer(), root);
    if (result.wasOk())
    {
        return root.getChild(0);
    }

    return {};
}

/**
 * @brief saveToString
 * 
 * @param string 
 * @param tree 
 * @return Result 
 */
Result JsonSerializer::saveToString(String &string, const SerializedData &tree) const
{
    MemoryOutputStream mo(1024);
    JsonFormatter::write(mo, tree, this->headerComments, 0, this->allOnOneLine, 6);
    string = mo.toUTF8();
    return Result::ok();
}

/**
 * @brief loadFromFile
 * 
 * @param string 
 * @return SerializedData 
 */
SerializedData JsonSerializer::loadFromString(const String &string) const
{
    SerializedData root(fakeRoot);
    const auto result = JsonParser::parseObjectOrArray(string.getCharPointer(), root);
    if (result.wasOk())
    {
        if (root.getNumChildren() == 1 && root.getNumProperties() == 0)
        {
            return root.getChild(0);
        }
        else
        {
            return root;
        }
    }

    return {};
}

/**
 * @brief supports File With Extension
 * 
 * @param extension 
 * @return true 
 * @return false 
 */
bool JsonSerializer::supportsFileWithExtension(const String &extension) const {
    return extension.endsWithIgnoreCase("json");
}

/**
 * @brief supports file with header 
 * 
 * @param header 
 * @return true 
 * @return false 
 */
bool JsonSerializer::supportsFileWithHeader(const String &header) const
{
    return header.startsWithChar('[') || header.startsWithChar('{');
}
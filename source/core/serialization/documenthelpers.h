/**
 * @file documenthelpers.h
 * @author Krisna Pranav
 * @brief DocumentHelpers
 * @version 0.1
 * @date 2023-02-14
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once

class DocumentHelpers final {
public:
    static String getTemporaryFolder();
    static File getDocumentSlot(const String &fileName);
    static File getConfigSlot(const String &fileName);
    static File getTempSlot(const String &fileName);

    template <typename T>
    static SerializedData read(const String &data)
    {
        static T serializer;
        return serializer.loadFromString(data);
    }

    class TempDocument final
    {
    public:
        explicit TempDocument(const File &target);
        ~TempDocument();

    private:
        const File targetFile;
        const File temperovaryFile;
    }
}
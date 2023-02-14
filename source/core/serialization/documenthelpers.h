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

    /**
     * @brief Get the Temporary Folder object
     * 
     * @return String 
     */
    static String getTemporaryFolder();

    /**
     * @brief Get the Document Slot object
     * 
     * @param fileName 
     * @return File 
     */
    static File getDocumentSlot(const String &fileName);

    /**
     * @brief Get the Config Slot object
     * 
     * @param fileName 
     * @return File 
     */
    static File getConfigSlot(const String &fileName);

    /**
     * @brief Get the Temp Slot object
     * 
     * @param fileName 
     * @return File 
     */
    static File getTempSlot(const String &fileName);

    static void showFileChooser(UniquePointer<FileChooser> &chooser,
        int flags, Function<void(URL &url)> successCallback);

    /**
     * @brief Read
     * 
     * @tparam T 
     * @param data 
     * @return SerializedData 
     */
    template <typename T>
    static SerializedData read(const String &data)
    {
        static T serializer;
        return serializer.loadFromString(data);
    }

    static SerializedData load(const File &file);
    static SerializedData load(const String &string);

    /**
     * @brief Load
     * 
     * @tparam T 
     * @param file 
     * @return SerializedData 
     */
    template <typename T>
    static SerializedData load(const File &file)
    {
        static T serializer;
        return serializer.loadFromFile(file);
    }

    template <typename T>
    static SerializedData load(InputStream &stream)
    {
        static T serializer;
        return serializer.loadFromStream(stream);
    }


    /**
     * @brief Save
     * 
     * @tparam T 
     * @param file 
     * @param tree 
     * @return true 
     * @return false 
     */
    template <typename T>
    static bool save(const File &file, const SerializedData &tree)
    {
        static T serializer;
        TempDocument tempDoc(file);
        if (serializer.saveToFile(tempDoc.getFile(), tree).wasOk())
        {
            return tempDoc.overwriteTargetFileWithTemporary();
        }

        return false;
    }

    template <typename T>
    static bool save(const File &file, const Serializable &serializable)
    {
        static T serializer;
        TempDocument tempDoc(file);
        const auto treeNode(serializable.serialize());
        if (serializer.saveToFile(tempDoc.getFile(), treeNode).wasOk())
        {
            return tempDoc.overwriteTargetFileWithTemporary();
        }

        return false;
    }

    /**
     * @brief TempDocument
     * 
     */
    class TempDocument final {
    public:

        /**
         * @brief Construct a new Temp Document object
         * 
         * @param target 
         */
        explicit TempDocument(const File &target);
        ~TempDocument();

        /**
         * @brief Get the File object
         * 
         * @return const File& 
         */
        const File &getFile() const noexcept;
        const File &getTargetFile() const noexcept;

        /**
         * @brief overwrite target file with tempervary + delete temporvary file
         * 
         * @return true 
         * @return false 
         */
        bool overwriteTargetFileWithTemporary() const;
        bool deleteTemporaryFile() const;

    private:

        /**
         * @brief target file, temporary file
         * 
         */
        const File targetFile;
        const File temporaryFile;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TempDocument)
    };
};
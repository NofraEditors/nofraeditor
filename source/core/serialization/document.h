/**
 * @file document.h
 * @author Krisna Pranav
 * @brief Document functionalities
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once 

class DocumentOwner;

class Document : public ChangeListener {
public:
    Document(DocumentOwner &documentOwner,
            const String &defaultName,
            const String &defaultExtension);
    Document(DocumentOwner &documentOwner
            const File &existingFile)

    ~Document() override;

    String getFullPath() const;
    void revealToUser() const;

    void renameFile(const String &newName);

    void save();
    void exportAs(const String &exportExtension,
                const String &defaultFilename = "");

    bool load(const File &file);
    void import(const String &filePath);
    
}
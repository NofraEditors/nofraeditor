/**
 * @file documentowner.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

#include "document.h"

class DocumentOwner : public virtual ChangeBroadcaster
{
public:

    explicit DocumentOwner(const File &existingFile) :
        document(make<Document>(*this, existingFile)) {}

    DocumentOwner(const String &name, const String &extension) :
        document(make<Document>(*this, name, extension)) {}

    Document *getDocument() const noexcept
    {
        return this->document.get();
    }

protected:

    virtual bool onDocumentLoad(const File &file) = 0;
    virtual bool onDocumentSave(const File &file) = 0;
    virtual void onDocumentImport(InputStream &stream) = 0;
    virtual bool onDocumentExport(OutputStream &stream) = 0;

    friend class Document;

private:

    const UniquePointer<Document> document;

};
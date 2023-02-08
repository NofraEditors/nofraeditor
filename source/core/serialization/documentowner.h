/**
 * @file documentowner.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once 

#include "document.h"

class DocumentOwner : public virtual ChangeBroadcaster {
public:
    explicit DocumentOwner(const File &existingFile) : 
        document(make<Document>(*this, existingFile)) {}
    
    DocumentOwner(const String &name, const String &extension) :
        document(make<Document>(*this, name, extension)) {}
    
    Document *getDocument() const noexcept {
        return this->document.get();
    }

}
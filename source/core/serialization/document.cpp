/**
 * @file document.cpp
 * @author Krisna Pranav
 * @brief Document Source
 * @version 0.1
 * @date 2023-02-17
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#include "common.h"
#include "document.h"
#include "documenthelpers.h"
#include "documentowner.h"
#include "mainlayout.h"

Document::Document(DocumentOwner &documentOwner,
                  const String &defaultName,)
{
    if(defaultName.isNotEmpty()) {
        //
    }

    this->owner.addChangeListener(this);
}
/**
 * @file autosaver.cpp
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-02-08
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#include "common.h"
#include "autosaver.h"
#include "documentowner.h"

Autosaver::Autosaver(DocumentOwner &targetDocumentOwner, int waitDelayMs) :
    documentOwner(targetDocumentOwner),
    delay(waitDelayMs)
{
    this->documentOwner.addChangeListener(this);
}

Autosaver::~Autosaver()
{
    this->documentOwner.removeChangeListener(this);
}

void Autosaver::timeCallback() 
{
    this->stopTimer();
    this->documentOwner.getDocument()->save();
}
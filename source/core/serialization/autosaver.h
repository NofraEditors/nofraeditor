/**
 * @file autosaver.h
 * @author Krisna Pranav
 * @brief AutoSaver
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

#include "JuceHeader.h"

class DocumentOwner;

class Autosaver final : private Timer {
public:
    explicit Autosaver(DocumentOwner &targetDocumentOwner, int waitDelayMs = 3000);

    ~Autosaver() override;

private:
    const int delay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Autosaver)
}
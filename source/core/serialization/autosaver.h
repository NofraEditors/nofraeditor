/**
 * @file autosaver.h
 * @author Krisna Pranav
 * @brief AutoSaver
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once

#include "JuceHeader.h"

class DocumentOwner;

class Autosaver final :
    private ChangeListener,
    private Timer
{
public:

    explicit Autosaver(DocumentOwner &targetDocumentOwner, int waitDelayMs = 30000);

    ~Autosaver() override;

private:

    void changeListenerCallback(ChangeBroadcaster *source) override;

    void timerCallback() override;

    DocumentOwner &documentOwner;

    const int delay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Autosaver)

};
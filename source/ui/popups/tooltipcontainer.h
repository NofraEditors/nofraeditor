/**
 * @file tooltipcontainer.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-02-26
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

class TooltipContainer final : public Component, private Timer {
public:
    TooltipContainer();
    ~TooltipContainer();

private:
    int hideTimeout = -1;
    int timeCounter = -1; 
    bool alignedToBottom = true;

    void timerCallback() override;
    void updatePosition();

    static constexpr auto timerMs = 100;
};
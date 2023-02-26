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

/**
 * @brief Tool Tip Container
 * 
 */
class TooltipContainer final : public Component, private Timer {
public:

    TooltipContainer();
    ~TooltipContainer();

    void showWithComponent(UniquePointer<Component> targetComponent,
        int timeOutMs = -1);

    void showWithComponent(UniquePointer<Component> targetComponent,
        Rectangle<int> callerScreenBounds, int timeOutMs = -1);

    void hide();

    void paint(Graphics &g) override;
    void resized() override;
    void parentHierarchyChanged() override;
    void parentSizeChanged() override;

private:

    int hideTimeout = -1;
    int timeCounter = -1;
    bool alignedToBottom = true;
    int clicksCountOnStart = 0;

    ComponentAnimator animator;

    void timerCallback() override;
    void updatePosition();

    UniquePointer<Component> tooltipComponent;

    static constexpr auto timerMs = 100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TooltipContainer)
};
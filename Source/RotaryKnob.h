#pragma once

#include <JuceHeader.h>

class RotaryKnob : public juce::Component
{
public:
    RotaryKnob(const juce::String& text,
        juce::AudioProcessorValueTreeState& apvts,
        const juce::ParameterID& parameterID,
        bool drawsFromMid);
    ~RotaryKnob() override = default;

    void resized() override;

    juce::Slider slider;
    juce::Label label;
private:

    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnob)
};
#pragma once

#include <JuceHeader.h>

namespace ParamID
{
    const juce::ParameterID overdrive{ "overdrive", 1 };
    const juce::ParameterID muffle{ "muffle", 1 };
    const juce::ParameterID output{ "output", 1 };
    const juce::ParameterID mix{ "mix", 1 };
}

class Params
{
public:
    Params(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void update() noexcept;
    void reset() noexcept;

    float _overdrive = 0.0f;
    float _muffle = 0.0f;
    float _gain = 0.0f;
    float _mix = 0.5f;
    float _filtL, _filtR; // delay units

private:
    juce::AudioParameterFloat* overdriveParam;
    juce::AudioParameterFloat* muffleParam;
    juce::AudioParameterFloat* outputParam;
    juce::AudioParameterFloat* mixParam;
   
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Params)
};

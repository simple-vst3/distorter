#include "Params.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

Params::Params(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, ParamID::overdrive, overdriveParam);
    castParameter(apvts, ParamID::muffle, muffleParam);
    castParameter(apvts, ParamID::output, outputParam);
    castParameter(apvts, ParamID::mix, mixParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Params::createParameterLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::overdrive,
        "Overdrive",
        juce::NormalisableRange<float> {0.0f, 100.0f, 0.1f},
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::muffle,
        "Muffle",
        juce::NormalisableRange<float> {0.0f, 100.0f, 0.1f},
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::output,
        "Output",
        juce::NormalisableRange<float> {-10.0f, 10.0f, 0.1f},
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParamID::mix,
        "Mix",
        juce::NormalisableRange<float> {0.0f, 100.0f, 0.1f},
        100.0f,
        juce::AudioParameterFloatAttributes().withLabel("%")));

    return layout;
}

void Params::update() noexcept
{
    // convert these from range [0,100] - > [0, 1]
    _overdrive = overdriveParam->get() * 0.01f;
    _mix = mixParam->get() * 0.01f; 

    float muff = muffleParam->get() * .01f;
    _muffle = std::pow(10.0f, -1.6f * muff);

    float output = outputParam->get();
    _gain = juce::Decibels::decibelsToGain(output);
}

void Params::reset() noexcept
{
    _filtL = _filtR = 0.0f;
}
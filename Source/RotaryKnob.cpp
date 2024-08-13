#include <JuceHeader.h>
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
RotaryKnob::RotaryKnob(const juce::String& text,
    juce::AudioProcessorValueTreeState& apvts,
    const juce::ParameterID& parameterID,
    bool drawsFromMid)
    : attachment(apvts, parameterID.getParamID(), slider)
{

    slider.getProperties().set("drawsFromMid", drawsFromMid);

    float pi = juce::MathConstants<float>::pi;
    slider.setRotaryParameters(1.25f * pi, 2.75f * pi, true);

    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 16);
    slider.setTextValueSuffix(" " + apvts.getParameter(parameterID.getParamID())->getLabel());
    slider.setBounds(0, 0, 70, 86);
    addAndMakeVisible(slider);

    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setBorderSize(juce::BorderSize<int>(0));
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);

    setLookAndFeel(RotaryKnobLookAndFeel::get());
    setSize(70, 110);
}

void RotaryKnob::resized()
{
    slider.setTopLeftPosition(0, 24);
}
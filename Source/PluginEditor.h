/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "Params.h" 
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class DistorterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistorterAudioProcessorEditor (DistorterAudioProcessor&);
    ~DistorterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DistorterAudioProcessor& audioProcessor;

    // new ----
    const int WIDTH = 480, HEIGHT = 200;
    MainLookAndFeel mainLF; 
    RotaryKnob driveKnob{ "overdrive", audioProcessor.apvts, ParamID::overdrive, false };
    RotaryKnob crushKnob{ "muffle", audioProcessor.apvts, ParamID::muffle, false };
    RotaryKnob outputKnob{ "output", audioProcessor.apvts, ParamID::output, true /*starts knob at center*/};
    RotaryKnob mixKnob{ "mix", audioProcessor.apvts, ParamID::mix, false };

    juce::Label logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistorterAudioProcessorEditor)
};

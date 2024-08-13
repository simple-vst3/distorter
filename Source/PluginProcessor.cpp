/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistorterAudioProcessor::DistorterAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
    apvts(*this, nullptr, "Parameters", Params::createParameterLayout()),
    params(apvts)
{
}

DistorterAudioProcessor::~DistorterAudioProcessor()
{
}

//==============================================================================
const juce::String DistorterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistorterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistorterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistorterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistorterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistorterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistorterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistorterAudioProcessor::setCurrentProgram (int)
{
}

const juce::String DistorterAudioProcessor::getProgramName (int)
{
    return {};
}

void DistorterAudioProcessor::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
void DistorterAudioProcessor::prepareToPlay (double, int)
{
    params.reset();
}

void DistorterAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistorterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistorterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Overdrive implementation -------------------------------------------

    params.update(); // get most recent value from each parameter

    const float* in1 = buffer.getReadPointer(0);
    const float* in2 = buffer.getReadPointer(1);
    float* out1 = buffer.getWritePointer(0);
    float* out2 = buffer.getWritePointer(1);

    const float drive = params._overdrive;
    const float f = params._muffle;
    const float gain = params._gain;
    const float mix = params._mix;

    float fa = params._filtL, fb = params._filtR;

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
       
        float dryA = in1[i]; // dry samples read in
        float dryB = in2[i];

        // processing -----------------------------

        float aa = 5.f * dryA / (5.f*std::abs(dryA) + 1);
        float bb = 5.f * dryB / (5.f*std::abs(dryB) + 1);

        // Filter (exponentially weighted moving average)
        fa = fa + f * (drive * (aa - dryA) + dryA - fa);
        fb = fb + f * (drive * (bb - dryB) + dryB - fb);

        // mix addition, 
        float mixL = mix * fa + (1.0f - mix) * dryA;
        float mixR = mix * fb + (1.0f - mix) * dryB;
        
        // new output -------------------------------
        out1[i] = mixL * gain;
        out2[i] = mixR * gain;
    }

    // Catch denormals
    if (std::abs(fa) > 1.0e-10f) params._filtL = fa; else params._filtL = 0.0f;
    if (std::abs(fb) > 1.0e-10f) params._filtR = fb; else params._filtR = 0.0f;
}

//==============================================================================
bool DistorterAudioProcessor::hasEditor() const
{
    return true; // we are not supplying an editor yet
}

juce::AudioProcessorEditor* DistorterAudioProcessor::createEditor()
{
    return new DistorterAudioProcessorEditor (*this);
}

//==============================================================================
void DistorterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void DistorterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistorterAudioProcessor();
}

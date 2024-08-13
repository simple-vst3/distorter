
#pragma once

#include <JuceHeader.h>

const juce::String PLUGIN_NAME{ "distorter" };

namespace Colours
{
	const juce::Colour background{ 0xff141414 };
	const juce::Colour boxOutline{ 0xff666666 };
	const juce::Colour textColour{ 0xffa0a0a0 };
	const juce::Colour midOutline{ 0xff353535 };

	namespace Knob
	{
		const juce::Colour trackBackground{ midOutline };
		const juce::Colour trackActive{ 0xfff77e0c };
		const juce::Colour outline{ 0xff292724 };
		const juce::Colour gradientTop{ 0xff858382 };
		const juce::Colour gradientBottom{ 0xff000000 };
		const juce::Colour dial{ 0xffebebeb };
		const juce::Colour dropShadow{ 195, 190, 185 };
	}
}


class RotaryKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
	RotaryKnobLookAndFeel();

	static RotaryKnobLookAndFeel* get()
	{
		static RotaryKnobLookAndFeel instance;
		return &instance;
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos, float rotaryStartAngle,
		float rotaryEndAngle, juce::Slider& slider) override;

private:
	juce::DropShadow dropShadow{ Colours::Knob::dropShadow, 6 /*radius*/, {0,3} /*offset*/ };
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryKnobLookAndFeel)
};


class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MainLookAndFeel() {};
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLookAndFeel)
};
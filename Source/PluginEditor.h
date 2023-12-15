/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GlideAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GlideAudioProcessorEditor (GlideAudioProcessor&);
    ~GlideAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GlideAudioProcessor& audioProcessor;
    juce::Slider glideRateSlider;
    juce::Slider glideDepthSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlideAudioProcessorEditor)
};

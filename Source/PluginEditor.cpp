/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GlideAudioProcessorEditor::GlideAudioProcessorEditor (GlideAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    glideRateSlider.setSliderStyle(juce::Slider::Rotary);
    glideRateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    glideRateSlider.setRange(0.1f, 10.0f, 0.1f);
    addAndMakeVisible(&glideRateSlider);
    
    glideDepthSlider.setSliderStyle(juce::Slider::Rotary);
    glideDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    glideDepthSlider.setRange(0.1f, 10.0f, 0.1f);
    addAndMakeVisible(&glideDepthSlider);
  
    setSize (400, 300);
}

GlideAudioProcessorEditor::~GlideAudioProcessorEditor()
{
}

//==============================================================================
void GlideAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Panning Stuff", getLocalBounds(), juce::Justification::centred, 1);
}

void GlideAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    glideRateSlider.setBounds (40, 30, 100, 100);
    glideDepthSlider.setBounds (150, 30, 100, 100);
}

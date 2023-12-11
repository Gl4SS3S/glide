/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GlideAudioProcessor::GlideAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
  // Initalise the parameters with default values
  rate = new juce::AudioParameterFloat({"glideRate_1090", 1}, "Rate", 0.1f, 10.0f,1.0f);
  depth = new juce::AudioParameterFloat({"glideDepth_1090", 1}, "Depth", 0.1f, 10.0f,1.0f);
  
  // Add parameters to the processor
  addParameter(rate);
  addParameter(depth);
  
  // Initialise non-parameter member variables
  phase = 0.0;
  phaseIncrement = 0.0;
}

GlideAudioProcessor::~GlideAudioProcessor()
{
}

//==============================================================================
const juce::String GlideAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GlideAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GlideAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GlideAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GlideAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GlideAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GlideAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GlideAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GlideAudioProcessor::getProgramName (int index)
{
    return {};
}

void GlideAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GlideAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    phase = 0.0;
  
    phaseIncrement = rate->get() * juce::MathConstants<double>::twoPi / sampleRate;
}

void GlideAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GlideAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GlideAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unwanted noise
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Calculate phase increment for the auto-pan effect
    phaseIncrement = (*rate) * juce::MathConstants<double>::twoPi / getSampleRate();
  
    // Process each sample
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      float panValue = (std::sin(phase) + 1.0f) / 2.0f;
      phase += phaseIncrement;
      if (phase >= juce::MathConstants<double>::twoPi) {
        phase -= juce::MathConstants<double>::twoPi;
      }
      
      // Auto-pan effect to stereo channels
      for (int channel = 0; channel < totalNumInputChannels; ++channel)
      {
        auto* channelData = buffer.getWritePointer (channel);
        
        float gain = (channel == 0) ? 1.0f - (*depth) * panValue // Left Channel
                                    : 1.0f - (*depth) * (1.0f - panValue); // Right channel
        
        channelData[sample] *= gain;
      }
    }
}

//==============================================================================
bool GlideAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GlideAudioProcessor::createEditor()
{
    return new GlideAudioProcessorEditor (*this);
}

//==============================================================================
void GlideAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GlideAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GlideAudioProcessor();
}

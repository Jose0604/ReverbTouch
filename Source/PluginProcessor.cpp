/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PruebaAudioProcessor::PruebaAudioProcessor()
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
    /* Initial Parameters */
    Fs = 44100;
    reverbTime = 1;
    h = 0.7;

    /* Comb Filter Times */
    delayedTimes[0] = 30e-3;
    delayedTimes[1] = delayedTimes[0] * juce::MathConstants<float>::euler / 2;
    delayedTimes[2] = delayedTimes[0] * juce::MathConstants<float>::sqrt2;
    delayedTimes[3] = delayedTimes[0]*1.5;

    /* All Pass Filter Times */
    delayedTimes[4] = 3e-3;
    delayedTimes[5] = 5e-3;

    for (int n = 0; n < numberOfCircularBuffer; ++n)
    {
        multiChannelCircularBuffer.add(new circularBuffer());
    }

    /* State Parameters */
    isPluginOn = true;
    isHSActive = false;
}

PruebaAudioProcessor::~PruebaAudioProcessor()
{
}

//==============================================================================
const juce::String PruebaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PruebaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PruebaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PruebaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PruebaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PruebaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PruebaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PruebaAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PruebaAudioProcessor::getProgramName (int index)
{
    return {};
}

void PruebaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PruebaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    Fs = sampleRate;
    int x = getTotalNumInputChannels();

    for (int nBuffer = 0;nBuffer<numberOfCircularBuffer;++nBuffer)
    {
        multiChannelCircularBuffer[nBuffer]->updateParameters(2,x,Fs);
        multiChannelCircularBuffer[nBuffer]->updateDelayTime(delayedTimes[nBuffer]);
    }

    HS.updateNumInputChannels(x);
    HS.updateSampleRate(sampleRate);
}

void PruebaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PruebaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PruebaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            const float* in = buffer.getReadPointer(channel);
            float* out = buffer.getWritePointer(channel);
            if (isPluginOn)
            {
                float yComb = 0;
                for (int nBuffer = 0; nBuffer < numberOfCircularBuffer - 2; ++nBuffer)
                {
                    float feedback = pow(10, (-3 * delayedTimes[nBuffer] / reverbTime));

                    yComb = yComb + 0.125 * pow(-1, nBuffer) * multiChannelCircularBuffer[nBuffer]->requestSample(
                        channel, in[sample], feedback);
                }
                yComb = multiChannelCircularBuffer[4]->requestSample(true, channel, yComb, 0.7);
                out[sample] = (1 - h) * out[sample] + h * multiChannelCircularBuffer[5]->requestSample(true, channel, yComb, 0.7);

                out[sample] = HS.filter(out[sample], channel, isHSActive);
            }
        }
    }

}

//==============================================================================
bool PruebaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PruebaAudioProcessor::createEditor()
{
    return new PruebaAudioProcessorEditor (*this);
}

//==============================================================================
void PruebaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PruebaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PruebaAudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "filmStrip.h" 

//==============================================================================
/**
*/
class PruebaAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PruebaAudioProcessorEditor (PruebaAudioProcessor&);
    ~PruebaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    std::unique_ptr<FilmStripKnob> RT60, dry_Wet;

    std::unique_ptr<FilmStripKnob> fHS, gHS;
    juce::ToggleButton highshelv;

    juce::ToggleButton bypass;
    PruebaAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PruebaAudioProcessorEditor)
};

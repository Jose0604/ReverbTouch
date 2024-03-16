/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PruebaAudioProcessorEditor::PruebaAudioProcessorEditor (PruebaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    addAndMakeVisible(bypass);
    bypass.setButtonText("Bypass");
    bypass.setToggleState(false, juce::dontSendNotification);
    bypass.onClick = [this]
    {
        audioProcessor.isPluginOn = !bypass.getToggleState();
    };

    /* Knobs*/
    // -----------------------------------------------------------------------------
    juce::Image fknob = juce::ImageFileFormat::loadFrom(BinaryData::Donald_Duck_png,
        BinaryData::Donald_Duck_pngSize);
    juce::Image gKnob = juce::ImageFileFormat::loadFrom(BinaryData::knob_png,
        BinaryData::knob_pngSize);
    juce::Image timeKnob = juce::ImageFileFormat::loadFrom(BinaryData::Felix_Orange_png,
        BinaryData::Felix_Orange_pngSize);
    juce::Image dryWet = juce::ImageFileFormat::loadFrom(BinaryData::Royal_Flush_png,
        BinaryData::Royal_Flush_pngSize);
    // -----------------------------------------------------------------------------
    

    RT60.reset(new FilmStripKnob(timeKnob, 31, false));
    addAndMakeVisible(RT60.get());
    RT60->setRange(250, 5e3, 0.1);
    RT60->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    RT60->setTextValueSuffix("ms");
    RT60->setSkewFactorFromMidPoint(1e3);
    RT60->setValue(1e3, juce::dontSendNotification);
    RT60->setTooltip(juce::String("Reverb Time [ms]"));
    RT60->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 30);
    RT60->onValueChange = [this]
    {
        audioProcessor.reverbTime = RT60->getValue() * 1e-3;       
    };

    dry_Wet.reset(new FilmStripKnob(dryWet, 31, false));
    addAndMakeVisible(dry_Wet.get());
    dry_Wet->setRange(0, 1, 0);
    dry_Wet->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    dry_Wet->setTextValueSuffix("ms");
    dry_Wet->setValue(0.7, juce::dontSendNotification);
    dry_Wet->setTooltip(juce::String("Dry/Wet"));
    dry_Wet->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 30);
    dry_Wet->onValueChange = [this]
    {
        audioProcessor.h = dry_Wet->getValue();
    };

    /* High Shelv */
    // -----------------------------------------------------------------------------
    addAndMakeVisible(highshelv);
    highshelv.setButtonText("High Shelv");
    highshelv.onClick = [this]
    {
        audioProcessor.isHSActive = highshelv.getToggleState();
    };

    fHS.reset(new FilmStripKnob(fknob, 31, false));
    addAndMakeVisible(fHS.get());
    fHS->setRange(1e3, 16e3, 0.1);
    fHS->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    fHS->setTextValueSuffix("Hz");
    fHS->setValue(8e3, juce::dontSendNotification);
    fHS->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    fHS->setSkewFactorFromMidPoint(8e3);
    fHS->onValueChange = [this]
    {
        audioProcessor.HS.updateCutFrequency(fHS->getValue());
    };

    gHS.reset(new FilmStripKnob(gKnob, 100, false));
    addAndMakeVisible(gHS.get());
    gHS->setRange(-12, 12, 0.1);
    gHS->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gHS->setValue(0, juce::dontSendNotification);
    gHS->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 70, 20);
    gHS->onValueChange = [this]
    {
        audioProcessor.HS.updateGdB(gHS->getValue());
    };
    // -----------------------------------------------------------------------------

    setSize(400, 220);

    bypass.setBounds(10, 10, 80, 30);

    //RT60->setBounds(80, 150, 80,80);
    RT60->setBounds(20, 100, 80, 80);
    dry_Wet->setBounds(110, 100, 80, 80);

    highshelv.setBounds(250, 70, 80, 30);
    fHS->setBounds(210, 100, 80, 80);
    gHS->setBounds(300, 100, 80, 80);
    
}

PruebaAudioProcessorEditor::~PruebaAudioProcessorEditor()
{

}

//==============================================================================
void PruebaAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Image background = juce::ImageFileFormat::loadFrom(
        BinaryData::stevejohnsondVRD8E3XUGsunsplash_jpg,
        BinaryData::stevejohnsondVRD8E3XUGsunsplash_jpgSize);
    juce::Rectangle<float>  interfaz(0, 0, getWidth(), getHeight());

    juce::Image texttitle = juce::ImageFileFormat::loadFrom(
        BinaryData::logo2_png, BinaryData::logo2_pngSize);
    juce::Rectangle<float> title(97, 25, 257*4/5,59*4/5);

    g.drawImage(background, interfaz,
        juce::RectanglePlacement::doNotResize);
    g.drawImage(texttitle, title, juce::RectanglePlacement::centred);
    
    g.drawFittedText("Made by Jose", getLocalBounds(),
        juce::Justification::bottomLeft, 1);
}

void PruebaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

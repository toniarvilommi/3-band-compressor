/*
  ==============================================================================
 
    This file contains the basic framework code for a JUCE plugin editor.
 
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_3bandcompressorAudioProcessorEditor::_3bandcompressorAudioProcessorEditor (_3bandcompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set editor size
    setSize (400, 300);

    // === THRESHOLD SLIDER ===
    thresholdSlider.setSliderStyle(juce::Slider::Rotary);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(thresholdSlider);

    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Threshold", thresholdSlider);

    // === ATTACK SLIDER ===
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(attackSlider);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Attack", attackSlider);

    // === RELEASE SLIDER ===
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(releaseSlider);

    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Release", releaseSlider);
}

_3bandcompressorAudioProcessorEditor::~_3bandcompressorAudioProcessorEditor()
{
}

//==============================================================================
void _3bandcompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Draw title
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (15.0f)); // Fixed: FontOptions → Font
    g.drawFittedText ("3-Band Compressor", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void _3bandcompressorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20).removeFromTop(200);
    auto sliderWidth = 100;
    auto sliderHeight = 100;
    auto padding = 20;

    thresholdSlider.setBounds(area.removeFromLeft(sliderWidth));
    area.removeFromLeft(padding); // space between sliders

    attackSlider.setBounds(area.removeFromLeft(sliderWidth));
    area.removeFromLeft(padding);

    releaseSlider.setBounds(area.removeFromLeft(sliderWidth));
}

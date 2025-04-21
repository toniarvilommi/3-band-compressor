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
    setSize (500, 400);

    // === THRESHOLD SLIDER ===
    addSliderWithLabel(thresholdSlider, thresholdLabel, "Threshold", "Threshold");
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Threshold", thresholdSlider);

    // === ATTACK SLIDER ===
    addSliderWithLabel(attackSlider, attackLabel, "Attack", "Attack");
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Attack", attackSlider);

    // === RELEASE SLIDER ===
    addSliderWithLabel(releaseSlider, releaseLabel, "Release", "Release");
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Release", releaseSlider);
    
    // === COMBOBOX ===
    addAndMakeVisible(ratioComboBox);

    // fetch and render choices from apvts
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter("Ratio")))
    {
        ratioComboBox.addItemList(choiceParam->choices, 1);
        
        ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            audioProcessor.apvts, "Ratio", ratioComboBox);
    }
    
    DBG("ComboBox items count: " + juce::String(ratioComboBox.getNumItems()));
}

void _3bandcompressorAudioProcessorEditor::addSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& paramID, const juce::String& labelText)
{
    // Slider
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible(slider);

    // Label
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&slider, true);
    label.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(label);
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
    g.setFont (juce::Font (15.0f));
    g.drawFittedText ("3-Band Compressor", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void _3bandcompressorAudioProcessorEditor::resized()
{

    const int margin = 10;
    
    const int sliderWidth = 300;
    const int sliderHeight = 40;
    
    const int ratioWidth = 300;
    const int ratioHeight = 20;
    

    thresholdSlider.setBounds(80, 30, sliderWidth, sliderHeight);
    attackSlider.setBounds(80, 30 + sliderHeight + margin, sliderWidth, sliderHeight);
    releaseSlider.setBounds(80, 30 + 2 * (sliderHeight + margin), sliderWidth, sliderHeight);
    ratioComboBox.setBounds(80, 30 + 2 * (sliderHeight + ratioHeight + margin), ratioWidth, ratioHeight);
}

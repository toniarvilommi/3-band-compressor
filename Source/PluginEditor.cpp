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
    addComponentWithLabel(thresholdSlider, thresholdLabel, "Threshold");
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Threshold", thresholdSlider);

    // === ATTACK SLIDER ===
    addComponentWithLabel(attackSlider, attackLabel, "Attack");
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Attack", attackSlider);

    // === RELEASE SLIDER ===
    addComponentWithLabel(releaseSlider, releaseLabel, "Release");
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "Release", releaseSlider);
    
    // === COMBOBOX ===
    // fetch and render choices from apvts
    // cast is checking if "Ratio" is nullptr (false) or AudioParameterChoice (true)
    addComponentWithLabel(ratioComboBox, ratioLabel, "Ratio");
    if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter("Ratio")))
    {
        ratioComboBox.addItemList(choiceParam->choices, 1);
        
        ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            audioProcessor.apvts, "Ratio", ratioComboBox);
    }
    
    
    
    // === CHECKBOX ===
    addComponentWithLabel(bypass, bypassLabel, "Bypass");
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "Bypass", bypass);

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
    const int startX = 120;
    const int startY = 30;
    const int width = 300;

    int currentY = startY;

    auto placeComponent = [&](juce::Component& comp, int height)
    {
        comp.setBounds(startX, currentY, width, height);
        currentY += height + margin;
    };

    placeComponent(thresholdSlider, 40);
    placeComponent(attackSlider, 40);
    placeComponent(releaseSlider, 40);
    placeComponent(ratioComboBox, 20);
    placeComponent(bypass, 20);
}


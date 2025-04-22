#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class _3bandcompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _3bandcompressorAudioProcessorEditor (_3bandcompressorAudioProcessor&);
    ~_3bandcompressorAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    template <typename ComponentType>
    void addComponentWithLabel(ComponentType& component, juce::Label& label, const juce::String& labelText, bool labelOnLeft = true)
    {
        // Add and configure the component
        addAndMakeVisible(component);

        // Configure the label
        label.setText(labelText, juce::dontSendNotification);
        label.attachToComponent(&component, labelOnLeft);
        label.setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(label);
    }

    
private:
    _3bandcompressorAudioProcessor& audioProcessor;

    void addSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& paramID, const juce::String& labelText);
    
    // === Sliderss ===
    juce::Slider thresholdSlider, attackSlider, releaseSlider;
    
    // === Combobox ===
    juce::ComboBox ratioComboBox;
    
    // === Labelss ===
    juce::Label thresholdLabel, attackLabel, releaseLabel, ratioLabel, bypassLabel;
    
    // === Checkbox ===
    juce::ToggleButton bypass;
    
    // === Attachments ===
    /** These attach the ui values to the PluginProcessor so you can alter the dsp with the ui values */
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ratioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_3bandcompressorAudioProcessorEditor)
};

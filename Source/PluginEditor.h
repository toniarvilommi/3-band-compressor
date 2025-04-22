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

private:
    _3bandcompressorAudioProcessor& audioProcessor;

    void addSliderWithLabel(juce::Slider& slider, juce::Label& label, const juce::String& paramID, const juce::String& labelText);
    
    // === Sliderss ===
    juce::Slider thresholdSlider, attackSlider, releaseSlider;
    
    // === Combobox ===
    juce::ComboBox ratioComboBox;
    
    // === Labelss ===
    juce::Label thresholdLabel, attackLabel, releaseLabel, ratioLabel;
    
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

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

    // === Sliders ===
    juce::Slider thresholdSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;

    // === Attachments ===
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_3bandcompressorAudioProcessorEditor)
};

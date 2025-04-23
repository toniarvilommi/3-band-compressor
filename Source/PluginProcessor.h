/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct CompressorBand
{
    // pointers to parameters that control the compressor
    juce::AudioParameterFloat* attack {nullptr};
    juce::AudioParameterFloat* release {nullptr};
    juce::AudioParameterFloat* threshold {nullptr};
    juce::AudioParameterChoice* ratio {nullptr};
    juce::AudioParameterBool* bypass {nullptr};
    
    // called to prepare the compressor for audio processing
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        compressor.prepare(spec);
    }
    
    // updates the compressor's internal settings from the parameter values
    void updateCompressorSettings()
    {
        compressor.setAttack(attack->get());
        compressor.setRelease(release->get());
        compressor.setThreshold(threshold->get());
        compressor.setRatio(ratio->getCurrentChoiceName().getFloatValue());
    }
    
    // processes the audio buffer through the compressor
    void process(juce::AudioBuffer<float>& buffer)
    {
        auto block = juce::dsp::AudioBlock<float>(buffer); // wrap buffer in a dsp block
        auto context = juce::dsp::ProcessContextReplacing<float>(block); // context for in-place processing
        
        bool isBypassed = bypass->get(); // check if we should bypass
        context.isBypassed = isBypassed; // set bypass flag
        
        compressor.process(context); // process the block
    }
    
private:
    juce::dsp::Compressor<float> compressor; // the actual juce dsp compressor
};


//==============================================================================
/**
*/
class _3bandcompressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    _3bandcompressorAudioProcessor();
    ~_3bandcompressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    // Manages all plugin parameters via createParameterLayout
    using APVTS = juce::AudioProcessorValueTreeState;
    static APVTS::ParameterLayout createParameterLayout();
    
    APVTS apvts { *this, nullptr, "Parameters", createParameterLayout() };

private:
    //==============================================================================
    
    // Define compressor band 1
    CompressorBand compressor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_3bandcompressorAudioProcessor)
};

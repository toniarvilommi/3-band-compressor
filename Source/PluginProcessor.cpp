/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_3bandcompressorAudioProcessor::_3bandcompressorAudioProcessor()
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
    // cast pointer to the apvts values for optimization
    // so there is only one value in the app we are changing
    // instead of doing million times in processblock
    compressor.attack = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Attack"));
    jassert(compressor.attack != nullptr);
    
    compressor.release = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Release"));
    jassert(compressor.release != nullptr);
    
    compressor.threshold = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Threshold"));
    jassert(compressor.threshold != nullptr);
    
    compressor.ratio = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("Ratio"));
    jassert(compressor.ratio != nullptr);
    
    compressor.bypass = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("Bypass"));
    jassert(compressor.bypass != nullptr);
}

_3bandcompressorAudioProcessor::~_3bandcompressorAudioProcessor()
{
}

//==============================================================================
const juce::String _3bandcompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _3bandcompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _3bandcompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _3bandcompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _3bandcompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _3bandcompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _3bandcompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void _3bandcompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _3bandcompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void _3bandcompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _3bandcompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Prepare compressor
    // Add spec object to compressor
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    compressor.prepare(spec);
}

void _3bandcompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _3bandcompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void _3bandcompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    compressor.updateCompressorSettings();
    compressor.process(buffer);
}

//==============================================================================
bool _3bandcompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* _3bandcompressorAudioProcessor::createEditor()
{
    return new _3bandcompressorAudioProcessorEditor (*this);
}

//==============================================================================
void _3bandcompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _3bandcompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}



// Define layout here
juce::AudioProcessorValueTreeState::ParameterLayout _3bandcompressorAudioProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout;

    // threshold
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Threshold", 1), "Threshold", juce::NormalisableRange<float>(-60.0f, 12.0f, 1.0f, 1.0f), 0.0f));

    // attack
    auto attackRange = juce::NormalisableRange<float>(5.0f, 500.0f, 1.0f, 1.0f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Attack", 1), "Attack", attackRange, 50));

    // release
    auto releaseRange = juce::NormalisableRange<float>(10.0f, 1000.0f, 1.0f, 1.0f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Release", 1), "Release", releaseRange, 250));

    // ratio
    juce::StringArray stringArray { "1", "1.5", "2", "3", "4", "5", "6", "7", "8", "9", "10", "15", "20", "50", "100" };

    DBG("Ratio Choices: " + stringArray.joinIntoString(", "));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("Ratio", 1), "Ratio", stringArray, 3));

    // bypass
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("Bypass", 1), "Bypass", false));

    return layout;
}





//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _3bandcompressorAudioProcessor();
}

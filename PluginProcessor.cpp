/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginBasicsAudioProcessor::AudioPluginBasicsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    treeState(*this, nullptr, "PARAMETER", createParameter())

#endif
    {
        treeState.state = ValueTree("savedParameters");
    }

AudioPluginBasicsAudioProcessor::~AudioPluginBasicsAudioProcessor()
{
}

//==============================================================================
const String AudioPluginBasicsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginBasicsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginBasicsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginBasicsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginBasicsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginBasicsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginBasicsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginBasicsAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPluginBasicsAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPluginBasicsAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioPluginBasicsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    previousGain = pow(10, *treeState.getRawParameterValue(GAIN_ID) / 20);
}

void AudioPluginBasicsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPluginBasicsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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


AudioProcessorValueTreeState::ParameterLayout AudioPluginBasicsAudioProcessor::createParameter()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    auto gainParam = std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, -48.0f, 0.0f, -3.0f);
    params.push_back(std::move(gainParam));

    return { params.begin(), params.end() };
}

void AudioPluginBasicsAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    float curGain = pow(10, *treeState.getRawParameterValue(GAIN_ID) / 20);



    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (curGain == previousGain)
    {
        buffer.applyGain(curGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, curGain);
        previousGain = curGain;
    }


    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto channelData = buffer.getWritePointer (channel);
    //    auto sliderValueGain = treeState.getRawParameterValue(GAIN_ID);

    //    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    //    {
    //        //channelData - âûõîäíàÿ ãðîìêîñòü. Ðàññ÷èòûâàåì âûõîäíóþ ãðîìêîñòü êàæäîãî ñýìïëà.
    //        channelData[sample] = buffer.getSample(channel, sample) * Decibels::decibelsToGain(static_cast<float>(*sliderValueGain));//gainValue;//Ðàññ÷¸ò âûõîäíîãî çíà÷åíèÿ: òåêóùèé ñýìïë óìíîæåííûé íà ãðîìêîñòü;
    //    }
    //}
}

//==============================================================================
bool AudioPluginBasicsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPluginBasicsAudioProcessor::createEditor()
{
    return new AudioPluginBasicsAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginBasicsAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginBasicsAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));
    if (theParams != nullptr)
    {
        if (theParams->hasTagName(treeState.state.getType()))
        {
            treeState.state = ValueTree::fromXml(*theParams);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginBasicsAudioProcessor();
}

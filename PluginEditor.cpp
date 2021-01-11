/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginBasicsAudioProcessorEditor::AudioPluginBasicsAudioProcessorEditor (AudioPluginBasicsAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    slValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, slGain);
    
    setSize (200, 400);
    slGain.setSliderStyle(Slider::SliderStyle::LinearVertical);
    slGain.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    slGain.setRange(-48.0f, 0.0f);
    addAndMakeVisible(&slGain);
}

AudioPluginBasicsAudioProcessorEditor::~AudioPluginBasicsAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginBasicsAudioProcessorEditor::paint(Graphics& g)
{
    Image background = ImageCache::getFromMemory(BinaryData::BACK_jpg, BinaryData::BACK_jpgSize);
    g.drawImageAt(background, 0, 0);
}

void AudioPluginBasicsAudioProcessorEditor::resized()
{
    slGain.setBounds(getLocalBounds());
}

void AudioPluginBasicsAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &slGain)
    {
        processor.rawGainValue = slGain.getValue();
    }
}
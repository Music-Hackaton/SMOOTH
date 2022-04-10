#pragma once

#include <shared_plugin_helpers/shared_plugin_helpers.h>

class MidiFXProcessor : public PluginHelpers::ProcessorBase
{
public:
    MidiFXProcessor()
    {
        note1 = new juce::AudioParameterInt("First", "First", 0, 127, 60);
        note2 = new juce::AudioParameterInt("Second", "Second", 0, 127, 64);
        note3 = new juce::AudioParameterInt("Third", "Third", 0, 127, 67);

        addParameter(note1);
        addParameter(note2);
        addParameter(note3);
    }

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;


    juce::AudioProcessorEditor* createEditor() override;
private:

    juce::AudioParameterInt* note1;
    juce::AudioParameterInt* note2;
    juce::AudioParameterInt* note3;

    std::vector<int> pressedNotes;
    juce::MidiBuffer tempBuffer;
    int noteToFind1[3] {50,51,52};
    int noteRecived[3] {-1,-1,-1};
    int openFlag=0;
    int openFlag1=0;


};

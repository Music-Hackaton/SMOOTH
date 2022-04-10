#include "PluginProcessor.h"
#include <vector>

int compareArraysContents(const  int* array1,const  int* array2 )
{
    for (int i = 0; i < 3; ++i)
    {
        if(array1[i]!=array2[i])
            return -1;
    }
    return 1;
}
int compareArraysContents2(const  int* array1,int note1,int note2,int note3 )
{
    if(note1==array1[0]&&note2==array1[1]&&note3==array1[2])
    {
        return 1;
    }
    return -1;
}

void MidiFXProcessor::processBlock(juce::AudioBuffer<float>& /*buffer*/,
                                   juce::MidiBuffer& midiMessages)

{
    note1->get();
    note2->get();
    note3->get();

    tempBuffer.clear();

    if (auto ph = getPlayHead())
    {
        juce::AudioPlayHead::CurrentPositionInfo info;
        ph->getCurrentPosition(info);
    }

    for (auto m: midiMessages)
    {
        auto message = m.getMessage();

        auto samplePos = m.samplePosition;
        if(message.isNoteOn())
        {
              noteRecived[0]=noteRecived[1];
              noteRecived[1]=noteRecived[2];
              noteRecived[2]=message.getNoteNumber();
              if (compareArraysContents2(noteRecived, note1->get(),
                                        note2->get(),
                                        note3->get())==1)      {
                  DBG("1");
                  if(openFlag==0)
                  {
                      auto newMessage = juce::MidiMessage::controllerEvent(1, 1, 127);
                      tempBuffer.addEvent(newMessage, samplePos);

                      openFlag=1;
                  }
                  else
                  {
                    auto newMessage = juce::MidiMessage::controllerEvent(1, 1, 0);
                      tempBuffer.addEvent(newMessage, samplePos);
                      openFlag=0;

                  }

              }else
              if (compareArraysContents(noteRecived,noteToFind1)==1)      {
                  DBG("2 N");
                  if(openFlag1==0)
                  {

                      auto newMessage1 = juce::MidiMessage::controllerEvent(1, 2, 127);
                      tempBuffer.addEvent(newMessage1, samplePos);
                      openFlag1=1;
                  }
                  else
                  {
                      auto newMessage = juce::MidiMessage::controllerEvent(1,2, 0);
                      tempBuffer.addEvent(newMessage, samplePos);
                      openFlag1=0;

                  }

              }
            }



            //First, debug all incoming messages to console:
            std::cout << "Incoming:" << message.getNoteNumber()
                      << " SamplePos:" << juce::String(samplePos) << std::endl;

            //Then, transpose all notes to be note #60
            if (message.isNoteOn() && message.getNoteNumber() == 101)
            {
                auto newMessage = juce::MidiMessage::controllerEvent(1, 100, 127);
                tempBuffer.addEvent(newMessage, samplePos);
            }
            else if (message.isNoteOff() && message.getNoteNumber() == 101)
            {
                //auto newMessage = juce::MidiMessage::controllerEvent(1, 100, 0);
                // tempBuffer.addEvent(newMessage, samplePos);
            }
            else
                tempBuffer.addEvent(message, samplePos);
        }


    midiMessages.swapWith(tempBuffer);
}

juce::AudioProcessorEditor* MidiFXProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiFXProcessor();
}



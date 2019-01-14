/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener
{
public:
    
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &freqSlider)
        {
            currentFrequency = freqSlider.getValue();
            
        } else if (slider == &ampSlider)
        {
            amplitude = ampSlider.getValue();
        }
    }
    
    
    void updateFrequency(const double& bufferSize)
    {
        if (targetFrequency != currentFrequency)
        {
           
        const double frequencyIncrement = (targetFrequency - currentFrequency) / bufferSize;
        increment = (currentFrequency + frequencyIncrement) * wtSize / currentSampleRate;
            
        phase = fmod ((phase + increment), wtSize);
        } else
        {
            increment = currentFrequency * wtSize / currentSampleRate;
            phase = fmod ((phase + increment), wtSize);
        }
    }

    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 100);
    
        freqSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        freqSlider.setRange(50.0, 5000.0);
        freqSlider.setTextValueSuffix("Hz");
        freqSlider.addListener(this);
        freqSlider.setValue(500.0);
        addAndMakeVisible(freqSlider);
        freqLabel.setText("Frequency", dontSendNotification);
        freqLabel.attachToComponent(&freqSlider, true);
       
        ampSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        ampSlider.setRange(0.0, 1.0);
        ampSlider.addListener(this);
        ampSlider.setValue(0.0);
        addAndMakeVisible(ampSlider);
        ampLabel.setText("Amplitude", dontSendNotification);
        ampLabel.attachToComponent(&ampSlider, true);
    
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        currentFrequency = freqSlider.getValue();
        targetFrequency = currentFrequency;
        amplitude = ampSlider.getValue();
        phase = 0;
        wtSize = 1024;
        currentSampleRate = sampleRate;
        
        //one cycle of a sine wave
        for (int i = 0; i < wtSize; i++)
        {
            waveTable.insert(i, sin(2.0 * double_Pi * i / wtSize));
        }
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

        const double bufferSize = bufferToFill.numSamples;

        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            leftSpeaker[sample] = waveTable[(int)phase] * amplitude;
            rightSpeaker[sample] = waveTable[(int)phase] * amplitude;
            updateFrequency(bufferSize);
        }
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        const int labelSpace = 100;
        freqSlider.setBounds(labelSpace, 20, getWidth() - 100, 20);
        ampSlider.setBounds(labelSpace, 50, getWidth() - 100, 50);
    }


private:
    Slider freqSlider, ampSlider;
    Label freqLabel, ampLabel;
   
    Array<double> waveTable;
    double wtSize;
    double currentFrequency, targetFrequency;
    double phase;
    double increment;
    double amplitude;
    double currentSampleRate;
    double bufferSize;
    
    //==============================================================================

    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }

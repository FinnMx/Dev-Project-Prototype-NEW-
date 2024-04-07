/*
  ==============================================================================

    TenBandComponent.cpp
    Created: 15 Feb 2024 9:38:04pm
    Author:  Finn

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TenBandComponent.h"

//==============================================================================
TenBandComponent::TenBandComponent(TenBandEQ* tenBandEQ) : tenBand(tenBandEQ)
{
    sliders.push_back(&freq30Slider);
    sliders.push_back(&freq62Slider);
    sliders.push_back(&freq125Slider);
    sliders.push_back(&freq250Slider);
    sliders.push_back(&freq500Slider);
    sliders.push_back(&freq1000Slider);
    sliders.push_back(&freq2000Slider);
    sliders.push_back(&freq4000Slider);
    sliders.push_back(&freq8000Slider);
    sliders.push_back(&freq16000Slider);

    sliderLabels.push_back(&freq30SliderLabel);
    sliderLabels.push_back(&freq62SliderLabel);
    sliderLabels.push_back(&freq125SliderLabel);
    sliderLabels.push_back(&freq250SliderLabel);
    sliderLabels.push_back(&freq500SliderLabel);
    sliderLabels.push_back(&freq1000SliderLabel);
    sliderLabels.push_back(&freq2000SliderLabel);
    sliderLabels.push_back(&freq4000SliderLabel);
    sliderLabels.push_back(&freq8000SliderLabel);
    sliderLabels.push_back(&freq16000SliderLabel);

    initSlider();
    addAndMakeVisible(resetButton);
    addAndMakeVisible(autoSetButton);
    resetButton.addListener(this);
    autoSetButton.addListener(this);
}

TenBandComponent::~TenBandComponent()
{
}

void TenBandComponent::handleMidi(int action, int value) {
    const juce::MessageManagerLock mmLock;
    switch (action) {
    case 0:
        freq30Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 1:
        freq62Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 2:
        freq125Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 3:
        freq250Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 4:
        freq500Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 5:
        freq1000Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 6:
        freq2000Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 7:
        freq4000Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 8:
        freq8000Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 9:
        freq16000Slider.setValue(juce::jmap((float)value, (float)0, (float)127, 0.5f, 7.0f));
        break;
    case 10:
        resetButton.triggerClick();
    }
}

void TenBandComponent::initSlider() {
    for (int i = 0; i <= 9; i++) 
    {
        addAndMakeVisible(sliders[i]);
        sliders[i]->addListener(this);
        sliders[i]->setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        sliders[i]->setRange(0.5f, +7.0f, 0.01f);
        sliders[i]->setSkewFactorFromMidPoint(1.f);
        sliders[i]->setValue(1.f);
        sliderLabels[i]->attachToComponent(sliders[i], false);
        sliderLabels[i]->setJustificationType(juce::Justification::centredBottom);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    }
}

void TenBandComponent::sliderValueChanged(juce::Slider* slider) {
    for (int i = 0; i <= 9; i++) {
        if (slider == sliders[i])
            tenBand->setNewGain(i, slider->getValue());
    }
}

void TenBandComponent::buttonClicked(juce::Button* button) {
    if (button == &resetButton){
        for each (juce::Slider* slider in sliders)
        {
            slider->setValue(1.0f);
        }
    }
    if (button == &autoSetButton) {
        tenBand->autoAdjustFrequencies(sliders);
    }
}

void TenBandComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (getLookAndFeel().findColour(juce::TextEditor::outlineColourId).contrasting(0.15f));
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawText("10 Band EQ", getLocalBounds(),
        juce::Justification::centredTop, true);
}

void TenBandComponent::resized()
{
    freq30Slider.setBounds(getWidth() * 0.005,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq62Slider.setBounds(getWidth() * 0.105,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq125Slider.setBounds(getWidth() * 0.205,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq250Slider.setBounds(getWidth() * 0.305,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq500Slider.setBounds(getWidth() * 0.405,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq1000Slider.setBounds(getWidth() * 0.505,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq2000Slider.setBounds(getWidth() * 0.605,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq4000Slider.setBounds(getWidth() * 0.705,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq8000Slider.setBounds(getWidth() * 0.805,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    freq16000Slider.setBounds(getWidth() * 0.905,
        getHeight() * 0.2,
        getWidth() * 0.1,
        getHeight() * 0.6);

    resetButton.setBounds(getWidth() * 0.85,
        getHeight() * 0.9,
        getWidth() * 0.15,
        getHeight() * 0.1);

    autoSetButton.setBounds(getWidth() * 0.005,
        getHeight() * 0.9,
        getWidth() * 0.15,
        getHeight() * 0.1);
}

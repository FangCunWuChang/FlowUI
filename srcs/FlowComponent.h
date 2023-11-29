#pragma once

#include <JuceHeader.h>

namespace flowUI {
	class FlowWindow;

	class FlowComponent : public juce::Component {
	public:
		FlowComponent(const juce::String& name);

		bool isOpened() const;

	protected:
		FlowWindow* getWindow() const;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlowComponent)
	};
}
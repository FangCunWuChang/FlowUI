#pragma once

#include <JuceHeader.h>

namespace flowUI {
	class FlowComponent : public juce::Component {
	public:
		FlowComponent(const juce::String& name);

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlowComponent)
	};
}
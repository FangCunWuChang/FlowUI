#pragma once

#include <JuceHeader.h>

namespace flowUI {
	class FlowWindow;

	class FlowMenu final {
	public:
		static juce::PopupMenu createContainerMenu(
			FlowWindow* currentWindow,
			std::function<void(int)> windowCallback,
			bool isTab,
			bool isFree,
			bool isMulti,
			bool isVertical);

	private:
		static juce::PopupMenu createWindowMenu(FlowWindow* currentWindow, std::function<void(int)> windowCallback);
	};
}

#include "FlowComponent.h"
#include "FlowWindow.h"

namespace flowUI {
	FlowComponent::FlowComponent(const juce::String& name)
		: juce::Component(name) {}

	FlowWindow* FlowComponent::getWindow() const {
		for (const juce::Component* currentComp = this;
			currentComp != nullptr; currentComp = currentComp->getParentComponent()) {
			if (auto ptr = dynamic_cast<const FlowWindow*>(currentComp)) {
				return const_cast<FlowWindow*>(ptr);
			}
		}
		return nullptr;
	}
}

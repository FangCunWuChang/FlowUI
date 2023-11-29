#include "FlowComponent.h"
#include "FlowWindow.h"
#include "FlowContainer.h"

namespace flowUI {
	FlowComponent::FlowComponent(const juce::String& name)
		: juce::Component(name) {}

	bool FlowComponent::isOpened() const {
		return dynamic_cast<FlowContainer*>(this->getParentComponent());
	}

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

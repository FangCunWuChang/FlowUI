﻿#include "FlowWindowContent.h"
#include "FlowWindow.h"

namespace flowUI {
	FlowWindowContent::FlowWindowContent(FlowWindow* parent)
		: parent(parent) {
		this->setWantsKeyboardFocus(true);

		this->manager = std::make_unique<FlowManager>(parent);
		this->addAndMakeVisible(this->manager.get());
	}

	FlowManager* FlowWindowContent::getManager() const {
		return this->manager.get();
	}

	void FlowWindowContent::setToolBar(FlowComponent* toolBar, double size) {
		this->removeToolBar();

		if (!toolBar) { return; }
		this->toolBar = toolBar;
		this->toolBarSize = size;
		this->addAndMakeVisible(toolBar);
		this->resized();
	}

	void FlowWindowContent::removeToolBar() {
		if (!this->toolBar) { return; }
		this->toolBar->setVisible(false);
		this->removeChildComponent(this->toolBar);
		this->toolBar = nullptr;
		this->toolBarSize = 0;
		this->resized();
	}

	void FlowWindowContent::resized() {
		auto screenSize = this->parent->getScreenSize();
		int toolBarHeight = this->toolBarSize * screenSize.getHeight();

		auto fullBounds = this->getLocalBounds();
		auto toolBarBounds = fullBounds.withHeight(toolBarHeight);
		auto managerBounds = fullBounds.withTrimmedTop(toolBarHeight);

		if (this->toolBar) {
			this->toolBar->setBounds(toolBarBounds);
		}

		this->manager->setBounds(managerBounds);
	}
}

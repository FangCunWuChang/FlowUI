﻿#include "FlowWindowHub.h"
#include "FlowWindow.h"

namespace flowUI {
	void FlowWindowHub::addWindow(FlowWindow* window) {
		if (!FlowWindowHub::getInstance()->windows.contains(window)) {
			int size = FlowWindowHub::getInstance()->windows.size();
			if (size < INT_MAX - 1) {
				FlowWindowHub::getInstance()->windows.add(window);

				window->setName(FlowWindowHub::getInstance()->title + " - " 
					+ juce::translate("Window") + " " + juce::String(size + 1));
				window->setIcon(FlowWindowHub::getInstance()->iconTemp);
				window->getPeer()->setIcon(FlowWindowHub::getInstance()->iconTemp);
				window->setOpenGL(FlowWindowHub::getInstance()->openGLOn);
				window->setBufferedPainting(FlowWindowHub::getInstance()->shouldBufferedPainting);

				for (auto i : FlowWindowHub::getInstance()->keyListeners) {
					window->addKeyListener(i);
				}
			}
			else {
				delete window;
			}
			FlowWindowHub::setToolBar(
				FlowWindowHub::getInstance()->toolBar,
				FlowWindowHub::getInstance()->toolBarSize);
		}
	}

	void FlowWindowHub::closeWindow(FlowWindow* window) {
		FlowWindowHub::getInstance()->windows.removeObject(window, false);
		int size = FlowWindowHub::getInstance()->windows.size();
		for (int i = 0; i < size; i++) {
			if (auto w = FlowWindowHub::getInstance()->windows[i]) {
				w->setName(FlowWindowHub::getInstance()->title + " - "
					+ juce::translate("Window") + " " + juce::String(i + 1));
			}
		}
		FlowWindowHub::setToolBar(
			FlowWindowHub::getInstance()->toolBar,
			FlowWindowHub::getInstance()->toolBarSize);
	}

	void FlowWindowHub::setToolBar(FlowComponent* toolBar, double size) {
		FlowWindowHub::getInstance()->toolBar = toolBar;
		FlowWindowHub::getInstance()->toolBarSize = size;

		if (FlowWindowHub::getInstance()->getSize() > 0) {
			if (auto window = FlowWindowHub::getInstance()->getWindow(0)) {
				window->setToolBar(toolBar, size);
			}
		}
	}

	void FlowWindowHub::removeToolBar() {
		FlowWindowHub::getInstance()->toolBar = nullptr;
		FlowWindowHub::getInstance()->toolBarSize = 0;

		if (FlowWindowHub::getInstance()->getSize() > 0) {
			if (auto window = FlowWindowHub::getInstance()->getWindow(0)) {
				window->removeToolBar();
			}
		}
	}

	void FlowWindowHub::addKeyListener(juce::KeyListener* listener) {
		if (!listener) { return; }
		if (FlowWindowHub::getInstance()->keyListeners.contains(listener)) { return; }

		FlowWindowHub::getInstance()->keyListeners.add(listener);

		int size = FlowWindowHub::getInstance()->windows.size();
		for (int i = 0; i < size; i++) {
			if (auto w = FlowWindowHub::getInstance()->windows[i]) {
				w->addKeyListener(listener);
			}
		}
	}

	void FlowWindowHub::removeKeyListener(juce::KeyListener* listener) {
		if (!listener) { return; }

		FlowWindowHub::getInstance()->keyListeners.removeAllInstancesOf(listener);

		int size = FlowWindowHub::getInstance()->windows.size();
		for (int i = 0; i < size; i++) {
			if (auto w = FlowWindowHub::getInstance()->windows[i]) {
				w->removeKeyListener(listener);
			}
		}
	}

	int FlowWindowHub::getSize() {
		return FlowWindowHub::getInstance()->windows.size();
	}

	int FlowWindowHub::findWindow(FlowWindow* window) {
		return FlowWindowHub::getInstance()->windows.indexOf(window);
	}

	FlowWindow* FlowWindowHub::getWindow(int index) {
		return FlowWindowHub::getInstance()->windows[index];
	}

	void FlowWindowHub::shutdown() {
		FlowWindowHub::getInstance()->windows.clear();
	}

	void FlowWindowHub::setIcon(const juce::String& iconPath) {
		juce::File iconFile = juce::File::getSpecialLocation(
			juce::File::SpecialLocationType::hostApplicationPath).getParentDirectory().getChildFile(iconPath);
		FlowWindowHub::getInstance()->iconTemp = juce::ImageFileFormat::loadFrom(iconFile);

		for (auto i : FlowWindowHub::getInstance()->windows) {
			i->setIcon(FlowWindowHub::getInstance()->iconTemp);
			i->getPeer()->setIcon(FlowWindowHub::getInstance()->iconTemp);
		}
	}

	void FlowWindowHub::setTitle(const juce::String& title) {
		FlowWindowHub::getInstance()->title = title;

		int size = FlowWindowHub::getInstance()->windows.size();
		for (int i = 0; i < size; i++) {
			if (auto w = FlowWindowHub::getInstance()->windows[i]) {
				w->setName(FlowWindowHub::getInstance()->title + " - "
					+ juce::translate("Window") + " " + juce::String(i + 1));
			}
		}
	}

	void FlowWindowHub::autoLayout(const juce::String& layoutPath, const juce::Array<FlowComponent*>& list) {
		/** Close All Windows */
		FlowWindowHub::getInstance()->windows.clear();
		FlowWindowHub::removeToolBar();

		/** Load Layout File */
		juce::File layoutFile = juce::File::getSpecialLocation(
			juce::File::SpecialLocationType::hostApplicationPath).getParentDirectory().getChildFile(layoutPath);
		auto layoutData = juce::JSON::parse(layoutFile);

		/** Error */
		if (!layoutData.isObject()) { return; }

		/** Windows List */
		auto window = layoutData.getProperty("window", juce::var());
		if (!window.isArray()) { return; }

		auto windowArray = window.getArray();
		for (auto& i : *windowArray) {
			auto grid = i.getProperty("grid", juce::var());
			if (!grid.isObject()) { return; }

			/** Window */
			auto ptrWindow = new FlowWindow();
			ptrWindow->autoLayout(grid, list);
		}

		/** ToolBar */
		auto toolBar = layoutData.getProperty("toolBar", juce::var());
		if (toolBar.isObject()) {
			int id = (int)toolBar.getProperty("id", juce::var{ -1 });
			double size = (double)toolBar.getProperty("size", juce::var{ 0.0 });
			if (id >= 0 && id < list.size()) {
				FlowWindowHub::setToolBar(
					list.getUnchecked(id), size);
			}
		}
	}

	void FlowWindowHub::saveLayout(const juce::String& layoutPath, const juce::Array<FlowComponent*>& list) {
		auto layout = new juce::DynamicObject{};

		/** ToolBar */
		if (FlowWindowHub::getInstance()->toolBar) {
			auto toolBar = new juce::DynamicObject{};

			toolBar->setProperty("id", list.indexOf(FlowWindowHub::getInstance()->toolBar));
			toolBar->setProperty("size", FlowWindowHub::getInstance()->toolBarSize);

			layout->setProperty("toolBar", juce::var{ toolBar });
		}

		/** Windows List */
		juce::Array<juce::var> windowList;
		for (auto i : FlowWindowHub::getInstance()->windows) {
			auto window = new juce::DynamicObject{};

			window->setProperty("grid", i->getLayout(list));

			windowList.add(juce::var{ window });
		}
		layout->setProperty("window", windowList);

		juce::var layoutVar(layout);

		juce::File layoutFile = juce::File::getSpecialLocation(
			juce::File::SpecialLocationType::hostApplicationPath).getParentDirectory().getChildFile(layoutPath);
		juce::FileOutputStream layoutStream(layoutFile);
		if (!layoutStream.openedOk()) { return; }

		layoutStream.setPosition(0);
		layoutStream.truncate();

		juce::JSON::writeToStream(layoutStream, layoutVar);
	}

	void FlowWindowHub::setOpenGL(bool openGLOn) {
		FlowWindowHub::getInstance()->openGLOn = openGLOn;
		for (auto i : FlowWindowHub::getInstance()->windows) {
			i->setOpenGL(openGLOn);
		}
	}

	void FlowWindowHub::setBufferedPainting(bool shouldBuffered) {
		FlowWindowHub::getInstance()->shouldBufferedPainting = shouldBuffered;
		for (auto i : FlowWindowHub::getInstance()->windows) {
			i->setBufferedPainting(shouldBuffered);
		}
	}

	void FlowWindowHub::setAppExitHook(const FlowWindowHub::AppExitHook& hook) {
		FlowWindowHub::getInstance()->appExitHook = hook;
	}

	const FlowWindowHub::AppExitHook& FlowWindowHub::getAppExitHook() {
		return FlowWindowHub::getInstance()->appExitHook;
	}

	FlowWindowHub* FlowWindowHub::getInstance() {
		return FlowWindowHub::instance;
	}

	FlowWindowHub* FlowWindowHub::instance = new FlowWindowHub();
}

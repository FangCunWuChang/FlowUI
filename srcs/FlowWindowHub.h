#pragma once

#include <JuceHeader.h>

namespace flowUI {
	class FlowWindow;
	class FlowComponent;

	class FlowWindowHub final : private juce::DeletedAtShutdown {
	public:
		FlowWindowHub() = default;

		static void addWindow(FlowWindow* window);
		static void closeWindow(FlowWindow* window);

		static void setToolBar(FlowComponent* toolBar, double size);
		static void removeToolBar();

		static void addKeyListener(juce::KeyListener* listener);
		static void removeKeyListener(juce::KeyListener* listener);

		static int getSize();
		static int findWindow(FlowWindow* window);
		static FlowWindow* getWindow(int index);

		static void shutdown();

		static void setIcon(const juce::String& iconPath);
		static void setTitle(const juce::String& title);

		static void autoLayout(const juce::String& layoutPath, const juce::Array<FlowComponent*>& list);
		static void saveLayout(const juce::String& layoutPath, const juce::Array<FlowComponent*>& list);

		static void setOpenGL(bool openGLOn);
		static void setBufferedPainting(bool shouldBuffered);

		using AppExitHook = std::function<bool(void)>;
		static void setAppExitHook(const AppExitHook& hook);
		static const AppExitHook& getAppExitHook();

	private:
		juce::OwnedArray<FlowWindow> windows;
		FlowComponent* toolBar = nullptr;
		juce::Array<juce::KeyListener*> keyListeners;
		double toolBarSize = 0;
		juce::Image iconTemp;
		juce::String title;
		bool openGLOn = false;
		bool shouldBufferedPainting = false;
		AppExitHook appExitHook = [] { return true; };

	private:
		static FlowWindowHub* getInstance();
		static FlowWindowHub* instance;
	};
}

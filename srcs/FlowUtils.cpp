#include "FlowUtils.h"

namespace flowUI {

juce::File resolveRuntimeAssetFile(const juce::String& path) {
#if JUCE_MAC
	return juce::File::getSpecialLocation(juce::File::hostApplicationPath)
		.getParentDirectory().getParentDirectory().getParentDirectory()
		.getChildFile("./Contents/Resources/")
		.getChildFile(path);

#else //MAC_OS
	return juce::File::getSpecialLocation(juce::File::hostApplicationPath)
		.getSiblingFile(path);

#endif//MAC_OS
}

}// namespace flowUI
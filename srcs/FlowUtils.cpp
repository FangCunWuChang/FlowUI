#include "FlowUtils.h"

namespace flowUI {

juce::File resolveRuntimeAssetFile(const juce::String& path) {
	auto executableDir = juce::File::getSpecialLocation(juce::File::hostApplicationPath)
		.getParentDirectory();

#if JUCE_MAC
	auto baseDir = executableDir
		.getParentDirectory().getParentDirectory()
		.getChildFile("./Contents/Resources/");
		
	if(baseDir.isDirectory()){
		return baseDir.getChildFile(path);
	}

#endif//MAC_OS

	return executableDir.getChildFile(path);
}

}// namespace flowUI
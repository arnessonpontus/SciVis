#include <modules/tnm067common/processors/test2by2image.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo Test2by2Image::processorInfo_{
    "org.inviwo.Test2by2Image",  // Class identifier
    "Test2by2Image",             // Display name
    "TNM067",                    // Category
    CodeState::Stable,           // Code state
    Tags::None,                  // Tags
};
const ProcessorInfo Test2by2Image::getProcessorInfo() const { return processorInfo_; }

Test2by2Image::Test2by2Image() : Processor(), outport_("outport", false) { addPort(outport_); }

void Test2by2Image::process() {
    auto img = std::make_shared<Image>(size2_t(2), DataVec4UInt8::get());
    auto pixels = static_cast<LayerRAMPrecision<glm::u8vec4>*>(
                      img->getColorLayer()->getEditableRepresentation<LayerRAM>())
                      ->getDataTyped();
    pixels[0] = glm::u8vec4(0, 0, 0, 255);
    pixels[1] = glm::u8vec4(255, 0, 0, 255);
    pixels[2] = glm::u8vec4(0, 255, 0, 255);
    pixels[3] = glm::u8vec4(0, 0, 255, 255);
    outport_.setData(img);
}

}  // namespace inviwo

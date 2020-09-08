#include <modules/tnm067common/processors/gauss2dfunction.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/util/indexmapper.h>

namespace inviwo {

const ProcessorInfo Gauss2DFunction::processorInfo_{
    "org.inviwo.gauss2dfunction",  // Class identifier
    "Gauss2DFunction",             // Display name
    "TNM067",                      // Category
    CodeState::Stable,             // Code state
    Tags::None,                    // Tags
};
const ProcessorInfo Gauss2DFunction::getProcessorInfo() const { return processorInfo_; }

Gauss2DFunction::Gauss2DFunction()
    : Processor()
    , imageOutport_("outport", DataFloat32::get(), false)
    , size_("size", "Size", 32, 1, 256)
    , gauss2d_("gauss2d", "Gaussian") {

    addPort(imageOutport_);
    gauss2d_.height_ = 1.0f;
    gauss2d_.sigma_ = 0.2f;
    gauss2d_.center_ = dvec2(0.5f, 0.5f);
    addProperty(size_);
    addProperty(gauss2d_);
}

void Gauss2DFunction::process() {
    std::unique_ptr<Image> upScaleImage =
        util::make_unique<Image>(size2_t(size_.get()), DataFloat32::get());
    upScaleImage->getColorLayer()->setSwizzleMask(swizzlemasks::luminance);
    gaussImage(*static_cast<LayerRAMPrecision<float>*>(
        upScaleImage->getColorLayer()->getEditableRepresentation<LayerRAM>()));
    imageOutport_.setData(upScaleImage.release());
}

void Gauss2DFunction::gaussImage(LayerRAMPrecision<float>& img) {
    const auto dim = img.getDimensions();
    const auto data = img.getDataTyped();
    util::IndexMapper2D index(dim);

    for (size_t i = 0; i < dim.x; i++) {
        for (size_t j = 0; j < dim.y; j++) {

            const auto x = static_cast<double>(i) / (dim.x - 1);
            const auto y = static_cast<double>(j) / (dim.y - 1);

            // height from gauss field
            const auto height = gauss2d_.evaluate(dvec2(x, y));
            data[index(i, j)] = static_cast<float>(height);
        }
    }
}

}  // namespace inviwo

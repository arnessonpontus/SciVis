#pragma once

#include <modules/tnm067common/tnm067commonmoduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/base/properties/gaussianproperty.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>

namespace inviwo {

class IVW_MODULE_TNM067COMMON_API Gauss2DFunction : public Processor { 
public:
    Gauss2DFunction();
    virtual ~Gauss2DFunction() = default;
     
    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;
private:
    void gaussImage(LayerRAMPrecision<float>& img);

    ImageOutport imageOutport_;
    IntSizeTProperty size_;
    Gaussian2DProperty gauss2d_;
};

} // namespace


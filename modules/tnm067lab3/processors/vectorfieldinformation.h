#pragma once

#include <modules/tnm067lab3/tnm067lab3moduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/shader/shader.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/stringproperty.h>

namespace inviwo {

class IVW_MODULE_TNM067LAB3_API VectorFieldInformation : public Processor {
public:
    static const ProcessorInfo processorInfo_;
    virtual const ProcessorInfo getProcessorInfo() const override;

    enum class Information { PassThoruh, Magnitude, Divergence, Rotation };

    VectorFieldInformation();
    virtual ~VectorFieldInformation() = default;

    virtual void initializeResources() override;
    virtual void process() override;

private:
    ImageInport vf_;
    ImageOutport outport_;

    TemplateOptionProperty<Information> outputType_;
    StringProperty outputTypeStr_;

    Shader shader_;
};

}  // namespace inviwo

#pragma once

#include <modules/tnm067lab3/tnm067lab3moduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/shader/shader.h>

namespace inviwo {

class IVW_MODULE_TNM067LAB3_API LineIntegralConvolution : public Processor {
public:
    LineIntegralConvolution();
    virtual ~LineIntegralConvolution() = default;

    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

private:
    ImageInport vf_;
    ImageInport noise_;
    ImageOutport outport_;

    IntProperty steps_;
    FloatProperty stepSize_;

    Shader shader_;
};

}  // namespace inviwo

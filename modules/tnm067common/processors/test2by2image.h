#pragma once

#include <modules/tnm067common/tnm067commonmoduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

class IVW_MODULE_TNM067COMMON_API Test2by2Image : public Processor {
public:
    Test2by2Image();
    virtual ~Test2by2Image() = default;

    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

private:
    ImageOutport outport_;
};

}  // namespace inviwo

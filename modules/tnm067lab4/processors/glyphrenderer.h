#pragma once

#include <modules/tnm067lab4/tnm067lab4moduledefine.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/eventproperty.h>
#include <modules/opengl/shader/shader.h>
#include <inviwo/core/datastructures/geometry/mesh.h>

#include <optional>

namespace inviwo {

class IVW_MODULE_TNM067LAB4_API GlyphRenderer : public Processor {
public:
    GlyphRenderer();
    virtual ~GlyphRenderer();

    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

private:
    static Mesh buildMesh(const Image& vectorField, std::optional<size_t> grid,
                          std::optional<vec2> mousePos);

    ImageInport background_;
    ImageInport vf_;
    ImageOutport outport_;

    FloatProperty glyphScale_;
    IntSizeTProperty gridSize_;
    BoolProperty includeGrid_;
    BoolProperty includeMousePos_;

    EventProperty mouseMoveEvent_;

    Shader shader_;
    std::optional<Mesh> mesh_;

    vec2 lastMousePos_;
};

}  // namespace inviwo

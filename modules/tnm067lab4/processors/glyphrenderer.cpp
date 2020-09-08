#include <modules/tnm067lab4/processors/glyphrenderer.h>

#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/shader/shaderutils.h>

#include <inviwo/core/util/imagesampler.h>
#include <modules/tnm067lab4/jacobian.h>

#include <modules/opengl/rendering/meshdrawergl.h>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

const ProcessorInfo GlyphRenderer::processorInfo_{
    "org.inviwo.TNM067GlyphRenderer",  // Class identifier
    "Glyph Renderer",                  // Display name
    "TNM067",                          // Category
    CodeState::Stable,                 // Code state
    Tags::GL,                          // Tags
};

const ProcessorInfo GlyphRenderer::getProcessorInfo() const { return processorInfo_; }

GlyphRenderer::GlyphRenderer()
    : Processor()
    , background_("background")
    , vf_("vf")
    , outport_("outport")
    , glyphScale_("glyphScale", "Glyph Scale", 1, 0, 10)
    , gridSize_("gridSize", "Grid Size (N x N)", 5, 3, 30)
    , includeGrid_("includeGrid", "Ellipses On Grid", true)
    , includeMousePos_("includeMousePos_", "Ellipse under Mouse Position", false)
    , mouseMoveEvent_(
          "mouseMoveEvent", "Mouse Move",
          [&](Event* e) {
              if (auto me = dynamic_cast<MouseEvent*>(e)) {
                  lastMousePos_ = me->posNormalized();
                  mesh_ = std::nullopt;
                  invalidate(InvalidationLevel::InvalidOutput);
              }
          },
          MouseButton::None, MouseState::Move)

    , shader_("tensor_glyphrenderer.vert", "tensor_glyphrenderer.geom", "tensor_glyphrenderer.frag")
    , mesh_{}

    , lastMousePos_(0.5f) {
    addPort(background_);
    addPort(vf_);
    addPort(outport_);

    addProperty(glyphScale_);
    addProperty(gridSize_);
    addProperty(includeGrid_);
    addProperty(includeMousePos_);

    addProperty(mouseMoveEvent_);

    includeGrid_.onChange([&]() { mesh_ = std::nullopt; });
    includeMousePos_.onChange([&]() { mesh_ = std::nullopt; });
    vf_.onChange([&]() { mesh_ = std::nullopt; });
    gridSize_.onChange([&]() { mesh_ = std::nullopt; });

    shader_.onReload([this]() { invalidate(InvalidationLevel::InvalidResources); });
}

GlyphRenderer::~GlyphRenderer() = default;

void GlyphRenderer::process() {
    if (!mesh_) {
        mesh_ = buildMesh(
            *vf_.getData(),
            includeGrid_ ? std::optional<size_t>{gridSize_} : std::optional<size_t>{},
            includeMousePos_ ? std::optional<vec2>{lastMousePos_} : std::optional<vec2>{});
    }

    utilgl::activateTargetAndCopySource(outport_, background_, ImageType::ColorOnly);

    shader_.activate();
    shader_.setUniform("radius", glyphScale_.get() * 0.005f);

    MeshDrawerGL::DrawObject drawer(mesh_->getRepresentation<MeshGL>(),
                                    mesh_->getDefaultMeshInfo());

    drawer.draw();
    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

Mesh GlyphRenderer::buildMesh(const Image& vectorField, std::optional<size_t> grid,
                              std::optional<vec2> mousePos) {

    std::vector<vec2> positions;

    if (grid) {
        auto s = *grid;
        float ds = 1.0f / s;
        for (size_t i = 0; i < s; i++) {
            float x = ds * (i + 0.5f);
            for (size_t j = 0; j < s; j++) {
                float y = ds * (j + 0.5f);
                positions.emplace_back(x, y);
            }
        }
    }

    if (mousePos) {
        positions.push_back(*mousePos);
    }

    ImageSampler sampler(&vectorField);

    const vec2 offset = 1.0f / vec2(vectorField.getDimensions() - size2_t(1));

    Mesh mesh;
    auto verticesBuf = std::make_shared<Buffer<vec3>>();
    auto jacobiansBuf = std::make_shared<Buffer<vec4>>();
    auto indicesBuf = std::make_shared<IndexBuffer>(std::make_shared<IndexBufferRAM>());

    auto& vertices = verticesBuf->getEditableRAMRepresentation()->getDataContainer();
    auto& jacobians = jacobiansBuf->getEditableRAMRepresentation()->getDataContainer();
    auto& indices = indicesBuf->getEditableRAMRepresentation()->getDataContainer();

    mesh.addBuffer(BufferType::PositionAttrib, verticesBuf);
    mesh.addBuffer(BufferType::ColorAttrib, jacobiansBuf);
    mesh.addIndices(Mesh::MeshInfo(DrawType::Points, ConnectivityType::None), indicesBuf);

    vertices.reserve(positions.size());
    jacobians.reserve(positions.size());
    indices.reserve(positions.size());

    for (const auto& p : positions) {
        vertices.emplace_back(p, 0);
        auto J = util::jacobian(sampler, p, offset);
        jacobians.emplace_back(J[0], J[1]);
        indices.push_back(static_cast<std::uint32_t>(indices.size()));
    }
    return mesh;
}

}  // namespace inviwo

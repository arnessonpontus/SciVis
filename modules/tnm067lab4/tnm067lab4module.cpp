#include <modules/tnm067lab4/tnm067lab4module.h>
#include <modules/tnm067lab4/processors/glyphrenderer.h>
#include <modules/opengl/shader/shadermanager.h>

namespace inviwo {

TNM067Lab4Module::TNM067Lab4Module(InviwoApplication* app) : InviwoModule(app, "TNM067Lab4") {
    // Add a directory to the search path of the Shadermanager
    ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:
    // Processors
    registerProcessor<GlyphRenderer>();
}

}  // namespace inviwo

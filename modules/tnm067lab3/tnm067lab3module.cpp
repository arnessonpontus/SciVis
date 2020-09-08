#include <modules/tnm067lab3/tnm067lab3module.h>
#include <modules/tnm067lab3/processors/lineintegralconvolution.h>
#include <modules/tnm067lab3/processors/vectorfieldinformation.h>
#include <modules/opengl/shader/shadermanager.h>

namespace inviwo {

TNM067Lab3Module::TNM067Lab3Module(InviwoApplication* app) : InviwoModule(app, "TNM067Lab3") {
    // Add a directory to the search path of the Shadermanager
    ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:
    // Processors
    registerProcessor<LineIntegralConvolution>();
    registerProcessor<VectorFieldInformation>();

}

}  // namespace inviwo

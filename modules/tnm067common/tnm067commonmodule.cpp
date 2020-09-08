#include <modules/tnm067common/tnm067commonmodule.h>
#include <modules/tnm067common/processors/gauss2dfunction.h>
#include <modules/tnm067common/processors/test2by2image.h>

namespace inviwo {

TNM067CommonModule::TNM067CommonModule(InviwoApplication* app) : InviwoModule(app, "TNM067Common") {
    // Add a directory to the search path of the Shadermanager
    // ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:
    // Processors
    registerProcessor<Gauss2DFunction>();
    registerProcessor<Test2by2Image>();
}

}  // namespace inviwo

#pragma once

#include <modules/tnm067lab4/tnm067lab4moduledefine.h>
#include <inviwo/core/util/imagesampler.h>

namespace inviwo {
class LayerRAM;

namespace util {

IVW_MODULE_TNM067LAB4_API mat2 jacobian(const ImageSampler& sampler, vec2 position, vec2 offset);

}

}  // namespace inviwo

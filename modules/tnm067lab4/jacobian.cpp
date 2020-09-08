#include <modules/tnm067lab4/jacobian.h>

namespace inviwo {

namespace util {

mat2 jacobian(const ImageSampler& sampler, vec2 position, vec2 offset) {
    vec2 ox(offset.x, 0);
    vec2 oy(0, offset.y);
    mat2 J;  // TODO: Calculate the Jacobian J
    return J;
}

}  // namespace util

}  // namespace inviwo

#include <modules/tnm067lab4/jacobian.h>

namespace inviwo
{

    namespace util
    {

        vec2 partialDerivateX(vec2 coord, vec2 offset, const ImageSampler &sampler)
        {
            float dx = offset.x;
            vec2 plusx = sampler.sample(coord + vec2(dx, 0));
            vec2 minusx = sampler.sample(coord + vec2(-dx, 0));

            vec2 partial_derivate = (plusx - minusx) / (2.0 * dx);
            return partial_derivate;
        }

        vec2 partialDerivateY(vec2 coord, vec2 offset, const ImageSampler &sampler)
        {
            float dy = offset.y;
            vec2 plusy = sampler.sample(coord + vec2(0, dy));
            vec2 minusy = sampler.sample(coord + vec2(0, -dy));

            vec2 partial_derivate = (plusy - minusy) / (2.0 * dy);
            return partial_derivate;
        }

        mat2 jacobian(const ImageSampler &sampler, vec2 position, vec2 offset)
        {
            vec2 pdx = partialDerivateX(position, offset, sampler);
            vec2 pdy = partialDerivateY(position, offset, sampler);

            mat2 J; // = {pdx.x, pdx.y, pdy.x, pdy.y}; // TODO: Calculate the Jacobian J
            J[0][0] = pdx.x;
            J[0][1] = pdx.y;
            J[1][0] = pdy.x;
            J[1][1] = pdy.y;

            return J;
        }

    } // namespace util

} // namespace inviwo

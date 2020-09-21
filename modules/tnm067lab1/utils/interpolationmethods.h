#pragma once

#include <modules/tnm067lab1/tnm067lab1moduledefine.h>
#include <inviwo/core/util/glm.h>

namespace inviwo {

template <typename T>
struct float_type {
    using type = double;
};

template <>
struct float_type<float> {
    using type = float;
};
template <>
struct float_type<vec3> {
    using type = float;
};
template <>
struct float_type<vec2> {
    using type = float;
};
template <>
struct float_type<vec4> {
    using type = float;
};

namespace TNM067 {
namespace Interpolation {

#define ENABLE_LINEAR_UNITTEST 1
template <typename T, typename F = double>
T linear(const T& a, const T& b, F x) {
    if (x <= 0) return a;
    if (x >= 1) return b;

    //F min_val = std::min(a, b);
    //F max_val = std::max(a, b);
    return (1 - x) * a + x * b;
}

// clang-format off
    /*
     2------3
     |      |
    y|  �   |
     |      |
     0------1
        x
    */
    // clang format on
#define ENABLE_BILINEAR_UNITTEST 1
template<typename T, typename F = double> 
T bilinear(const std::array<T, 4> &v, F x, F y) {
    return (linear(linear(v[0], v[1], x), linear(v[2], v[3], x), y));
}

    // clang-format off
    /* 
    a------�b------c
    0  x    1      2
    */
// clang-format on
#define ENABLE_QUADRATIC_UNITTEST 1
template <typename T, typename F = double>
T quadratic(const T& a, const T& b, const T& c, F x) {
    if (x <= 0) return a;
    return ((1-x)*(1-2*x)*a+4*x*(1-x)*b+x*(2*x-1)*c); //Unsiged int fails
}

// clang-format off
    /* 
    6-------7-------8
    |       |       |
    |       |       |
    |       |       |
    3-------4-------5
    |       |       |
   y|  �    |       |
    |       |       |
    0-------1-------2
    0  x    1       2
    */
// clang-format on
#define ENABLE_BIQUADRATIC_UNITTEST 0
template <typename T, typename F = double>
T biQuadratic(const std::array<T, 9>& v, F x, F y) {
    T x012 = quadratic(v[0], v[1], v[2], x);
    T x345 = quadratic(v[3], v[4], v[5], x);
    T x678 = quadratic(v[6], v[7], v[8], x);

    return quadratic(x012, x345, x678, y);
}

// clang-format off
    /*
     2---------3
     |'-.      |
     |   -,    |
   y |  �  -,  |
     |       -,|
     0---------1
        x
    */
// clang-format on
#define ENABLE_BARYCENTRIC_UNITTEST 0
template <typename T, typename F = double>
T barycentric(const std::array<T, 4>& v, F x, F y) {
    return v[0];
}

}  // namespace Interpolation
}  // namespace TNM067
}  // namespace inviwo

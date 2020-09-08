#include <warn/push>
#include <warn/ignore/all>
#include <gtest/gtest.h>
#include <warn/pop>

#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/util/imagesampler.h>
#include <inviwo/core/datastructures/image/image.h>

#include <modules/tnm067lab4/jacobian.h>

#include <memory>
#include <tuple>

namespace inviwo {

constexpr double EPS{3.0e-07};

void test_vec2(vec2 e, vec2 r) {
    ASSERT_NEAR(e.x, r.x, EPS);
    ASSERT_NEAR(e.y, r.y, EPS);
}

void test_mat2(mat2 e, mat2 r) {
    test_vec2(e[0], r[0]);
    test_vec2(e[1], r[1]);
}

template <typename C>
std::shared_ptr<Image> createTestImage(C callback) {
    auto img = std::make_shared<Image>(size2_t(10, 10), DataVec2Float32::get());
    auto data =
        static_cast<vec2*>(img->getColorLayer()->getEditableRepresentation<LayerRAM>()->getData());

    const size_t size = 10;
    size_t idx = 0;
    for (int j = 0; j < size; j++)
        for (int i = 0; i < size; i++) {
            vec2 p(i, j);
            p /= size - 1;
            p -= 0.5;
            data[idx++] = callback(p);
        }
    return img;
}

std::vector<std::pair<std::function<vec2(const vec2&)>, mat2>> testValues = {
    {[](const vec2& p) { return p; }, mat2(1, 0, 0, 1)},
    {[](const vec2& p) { return vec2(p.y, p.x); }, mat2(0, 1, 1, 0)},
    {[](const vec2& p) { return vec2(-p.y, p.x); }, mat2(0, 1, -1, 0)},
    {[](const vec2& p) { return vec2(p.x + p.y, p.x + p.y); }, mat2(1, 1, 1, 1)}};

const std::vector<vec2> points = {{0.25f, 0.25f}, {0.5f, 0.25f}, {0.75f, 0.25f},
                                  {0.25f, 0.5f},  {0.5f, 0.5f},  {0.75f, 0.5f},
                                  {0.25f, 0.75f}, {0.5f, 0.57f}, {0.75f, 0.75f}};

TEST(JacobianTest, FullTest) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            test_mat2(e, r);
        }
    }
}

TEST(JacobianTest, IgnoringScaling) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            r[0] = glm::normalize(r[0]);
            r[1] = glm::normalize(r[1]);
            auto e = testValue.second;
            e[0] = glm::normalize(e[0]);
            e[1] = glm::normalize(e[1]);
            test_mat2(e, r);
        }
    }
}

TEST(JacobianTest, OnlyDiagonal) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            ASSERT_NEAR(e[0][0], r[0][0], EPS);
            ASSERT_NEAR(e[1][1], r[1][1], EPS);
        }
    }
}

TEST(JacobianTest, OnlyNotDiagonal) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            ASSERT_NEAR(e[0][1], r[0][1], EPS);
            ASSERT_NEAR(e[1][0], r[1][0], EPS);
        }
    }
}

TEST(JacobianTest, OnlyCol0Row0) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            ASSERT_NEAR(e[0][0], r[0][0], EPS);
        }
    }
}

TEST(JacobianTest, OnlyCol0Row1) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            ASSERT_NEAR(e[0][1], r[0][1], EPS);
        }
    }
}

TEST(JacobianTest, OnlyCol1Row0) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            ASSERT_NEAR(e[1][0], r[1][0], EPS);
        }
    }
}

TEST(JacobianTest, OnlyCol1Row1) {
    for (auto& testValue : testValues) {
        auto img = createTestImage(testValue.first);
        ImageSampler s(img);
        for (const auto& p : points) {
            auto r = util::jacobian(s, p, vec2(0.1, 0.1));
            auto e = testValue.second;
            ASSERT_NEAR(e[1][1], r[1][1], EPS);
        }
    }
}

}  // namespace inviwo
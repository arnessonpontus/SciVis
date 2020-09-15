#include <modules/tnm067lab1/utils/scalartocolormapping.h>

namespace inviwo {

void ScalarToColorMapping::clearColors() { baseColors_.clear(); }
void ScalarToColorMapping::addBaseColors(vec4 color) { baseColors_.push_back(color); }

vec4 ScalarToColorMapping::sample(float t) const {
    if (baseColors_.size() == 0) return vec4(t);
    if (baseColors_.size() == 1) return vec4(baseColors_[0]);

    // Implement here:
    // Interpolate colors in baseColors_
    // return the right values

    float smallest_dist1 = __INT_MAX__;
    float smallest_dist2 = __INT_MAX__;
    int smallest_idx1 = 0;
    int smallest_idx2 = 0;

    vec4 finalColor(t, t, t, 1);  // dummy color

    for (size_t i = 0; i < baseColors_.size(); ++i) {
        if(glm::distance(finalColor, baseColors_[i]) < smallest_dist1) {
            smallest_dist1 = glm::distance(finalColor, baseColors_[i]);
            smallest_idx1 = i;
        } else if(glm::distance(finalColor, baseColors_[i]) < smallest_dist2) {
            smallest_dist2 = glm::distance(finalColor, baseColors_[i]);
            smallest_idx2 = i;
        }
    }

    return ((1 - t) * baseColors_[smallest_idx1] + t * baseColors_[smallest_idx2]);

    if (t <= 0) return vec4(baseColors_.front());
    if (t >= 1) return vec4(baseColors_.back());

    return finalColor;
}

}  // namespace inviwo

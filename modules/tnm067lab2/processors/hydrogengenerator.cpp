#include <modules/tnm067lab2/processors/hydrogengenerator.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/util/volumeramutils.h>
#include <modules/base/algorithm/dataminmax.h>
#include <inviwo/core/util/indexmapper.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <modules/base/algorithm/dataminmax.h>

namespace inviwo {

const ProcessorInfo HydrogenGenerator::processorInfo_{
    "org.inviwo.HydrogenGenerator",  // Class identifier
    "Hydrogen Generator",            // Display name
    "TNM067",                        // Category
    CodeState::Stable,               // Code state
    Tags::CPU,                       // Tags
};

const ProcessorInfo HydrogenGenerator::getProcessorInfo() const { return processorInfo_; }

HydrogenGenerator::HydrogenGenerator()
    : Processor(), volume_("volume"), size_("size_", "Volume Size", 16, 4, 256) {
    addPort(volume_);
    addProperty(size_);
}

void HydrogenGenerator::process() {
    auto vol = std::make_shared<Volume>(size3_t(size_), DataFloat32::get());

    auto ram = vol->getEditableRepresentation<VolumeRAM>();
    auto data = static_cast<float*>(ram->getData());
    util::IndexMapper3D index(ram->getDimensions());

    util::forEachVoxel(*ram, [&](const size3_t& pos) {
        vec3 cartesian = idTOCartesian(pos);
        data[index(pos)] = static_cast<float>(eval(cartesian));
    });

    auto minMax = util::volumeMinMax(ram);
    vol->dataMap_.dataRange = vol->dataMap_.valueRange = dvec2(minMax.first.x, minMax.second.x);

    volume_.setData(vol);
}

vec3 HydrogenGenerator::cartesianToSphereical(vec3 cartesian) {
    double r = sqrt((cartesian.x * cartesian.x) + (cartesian.y * cartesian.y) + (cartesian.z * cartesian.z));
    double theta;
    double psi;

    if(r == 0) {
        theta = 0;
        psi = 0;
        return vec3{r, theta, psi};
    }

    double tmp = cartesian.z / r;
    if(tmp > 1 || tmp < -1 || tmp == 0) {
        theta = M_PI_2;
    } else {
        theta = acos(tmp);
    }

    psi = atan2(cartesian.y, cartesian.x);
    return vec3{r, theta, psi};
}

double HydrogenGenerator::eval(vec3 cartesian) {
    vec3 sph = cartesianToSphereical(cartesian);
    
    double r = sph.r;
    double theta = sph.g;
    double psi = sph.b;
    
    double Z = 1.0;
    double a0 = 1.0;
    
    double yellow = 1.0 / (81 * sqrt(6 * M_PI));
    double red = pow((Z / a0), (3.0 / 2.0));
    double blue = ((Z * Z) * (r * r)) / (a0 * a0); 
    double green = exp((-Z * r) / (3.0 * a0));
    double pink = (3 * pow(cos(theta), 2)) - 1;
    double density = yellow * red * blue * green * pink;

    return pow(density, 2);
}

vec3 HydrogenGenerator::idTOCartesian(size3_t pos) {
    vec3 p(pos);
    p /= size_ - 1;
    return p * (36.0f) - 18.0f;
}

}  // namespace inviwo

#include <modules/tnm067lab2/processors/marchingtetrahedra.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/util/indexmapper.h>
#include <inviwo/core/util/assertion.h>
#include <inviwo/core/network/networklock.h>

namespace inviwo {

size_t MarchingTetrahedra::HashFunc::max = 1;

const ProcessorInfo MarchingTetrahedra::processorInfo_{
    "org.inviwo.MarchingTetrahedra",  // Class identifier
    "Marching Tetrahedra",            // Display name
    "TNM067",                         // Category
    CodeState::Stable,                // Code state
    Tags::None,                       // Tags
};
const ProcessorInfo MarchingTetrahedra::getProcessorInfo() const { return processorInfo_; }

MarchingTetrahedra::MarchingTetrahedra()
    : Processor()
    , volume_("volume")
    , mesh_("mesh")
    , isoValue_("isoValue", "ISO value", 0.5f, 0.0f, 1.0f) {

    addPort(volume_);
    addPort(mesh_);

    addProperty(isoValue_);

    isoValue_.setSerializationMode(PropertySerializationMode::All);

    volume_.onChange([&]() {
        if (!volume_.hasData()) {
            return;
        }
        NetworkLock lock(getNetwork());
        float iso = (isoValue_.get() - isoValue_.getMinValue()) /
                    (isoValue_.getMaxValue() - isoValue_.getMinValue());
        const auto vr = volume_.getData()->dataMap_.valueRange;
        isoValue_.setMinValue(static_cast<float>(vr.x));
        isoValue_.setMaxValue(static_cast<float>(vr.y));
        isoValue_.setIncrement(static_cast<float>(glm::abs(vr.y - vr.x) / 50.0));
        isoValue_.set(static_cast<float>(iso * (vr.y - vr.x) + vr.x));
        isoValue_.setCurrentStateAsDefault();
    });
}

// Our own func
void addTriangles(MarchingTetrahedra::MeshHelper& mesh, double iso, std::vector<MarchingTetrahedra::Voxel> voxels, std::vector<unsigned long> vert_idx, bool single) {
    double t = 0;
    double iso_diff1 = 0;
    double iso_diff2 = 0;
    double voxel_diff1 = 0;
    double voxel_diff2 = 0;
    vec3 loc{};
    size_t indices[4];
    
    if(single) {
        iso_diff1 = iso - voxels.back().value;
        
        for(size_t i = 0; i < voxels.size() - 1; ++i) {
            voxel_diff1 = voxels[i].value - voxels.back().value;
            t = glm::clamp(iso_diff1 / voxel_diff1, 0.0, 1.0);
            loc = voxels.back().pos + (voxels[i].pos - voxels.back().pos) * t; 
            indices[i] = mesh.addVertex(loc, vert_idx[voxels.size() - 1], vert_idx[i]);
        }
        

        mesh.addTriangle(indices[0], indices[1], indices[2]);
    } else {
        iso_diff1 = iso - voxels.back().value;
        iso_diff2 = iso - voxels[2].value;
        
        for(size_t i = 0; i < voxels.size() - 2; ++i) {
            voxel_diff1 = voxels[i].value - voxels.back().value;
            voxel_diff2 = voxels[i].value - voxels[2].value;
            
            t = iso_diff1 / voxel_diff1;
            loc = voxels.back().pos + (voxels[i].pos - voxels.back().pos) * t; 
            indices[i] = mesh.addVertex(loc, vert_idx[voxels.size() - 1], vert_idx[i]);
            
            t = iso_diff2 / voxel_diff2;
            loc = voxels[2].pos + (voxels[i].pos - voxels[2].pos) * t; 
            indices[i+2] = mesh.addVertex(loc, vert_idx[voxels.size() - 2], vert_idx[i]);
        }

        mesh.addTriangle(indices[1], indices[0], indices[3]); // Check order
        mesh.addTriangle(indices[1], indices[3], indices[2]);
    }

    voxels.clear();
    vert_idx.clear();
}

void MarchingTetrahedra::process() {
    auto volume = volume_.getData()->getRepresentation<VolumeRAM>();
    MeshHelper mesh(volume_.getData());

    const auto& dims = volume->getDimensions();
    MarchingTetrahedra::HashFunc::max = dims.x * dims.y * dims.z;

    const float iso = isoValue_.get();

    util::IndexMapper3D index(dims);

    const static size_t tetrahedraIds[6][4] = {{0, 1, 2, 5}, {1, 3, 2, 5}, {3, 2, 5, 7},
                                               {0, 2, 4, 5}, {6, 4, 2, 5}, {6, 7, 5, 2}};

    size3_t pos{};
    for (pos.z = 0; pos.z < dims.z - 1; ++pos.z) {
        for (pos.y = 0; pos.y < dims.y - 1; ++pos.y) {
            for (pos.x = 0; pos.x < dims.x - 1; ++pos.x) {
                // Step 1: create current cell
                // Use volume->getAsDouble to query values from the volume
                // Spatial position should be between 0 and 1
                // The voxel index should be the 1D-index for the voxel
                
                // Cell c;
                Voxel vxl;
                Cell c;
				size_t idx = 0;
				for (size_t z = 0; z < 2; ++z) {
					for (size_t y = 0; y < 2; ++y) {
						for (size_t x = 0; x < 2; ++x) {
							vec3 global_pos(pos.x + x, pos.y + y, pos.z + z);
							vxl.pos = global_pos; 
							vxl.index = index(global_pos); 
							vxl.value = volume->getAsDouble(global_pos);
							c.voxels[idx] = vxl;
							idx++;
						}
					}
				}
                // Task 6
                // Step 2: Subdivide cell into tetrahedra (hint: use tetrahedraIds)

                std::vector<Tetrahedra> tetrahedras;
                Tetrahedra t;
                for(int i = 0; i < 6; ++i) {
                    for(int j = 0; j < 4; ++j) {
                        t.voxels[j].pos = c.voxels[tetrahedraIds[i][j]].pos;
                        t.voxels[j].value = c.voxels[tetrahedraIds[i][j]].value;
                        t.voxels[j].index = c.voxels[tetrahedraIds[i][j]].index;
                    }
                    tetrahedras.push_back(t);
                }

                for (const Tetrahedra& tetrahedra : tetrahedras) {
                    // Step three: Calculate for tetra case index

                    //if(iso < 0) std::cout << "HELLO" << std::endl;
                    unsigned int case_id = 0b0000;
                    if(tetrahedra.voxels[0].value < iso) case_id |= 1;
                    if(tetrahedra.voxels[1].value < iso) case_id |= 2;
                    if(tetrahedra.voxels[2].value < iso) case_id |= 4;
                    if(tetrahedra.voxels[3].value < iso) case_id |= 8;

                    std::vector<Voxel> voxels;
                    std::vector<unsigned long> vert_idx;

                    Voxel v0;
                    v0.pos = tetrahedra.voxels[0].pos;
                    v0.value = tetrahedra.voxels[0].value;
                    v0.index  = tetrahedra.voxels[0].index;

                    Voxel v1;
                    v1.pos = tetrahedra.voxels[1].pos;
                    v1.value = tetrahedra.voxels[1].value;
                    v1.index = tetrahedra.voxels[1].index;

                    Voxel v2;
                    v2.pos = tetrahedra.voxels[2].pos;
                    v2.value = tetrahedra.voxels[2].value;
                    v2.index = tetrahedra.voxels[2].index;
                    
                    Voxel v3;
                    v3.pos = tetrahedra.voxels[3].pos;
                    v3.value = tetrahedra.voxels[3].value;
                    v3.index = tetrahedra.voxels[3].index;
                    
                    switch(case_id) {
                    case 1:
                    case 14:
                        // One triangle
                        if(case_id == 1) addTriangles(mesh, iso, std::vector<Voxel>{v1, v2, v3, v0}, std::vector<size_t>{v1.index, v2.index, v3.index, v0.index}, true);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v3, v2, v1, v0 }, std::vector<size_t>{v3.index, v2.index, v1.index, v0.index}, true);
                        break;
                    case 2:
                    case 13:
                       // one triangle
                        if(case_id == 2) addTriangles(mesh, iso, std::vector<Voxel>{v0, v2, v3, v1 }, std::vector<size_t>{v0.index, v2.index, v3.index, v1.index}, true);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v3, v2, v0, v1 }, std::vector<size_t>{v3.index, v2.index, v0.index, v1.index}, true);
                        break;
                    case 3:
                    case 12:
                        // Two triangles
                        if(case_id == 3) addTriangles(mesh, iso, std::vector<Voxel>{v3, v2, v0, v1 }, std::vector<size_t>{v3.index, v2.index, v0.index, v1.index}, false);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v1, v0, v2, v3 }, std::vector<size_t>{v1.index, v0.index, v2.index, v3.index}, false);
                        break;
                    case 4:
                    case 11:                    
                        // One triangle
                        if(case_id == 4) addTriangles(mesh, iso, std::vector<Voxel>{v3, v1, v0, v2 }, std::vector<size_t>{v3.index, v1.index, v0.index, v2.index}, true);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v0, v1, v3, v2 }, std::vector<size_t>{v0.index, v1.index, v3.index, v2.index}, true);
                        break;
                    case 5:
                    case 10:
                        // Two triangles
                        if(case_id == 5) addTriangles(mesh, iso, std::vector<Voxel>{v1, v3, v0, v2 }, std::vector<size_t>{v1.index, v3.index, v0.index, v2.index}, false);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v2, v0, v3, v1 }, std::vector<size_t>{v2.index, v0.index, v3.index, v1.index}, false);
                        break;
                    case 6:
                    case 9:
                        // Two triangles
                        if(case_id == 6) addTriangles(mesh, iso, std::vector<Voxel>{v3, v0, v1, v2 }, std::vector<size_t>{v3.index, v0.index, v1.index, v2.index}, false);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v2, v1, v0, v3 }, std::vector<size_t>{v2.index, v1.index, v0.index, v3.index}, false);
                        break;
                    case 7:
                    case 8:
                        // One triangle
                        if(case_id == 7) addTriangles(mesh, iso, std::vector<Voxel>{v0, v1, v2, v3 }, std::vector<size_t>{v0.index, v1.index, v2.index, v3.index}, true);
                        else addTriangles(mesh, iso, std::vector<Voxel>{v2, v1, v0, v3 }, std::vector<size_t>{v2.index, v1.index, v0.index, v3.index}, true);
                        break;
                    default:
                        break;
                    }
                    
                }



                    //step four: Extract triangles
                
            }
        }
    }
    mesh_.setData(mesh.toBasicMesh());
}


MarchingTetrahedra::MeshHelper::MeshHelper(std::shared_ptr<const Volume> vol)
    : edgeToVertex_()
    , vertices_()
    , mesh_(std::make_shared<BasicMesh>())
    , indexBuffer_(mesh_->addIndexBuffer(DrawType::Triangles, ConnectivityType::None)) {
    mesh_->setModelMatrix(vol->getModelMatrix());
    mesh_->setWorldMatrix(vol->getWorldMatrix());
}

void MarchingTetrahedra::MeshHelper::addTriangle(size_t i0, size_t i1, size_t i2) {
    IVW_ASSERT(i0 != i1, "i0 and i1 should not be the same value");
    IVW_ASSERT(i0 != i2, "i0 and i2 should not be the same value");
    IVW_ASSERT(i1 != i2, "i1 and i2 should not be the same value");

    indexBuffer_->add(static_cast<glm::uint32_t>(i0));
    indexBuffer_->add(static_cast<glm::uint32_t>(i1));
    indexBuffer_->add(static_cast<glm::uint32_t>(i2));

    const auto a = std::get<0>(vertices_[i0]);
    const auto b = std::get<0>(vertices_[i1]);
    const auto c = std::get<0>(vertices_[i2]);

    const vec3 n = glm::normalize(glm::cross(b - a, c - a));
    std::get<1>(vertices_[i0]) += n;
    std::get<1>(vertices_[i1]) += n;
    std::get<1>(vertices_[i2]) += n;
}

std::shared_ptr<BasicMesh> MarchingTetrahedra::MeshHelper::toBasicMesh() {
    for (auto& vertex : vertices_) {
        std::get<1>(vertex) = glm::normalize(std::get<1>(vertex));
    }
    mesh_->addVertices(vertices_);
    return mesh_;
}

std::uint32_t MarchingTetrahedra::MeshHelper::addVertex(vec3 pos, size_t i, size_t j) {
    IVW_ASSERT(i != j, "i and j should not be the same value");
    if (j < i) std::swap(i, j);

    auto [edgeIt, inserted] = edgeToVertex_.try_emplace(std::make_pair(i, j), vertices_.size());
    if (inserted) {
        vertices_.push_back({pos, vec3(0, 0, 0), pos, vec4(0.7f, 0.7f, 0.7f, 1.0f)});
    }
    return static_cast<std::uint32_t>(edgeIt->second);
}

}  // namespace inviwo
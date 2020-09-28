#pragma once

#include <modules/tnm067lab2/tnm067lab2moduledefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>

namespace inviwo {

class IVW_MODULE_TNM067LAB2_API MarchingTetrahedra : public Processor {
public:
    struct HashFunc {
        static size_t max;
        size_t operator()(std::pair<size_t, size_t> p) const {
            size_t asdf;
            asdf = p.first;
            asdf += p.second * max;
            return std::hash<size_t>{}(asdf);
        }
    };

    struct Voxel {
        vec3 pos;
        float value;
        size_t index;
    };

    struct Cell {
        Voxel voxels[8];
    };

    struct Tetrahedra {
        Voxel voxels[4];
    };

    struct MeshHelper {

        MeshHelper(std::shared_ptr<const Volume> vol);

        /**
         * Adds a vertex to the mesh. The input parameters i and j are the voxel-indices of the two
         * voxels spanning the edge on which the vertex lies. The vertex will only be added created
         * if a vertex between the same indices has not been added before. Will return the index of
         * the created vertex or the vertex that was created for this edge before. The voxel-index i
         * and j can be given in any order.
         *
         * @param pos spatial position of the vertex
         * @param i voxel index of first voxel of the edge
         * @param j voxel index of second voxel of the edge
         */
        std::uint32_t addVertex(vec3 pos, size_t i, size_t j);
        void addTriangle(size_t i0, size_t i1, size_t i2);
        std::shared_ptr<BasicMesh> toBasicMesh();

    private:
        std::unordered_map<std::pair<size_t, size_t>, size_t, HashFunc> edgeToVertex_;
        std::vector<BasicMesh::Vertex> vertices_;
        std::shared_ptr<BasicMesh> mesh_;
        std::shared_ptr<IndexBufferRAM> indexBuffer_;
    };

    MarchingTetrahedra();
    virtual ~MarchingTetrahedra() = default;

    virtual void process() override;

    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

private:
    VolumeInport volume_;
    MeshOutport mesh_;

    FloatProperty isoValue_;
};

}  // namespace inviwo

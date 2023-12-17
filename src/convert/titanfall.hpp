#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../common.hpp"
#include "../bsp/respawn.hpp"


namespace titanfall {
    struct VertexGLTF {
        Vector    position;       // POSITION
        Vector    normal;         // NORMAL
        TexCoord  uv;             // TEXCOORD_0
        ByteRGBA  colour;         // COLOR_0
        TexCoord  lightmap_uv;    // TEXCOORD_1
        TexCoord  lightmap_step;  // TEXCOORD_2
        // TODO: tangent (vec2) + lightmap index + cubemap index
        // -- pack into COLOR_1?
    };


    struct MetaGLTF {
        uint32_t     first_byte;      // accessor.byteOffset
        uint32_t     num_vertices;    // accessor.count
        uint32_t     lightmap_index;  // TODO: encode in VertexGLTF
        std::string  material_name;   // gltf.material.name
    };


    struct StateGLTF {
        std::vector<VertexGLTF>   buffer_vertices;
        std::vector<uint32_t>     buffer_indices;
        struct {
            uint32_t  unlit;
            uint32_t  lit_bump;
            uint32_t  lit_flat;
            uint32_t  unlit_ts;
        } offset;
        std::vector<MetaGLTF>     metadata;  // per-primitive
        // TODO: per-mesh metadata
        // TODO: uri filenames
        std::vector<std::string>  gltf;
    };


    /* FUNCTIONS */

    void gather_vertices(respawn::Bsp &bsp, StateGLTF &state);

    void gather_indices(respawn::Bsp &bsp, StateGLTF &state);

    void generate_gltf(StateGLTF &state);

    // void generate_glb(StateGLTF &state);
};

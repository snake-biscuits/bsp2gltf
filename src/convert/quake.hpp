#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../common.hpp"
#include "../bsp/quake.hpp"


namespace quake {
    struct VertexGLTF {
        Vector    position;       // POSITION
        Vector    normal;         // NORMAL
        TexCoord  uv;             // TEXCOORD_0
        ByteRGBA  colour;         // COLOR_0
        TexCoord  lightmap_uv;    // TEXCOORD_1
        // TODO: convert LIGHTING lump into texture atlas
    };


    struct MetaGLTF {
        uint32_t     first_byte;      // accessor.byteOffset
        uint32_t     num_vertices;    // accessor.count
        std::string  material_name;   // gltf.material.name
    };


    typedef struct {
        std::vector<VertexGLTF>   buffer_vertices;
        std::vector<uint32_t>     buffer_indices;
        std::vector<MetaGLTF>     metadata;  // per-primitive
        // TODO: per-mesh metadata
        // TODO: uri filenames
        std::vector<std::string>  gltf;
    } StateGLTF;


    /* FUNCTIONS */

    void gather_vertices(Bsp &bsp, StateGLTF &state);

    void gather_indices(Bsp &bsp, StateGLTF &state);

    void generate_gltf(StateGLTF &state);

    // void generate_glb(StateGLTF &state);
}

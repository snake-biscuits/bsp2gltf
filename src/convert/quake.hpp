#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../bsp/quake.hpp"


namespace quake {
    typedef struct {
        float    position[3];       // POSITION
        float    normal[3];         // NORMAL
        float    uv[2];             // TEXCOORD_0
        uint8_t  colour[4];         // COLOR_0
        float    lightmap_uv[2];    // TEXCOORD_1
        // TODO: convert LIGHTING lump into texture atlas
    } VertexGLTF;


    typedef struct {
        uint32_t     first_byte;      // accessor.byteOffset
        uint32_t     num_vertices;    // accessor.count
        std::string  material_name;   // gltf.material.name
    } MetaGLTF;


    typedef struct {
        std::vector<VertexGLTF>   buffer_vertices;
        std::vector<uint32_t>     buffer_indices;
        std::vector<MetaGLTF>     metadata;  // per-primitive
        // TODO: per-mesh metadata
        // TODO: uri filenames
        std::vector<std::string>  gltf;
    } StateGLTF;


    void gather_vertices(Bsp &bsp, StateGLTF &state);

    void gather_indices(Bsp &bsp, StateGLTF &state);

    void generate_gltf(StateGLTF &state);

    // void generate_glb(StateGLTF &state);
}

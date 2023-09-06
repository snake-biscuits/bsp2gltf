#include <cstdint>
#include <string>
#include <vector>

#include "../bsp/respawn.hpp"


namespace titanfall {
    typedef struct {
        float    position[3];       // POSITION
        float    normal[3];         // NORMAL
        float    uv[2];             // TEXCOORD_0
        uint8_t  colour[4];         // COLOR_0
        float    lightmap_uv[2];    // TEXCOORD_1
        float    lightmap_step[2];  // TEXCOORD_2
        // TODO: tangent (vec2) + lightmap index + cubemap index
        // -- pack into COLOR_1?
    } VertexGLTF;


    typedef struct {
        uint32_t     first_byte;      // accessor.byteOffset
        uint32_t     num_vertices;    // accessor.count
        uint32_t     lightmap_index;  // TODO: encode in VertexGLTF
        std::string  material_name;   // gltf.material.name
    } MetaGLTF;


    typedef struct {
        std::vector<VertexGLTF>   buffer_vertices;
        std::vector<uint32_t>     buffer_indices;
        struct {
            uint32_t unlit, lit_bump, lit_flat, unlit_ts;
        } offset;
        std::vector<MetaGLTF>     metadata;  // per-primitive
        // TODO: per-mesh metadata
        // TODO: uri filenames
        std::vector<std::string>  gltf;
    } StateGLTF;


    void gather_vertices(RespawnBsp &bsp, StateGLTF &state);

    void gather_indices(RespawnBsp &bsp, StateGLTF &state);

    void generate_gltf(StateGLTF &state);

    // void generate_glb(StateGLTF &state);
};

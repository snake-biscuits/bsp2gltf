#pragma once

#include <cstdint>


namespace titanfall {
    typedef struct {  // lump 0x52 (82)
        int16_t  texture_data, lightmap_header, cubemap, last_vertex;
        int32_t  vertex_offset;
    } MaterialSort;


    typedef struct {  // lump 0x50 (80)
        uint32_t  first_mesh_index;
        uint16_t  num_triangles;
        uint16_t  first_vertex, num_vertices;
        uint16_t  vertex_type;
        int8_t    styles[4];
        int16_t   luxel_origin[2];
        uint8_t   luxel_offset_max[2];
        uint16_t  material_sort;
        uint32_t  flags;
    } Mesh;


    namespace MeshFlags {
        const int  VERTEX_LIT_FLAT = 0x000;
        const int  VERTEX_LIT_BUMP = 0x200;
        const int  VERTEX_UNLIT    = 0x400;
        const int  VERTEX_UNLIT_TS = 0x600;
    };


    typedef struct {  // lump 0x0E (14)
        float     mins[3], maxs[3];
        uint32_t  first_mesh, num_meshes;
    } Model;


    typedef struct {  // lump 0x02 (2)
        float     reflectivity[3];
        uint32_t  name_index, size[2], view[2], flags;
    } TextureData;


    typedef struct {  // lump 0x48 (72)
        uint32_t  position_index, normal_index;
        float     uv[2];
        uint8_t   colour[4];
        struct { float uv[2], step[2]; }  lightmap;
    } VertexLitFlat;


    typedef struct {  // lump 0x49 (73)
        uint32_t  position_index, normal_index;
        float     uv[2];
        uint8_t   colour[4];
        struct { float uv[2], step[2]; }  lightmap;
        int32_t   tangent[2];
    } VertexLitBump;


    typedef struct {  // lump 0x47 (71)
        uint32_t  position_index, normal_index;
        float     uv[2];
        uint8_t   colour[4];
    } VertexUnlit;


    typedef struct {  // lump 0x4A (74)
        uint32_t  position_index, normal_index;
        float     uv[2];
        uint8_t   colour[4];
        int32_t   tangent[2];
    } VertexUnlitTS;
};

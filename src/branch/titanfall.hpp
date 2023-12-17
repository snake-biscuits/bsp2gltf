#pragma once

#include <cstddef>
#include <cstdint>

#include "../common.hpp"


namespace titanfall {
    const int VERSION = 29;
    /* LUMP INDICES */
    const int TEXTURE_DATA    = 0x02;
    const int VERTICES        = 0x03;  // quake::Vertex
    const int MODELS          = 0x0E;
    const int VERTEX_NORMALS  = 0x1E;  // quake::Vertex
    const int VERTEX_UNLIT    = 0x47;
    const int VERTEX_LIT_FLAT = 0x48;
    const int VERTEX_LIT_BUMP = 0x49;
    const int VERTEX_UNLIT_TS = 0x4A;
    const int MESHES          = 0x50;
    const int MATERIAL_SORTS  = 0x52;


    struct MaterialSort {  // lump 0x52 (82)
        int16_t  texture_data;
        int16_t  lightmap_header;
        int16_t  cubemap;
        int16_t  last_vertex;
        int32_t  vertex_offset;
    };
    static_assert(sizeof(MaterialSort) == 0xC);
    static_assert(offsetof(MaterialSort, texture_data)    == 0x0);
    static_assert(offsetof(MaterialSort, lightmap_header) == 0x2);
    static_assert(offsetof(MaterialSort, cubemap)         == 0x4);
    static_assert(offsetof(MaterialSort, last_vertex)     == 0x6);
    static_assert(offsetof(MaterialSort, vertex_offset)   == 0x8);


    struct Mesh {  // lump 0x50 (80)
        uint32_t  first_mesh_index;
        uint16_t  num_triangles;
        uint16_t  first_vertex;
        uint16_t  num_vertices;
        uint16_t  vertex_type;
        int8_t    styles[4];
        int16_t   luxel_origin[2];
        uint8_t   luxel_offset_max[2];
        uint16_t  material_sort;
        uint32_t  flags;
    };
    static_assert(sizeof(Mesh) == 0x1C);
    static_assert(offsetof(Mesh, first_mesh_index) == 0x00);
    static_assert(offsetof(Mesh, num_triangles)    == 0x04);
    static_assert(offsetof(Mesh, first_vertex)     == 0x06);
    static_assert(offsetof(Mesh, num_vertices)     == 0x08);
    static_assert(offsetof(Mesh, vertex_type)      == 0x0A);
    static_assert(offsetof(Mesh, styles)           == 0x0C);
    static_assert(offsetof(Mesh, luxel_origin)     == 0x10);
    static_assert(offsetof(Mesh, luxel_offset_max) == 0x14);
    static_assert(offsetof(Mesh, material_sort)    == 0x16);
    static_assert(offsetof(Mesh, flags)            == 0x18);


    namespace MeshFlags {
        const int  VERTEX_LIT_FLAT = 0x000;
        const int  VERTEX_LIT_BUMP = 0x200;
        const int  VERTEX_UNLIT    = 0x400;
        const int  VERTEX_UNLIT_TS = 0x600;
    };


    struct Model {  // lump 0x0E (14)
        Vector    mins;
        Vector    maxs;
        uint32_t  first_mesh;
        uint32_t  num_meshes;
    };
    static_assert(sizeof(Model) == 0x20);
    static_assert(offsetof(Model, mins)       == 0x00);
    static_assert(offsetof(Model, maxs)       == 0x0C);
    static_assert(offsetof(Model, first_mesh) == 0x18);
    static_assert(offsetof(Model, num_meshes) == 0x1C);


    struct TextureData {  // lump 0x02 (2)
        FloatRGB  reflectivity;
        uint32_t  name_index;
        uint32_t  size[2];  // width, height
        uint32_t  view[2];  // width, height
        uint32_t  flags;
    };
    static_assert(sizeof(TextureData) == 0x24);
    static_assert(offsetof(TextureData, reflectivity) == 0x00);
    static_assert(offsetof(TextureData, name_index)   == 0x0C);
    static_assert(offsetof(TextureData, size)         == 0x10);
    static_assert(offsetof(TextureData, view)         == 0x18);
    static_assert(offsetof(TextureData, flags)        == 0x20);


    struct VertexLitFlat {  // lump 0x48 (72)
        uint32_t  position_index;
        uint32_t  normal_index;
        TexCoord  uv;
        ByteRGBA  colour;
        TexCoord  lightmap_uv;
        TexCoord  lightmap_step;
    };
    static_assert(sizeof(VertexLitFlat) == 0x24);
    static_assert(offsetof(VertexLitFlat, position_index) == 0x00);
    static_assert(offsetof(VertexLitFlat, normal_index)   == 0x04);
    static_assert(offsetof(VertexLitFlat, uv)             == 0x08);
    static_assert(offsetof(VertexLitFlat, colour)         == 0x10);
    static_assert(offsetof(VertexLitFlat, lightmap_uv)    == 0x14);
    static_assert(offsetof(VertexLitFlat, lightmap_step)  == 0x1C);


    struct VertexLitBump {  // lump 0x49 (73)
        uint32_t  position_index;
        uint32_t  normal_index;
        TexCoord  uv;
        ByteRGBA  colour;
        TexCoord  lightmap_uv;
        TexCoord  lightmap_step;
        int32_t   tangent[2];
    };
    static_assert(sizeof(VertexLitBump) == 0x2C);
    static_assert(offsetof(VertexLitBump, position_index) == 0x00);
    static_assert(offsetof(VertexLitBump, normal_index)   == 0x04);
    static_assert(offsetof(VertexLitBump, uv)             == 0x08);
    static_assert(offsetof(VertexLitBump, colour)         == 0x10);
    static_assert(offsetof(VertexLitBump, lightmap_uv)    == 0x14);
    static_assert(offsetof(VertexLitBump, lightmap_step)  == 0x1C);
    static_assert(offsetof(VertexLitBump, tangent)        == 0x24);


    struct VertexUnlit {  // lump 0x47 (71)
        uint32_t  position_index;
        uint32_t  normal_index;
        TexCoord  uv;
        ByteRGBA  colour;
    };
    static_assert(sizeof(VertexUnlit) == 0x14);
    static_assert(offsetof(VertexUnlit, position_index) == 0x00);
    static_assert(offsetof(VertexUnlit, normal_index)   == 0x04);
    static_assert(offsetof(VertexUnlit, uv)             == 0x08);
    static_assert(offsetof(VertexUnlit, colour)         == 0x10);


    struct VertexUnlitTS {  // lump 0x4A (74)
        uint32_t  position_index;
        uint32_t  normal_index;
        TexCoord  uv;
        ByteRGBA  colour;
        int32_t   tangent[2];
    };
    static_assert(sizeof(VertexUnlitTS) == 0x1C);
    static_assert(offsetof(VertexUnlitTS, position_index) == 0x00);
    static_assert(offsetof(VertexUnlitTS, normal_index)   == 0x04);
    static_assert(offsetof(VertexUnlitTS, uv)             == 0x08);
    static_assert(offsetof(VertexUnlitTS, colour)         == 0x10);
    static_assert(offsetof(VertexUnlitTS, tangent)        == 0x14);
};

#include <cstdint>
#include <cstring>

#include "../branch/titanfall.hpp"
#include "../branch/quake.hpp"
#include "../common.hpp"
#include "../gltf.hpp"
#include "titanfall.hpp"


namespace titanfall {
    void gather_vertices(respawn::Bsp &bsp, StateGLTF &state) {
        // get lumps
        GET_LUMP(quake::Vertex,  VERTICES,         0x03);
        GET_LUMP(quake::Vertex,  VERTEX_NORMALS,   0x1E);
        GET_LUMP(VertexUnlit,    VERTEX_UNLIT,     0x47);
        GET_LUMP(VertexLitFlat,  VERTEX_LIT_FLAT,  0x48);
        GET_LUMP(VertexLitBump,  VERTEX_LIT_BUMP,  0x49);
        GET_LUMP(VertexUnlitTS,  VERTEX_UNLIT_TS,  0x4A);
        // generate vertex buffer
        VertexGLTF  gltf_vertex;
        // NOTE: quake::Vertex hates cast & memcpy, so we do it the hard way
        #define COPY_VERTEX_BASE() \
            auto position = VERTICES[bsp_vertex.position_index];\
            gltf_vertex.position[0] =  position.x; \
            gltf_vertex.position[1] =  position.z; \
            gltf_vertex.position[2] = -position.y; \
            auto normal = VERTEX_NORMALS[bsp_vertex.normal_index]; \
            gltf_vertex.normal[0] = normal.x; \
            gltf_vertex.normal[1] = normal.y; \
            gltf_vertex.normal[2] = normal.z; \
            std::memcpy(gltf_vertex.uv,     bsp_vertex.uv,     sizeof(float)   * 2); \
            std::memcpy(gltf_vertex.colour, bsp_vertex.colour, sizeof(uint8_t) * 4)
        for (const auto &bsp_vertex : VERTEX_UNLIT) {
            COPY_VERTEX_BASE();
            memset(gltf_vertex.lightmap_uv,   0, sizeof(float) * 2);
            memset(gltf_vertex.lightmap_step, 0, sizeof(float) * 2);
            state.buffer_vertices.emplace_back(gltf_vertex);
        }
        for (const auto &bsp_vertex : VERTEX_LIT_FLAT) {
            COPY_VERTEX_BASE();
            memcpy(gltf_vertex.lightmap_uv,   bsp_vertex.lightmap.uv,   sizeof(float) * 2);
            memcpy(gltf_vertex.lightmap_step, bsp_vertex.lightmap.step, sizeof(float) * 2);
            state.buffer_vertices.emplace_back(gltf_vertex);
        }
        for (const auto &bsp_vertex : VERTEX_LIT_BUMP) {
            COPY_VERTEX_BASE();
            memcpy(gltf_vertex.lightmap_uv,   bsp_vertex.lightmap.uv,   sizeof(float) * 2);
            memcpy(gltf_vertex.lightmap_step, bsp_vertex.lightmap.step, sizeof(float) * 2);
            // TODO: tangent
            state.buffer_vertices.emplace_back(gltf_vertex);
        }
        for (const auto &bsp_vertex : VERTEX_UNLIT_TS) {
            COPY_VERTEX_BASE();
            memset(gltf_vertex.lightmap_uv,   0, sizeof(float) * 2);
            memset(gltf_vertex.lightmap_step, 0, sizeof(float) * 2);
            // TODO: tangent
            state.buffer_vertices.emplace_back(gltf_vertex);
        }
        #undef COPY_VERTEX_BASE
        // TODO: pass offsets to index_buffer() func
        state.offset.unlit    = 0;
        state.offset.lit_flat = VERTEX_UNLIT.size();
        state.offset.lit_bump = state.offset.lit_flat + VERTEX_LIT_FLAT.size();
        state.offset.unlit_ts = state.offset.lit_bump + VERTEX_LIT_BUMP.size();
    }


    void gather_indices(respawn::Bsp &bsp, StateGLTF &state) {
        // get lumps
        GET_LUMP(TextureData,    TEXTURE_DATA,     0x02);
        GET_LUMP(Model,          MODELS,           0x0E);
        GET_LUMP(uint32_t,       STRING_TABLE,     0x2C);
        GET_LUMP(uint16_t,       MESH_INDICES,     0x4F);
        GET_LUMP(Mesh,           MESHES,           0x50);
        GET_LUMP(MaterialSort,   MATERIAL_SORTS,   0x52);
        char* STRING_DATA;
        STRING_DATA = static_cast<char*>(malloc(bsp.header.lumps[0x2B].length));
        bsp.load_lump_raw(0x2B, STRING_DATA);
        // generate index buffer
        MetaGLTF  meta;
        uint16_t  previous_material_sort = -1;
        auto  worldspawn = MODELS[0];
        for (int i = 0; i < static_cast<int>(worldspawn.num_meshes); i++) {
            auto      mesh = MESHES[i];
            uint32_t  gltf_offset;
            switch (mesh.flags & 0x600) {
                case MeshFlags::VERTEX_UNLIT:     gltf_offset = state.offset.unlit;     break;
                case MeshFlags::VERTEX_LIT_FLAT:  gltf_offset = state.offset.lit_flat;  break;
                case MeshFlags::VERTEX_LIT_BUMP:  gltf_offset = state.offset.lit_bump;  break;
                case MeshFlags::VERTEX_UNLIT_TS:  gltf_offset = state.offset.unlit_ts;  break;
                default:  gltf_offset = 0;  // TODO: std::unreachable (C++23)
            }
            auto material_sort = MATERIAL_SORTS[mesh.material_sort];
            const int start = static_cast<int>(mesh.first_mesh_index);
            const int end = start + mesh.num_triangles * 3;
            for (int j = start; j < end; j++) {
                auto mesh_index = MESH_INDICES[j];
                auto index = material_sort.vertex_offset + mesh_index + gltf_offset;
                // TODO: bake material_sort.{cubemap,lightmap}_index into GLTFVertex[index]
                state.buffer_indices.push_back(index);
            }
            // metadata
            if (previous_material_sort == static_cast<uint16_t>(-1)) {
                previous_material_sort = mesh.material_sort;
                meta.first_byte     = 0;
                meta.num_vertices   = 0;
                meta.lightmap_index = material_sort.lightmap_header;
                auto  texture_data = TEXTURE_DATA[material_sort.texture_data];
                meta.material_name = sanitise(&STRING_DATA[STRING_TABLE[texture_data.name_index]]);
            }
            if (mesh.material_sort == previous_material_sort) {
                meta.num_vertices += mesh.num_triangles * 3;
            } else {  // changeover
                state.metadata.push_back(meta);
                previous_material_sort = mesh.material_sort;
                meta.num_vertices = mesh.num_triangles * 3;
                meta.first_byte = (state.buffer_indices.size() - meta.num_vertices) * sizeof(uint32_t);
                meta.lightmap_index = material_sort.lightmap_header;
                auto  texture_data = TEXTURE_DATA[material_sort.texture_data];
                meta.material_name = sanitise(&STRING_DATA[STRING_TABLE[texture_data.name_index]]);
            }
        }
        state.metadata.push_back(meta);
        free(STRING_DATA);
    }


    void generate_gltf(StateGLTF &state) {
        char         chrbuf[4096];
        std::string  strbuf;
        #define WRITE_GLTF(...) \
            sprintf(chrbuf, __VA_ARGS__);  strbuf += chrbuf; \
            state.gltf.push_back(strbuf);  strbuf = ""
        auto vertex_buffer_length = state.buffer_vertices.size() * sizeof(VertexGLTF);
        auto index_buffer_length  = state.buffer_indices.size()  * sizeof(uint32_t);
        WRITE_GLTF("{\"scene\": 0, \"scenes\": [{\"nodes\": [0]}], \"nodes\": [{\"mesh\": 0}],\n");
        WRITE_GLTF(" \"meshes\": [{\"primitives\": [\n");
        #define FOREACH_META  for (unsigned long int i = 0; i < state.metadata.size(); i++)
        FOREACH_META {
            WRITE_GLTF("  {\"attributes\": {\"POSITION\": 0, \"NORMAL\": 1, \"TEXCOORD_0\": 2, ");
            WRITE_GLTF("\"COLOR_0\": 3, \"TEXCOORD_1\": 4, \"TEXCOORD_2\": 5}, ");
            WRITE_GLTF("\"indices\": %lu, \"material\": %lu}", 6 + i, i);
            if (i < state.metadata.size() - 1) { WRITE_GLTF(",\n");    }
            else                               { WRITE_GLTF("]}],\n"); }
        }
        WRITE_GLTF( "\"materials\": [\n");
        FOREACH_META {
            auto  meta = state.metadata[i];
            if (meta.lightmap_index != static_cast<uint32_t>(-1)) {
                WRITE_GLTF("  {\"name\": \"%s(%d)\"}", meta.material_name.c_str(), meta.lightmap_index);
            } else {
                WRITE_GLTF("  {\"name\": \"%s\"}", meta.material_name.c_str());  // unlit
            }
            if (i < state.metadata.size() - 1) { WRITE_GLTF(",\n");  }
            else                               { WRITE_GLTF("],\n"); }
        }
        WRITE_GLTF(" \"buffers\": [\n");
        // TODO: any filename
        WRITE_GLTF("  {\"uri\": \"bsp.vertices.bin\", \"byteLength\": %lu},\n", vertex_buffer_length);
        WRITE_GLTF("  {\"uri\": \"bsp.indices.bin\", \"byteLength\": %lu}],\n", index_buffer_length);
        WRITE_GLTF(" \"bufferViews\": [\n");
        WRITE_GLTF("  {\"buffer\": 0, \"byteLength\": %lu, \"target\": %d, \"byteStride\": %lu},\n",
                   vertex_buffer_length, GLTF::ARRAY_BUFFER, sizeof(VertexGLTF));
        WRITE_GLTF("  {\"buffer\": 1, \"byteLength\": %lu, \"target\": %d}],\n",
                   index_buffer_length, GLTF::ELEMENT_ARRAY_BUFFER);
        // TODO: accessor mins & maxs
        WRITE_GLTF(" \"accessors\": [\n");
        #define VERTEX_ATTR(T1, T2, m)  WRITE_GLTF( \
            "  {\"bufferView\": 0, \"count\": %lu, \"type\": \"%s\", \"componentType\": %d, \"byteOffset\": %lu},\n", \
            state.buffer_vertices.size(), #T1, T2, offsetof(VertexGLTF, m))
        VERTEX_ATTR(VEC3, GLTF::FLOAT,         position);
        VERTEX_ATTR(VEC3, GLTF::FLOAT,         normal);
        VERTEX_ATTR(VEC2, GLTF::FLOAT,         uv);
        VERTEX_ATTR(VEC4, GLTF::UNSIGNED_BYTE, colour);
        VERTEX_ATTR(VEC2, GLTF::FLOAT,         lightmap_uv);
        VERTEX_ATTR(VEC2, GLTF::FLOAT,         lightmap_step);
        #undef VERTEX_ATTR
        // TODO: 1 gltf.accessor (indices) for each MeshMeta
        FOREACH_META {
            auto  meta = state.metadata[i];
            WRITE_GLTF("  {\"bufferView\": 1, \"count\": %d, \"byteOffset\": %d, \"type\": \"SCALAR\", \"componentType\": %d}",
                       meta.num_vertices, meta.first_byte, GLTF::UNSIGNED_INT);
            if (i < state.metadata.size() - 1) { WRITE_GLTF(",\n");  }
            else                               { WRITE_GLTF("],\n"); }
        }
        WRITE_GLTF(" \"asset\": {\"version\": \"2.0\"}}\n");
    }
};

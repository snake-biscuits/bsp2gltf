#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "bsp/respawn.hpp"
#include "convert/titanfall.hpp"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s FILENAME\n", argv[0]);
        return 0;
    }
    char* filename = argv[1];

    // load bsp data
    // TODO: detect bsp type & select appropriate converter
    RespawnBsp  bsp(filename);
    if (!bsp.is_valid()) {
        fprintf(stderr, "%s is not a Titanfall / Titanfall 2 .bsp!\n", filename);
        return 0;
    }

    titanfall::StateGLTF  state;
    titanfall::gather_vertices(bsp, state);
    titanfall::gather_indices(bsp, state);
    bsp.file.close();

    // write .bin
    char           bin_filename[256];
    std::ofstream  outfile;
    #define WRITE_BIN(n, T) \
        sprintf(bin_filename, "bsp.%s.bin", #n); \
        outfile.open(bin_filename, std::ios::out | std::ios::binary); \
        auto  buffer_##n##_length = state.buffer_##n.size() * sizeof(T); \
        outfile.write(reinterpret_cast<char*>(&state.buffer_##n[0]), buffer_##n##_length); \
        outfile.close();  outfile.clear()
    WRITE_BIN(indices,  uint32_t);
    WRITE_BIN(vertices, titanfall::VertexGLTF);
    #undef WRITE_BIN

    titanfall::generate_gltf(state);

    // write .gltf
    FILE* gltf_file = fopen("bsp.gltf", "w");
    for (std::string &line : state.gltf) {
        std::fprintf(gltf_file, "%s", line.c_str());
    }
    fclose(gltf_file);
    return 0;
}

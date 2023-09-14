#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "bsp/respawn.hpp"
#include "convert/titanfall.hpp"
#include "identify.hpp"

#include "bsp2gltf.hpp"



void convert_titanfall(const char* filename) {
    // .bsp data
    respawn::Bsp  bsp(filename);

    // .bin data & write
    titanfall::StateGLTF  state;
    titanfall::gather_vertices(bsp, state);
    titanfall::gather_indices(bsp, state);
    bsp.file.close();
    write_bin<uint32_t, titanfall::VertexGLTF, titanfall::StateGLTF>(state);

    // .gltf data & write
    titanfall::generate_gltf(state);
    FILE* gltf_file = fopen("bsp.gltf", "w");
    for (std::string &line : state.gltf) {
        std::fprintf(gltf_file, "%s", line.c_str());
    }
    fclose(gltf_file);
}


int main(int argc, char* argv[]) {
    if (argc != 2) { printf("Usage: %s FILENAME\n", argv[0]);  return 0; }

    char* filename = argv[1];

    // TODO: simplify this all down with macros
    // NOTE: indentify already checked format validity
    auto spec = identify(filename);
    switch (spec.bsp_class) {
        // case BspClass::quake:
        //     switch (spec.branch) {
        //         case BspBranch::quake:  convert_quake;  break;
        //         default:  goto JMP_UNSUPPORTED;
        //     }  break;
        case BspClass::respawn:
            switch (spec.branch) {
                case BspBranch::titanfall:  convert_titanfall(filename);  break;
                default:                    goto JMP_UNSUPPORTED;
            }  break;
        default:  goto JMP_UNSUPPORTED;
    }
    goto JMP_END;  // skip the JMP_ table
    JMP_UNSUPPORTED:
        fprintf(stderr, "%s is not a supported .bsp format!\n", filename);
        return 0;
    JMP_END:
        return 0;
}

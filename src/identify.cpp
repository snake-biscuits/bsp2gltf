#include <cstdint>
#include <fstream>

#include "bsp/common.hpp"

#include "identify.hpp"


BspSpec identify(const char* filename) {
    struct id_s {
        uint32_t magic;
        struct { uint16_t major, minor; } version;
    } identifiers;

    auto file = std::ifstream(filename, std::ios::in | std::ios::binary);
    if   (!file.fail()) { file.read(INTO(identifiers), sizeof(id_s));      }
    else                { throw std::runtime_error("Failed to open file"); }
    file.close();

    BspSpec  out;
    #define CLASS_IS(x)   out.bsp_class = BspClass::x
    #define BRANCH_IS(x)  out.branch    = BspBranch::x
    switch (identifiers.magic) {
        case 29:                         CLASS_IS(quake);    break;
        case MAGIC('r', 'B', 'S', 'P'):  CLASS_IS(respawn);
            // NOTE: titanfall & titanfall2 share a converter
            switch (identifiers.version.major) {
                case 29:
                case 36:
                case 37:
                    if   (identifiers.version.minor == 0) { BRANCH_IS(titanfall); }
                    else                                  { BRANCH_IS(unknown);   }  break;
                default:  BRANCH_IS(unknown);
            }  break;
        default:  CLASS_IS(unknown);
    }
    #undef CLASS_IS
    #undef BRANCH_IS
    return out;
}

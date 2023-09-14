#pragma once

#include <cstdint>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "common.hpp"


#define MAGIC_rBSP  MAGIC('r', 'B', 'S', 'P')

namespace respawn {
    struct LumpHeader { uint32_t offset, length, version, fourCC; };
    struct BspHeader {
        uint32_t   magic;
        struct {
            uint16_t major, minor;
        } version;
        uint32_t   revision;
        uint32_t   _127;
        LumpHeader lumps[128];
    };


    class Bsp { public:
        BspHeader      header;
        std::ifstream  file;

        Bsp(const char* filename) {
            this->file = std::ifstream(filename, std::ios::in | std::ios::binary);
            if   (!this->file.fail()) { this->file.read(INTO(this->header), sizeof(BspHeader)); }
            else                      { throw std::runtime_error("Failed to open file");        }
        }

        ~Bsp() {}

        bool is_valid() {
            if (this->header.magic == MAGIC_rBSP) {
                switch (this->header.version.major) {
                    case 29:
                    case 36:  // r2tt
                    case 37:
                    case 47:  // season  0
                    case 48:  // season  7
                        return (this->header.version.minor == 0);
                    case 49:  // season  8 | season 11
                    case 50:  // season 10 | season 11
                        return (this->header.version.minor <= 1);
                    case 51:  // season 13
                        return (this->header.version.minor == 1);
                    default:
                        return false;
                }
            } else {
                return false;
            }
        }

        // TODO: enum for matching engine branch namespace
        // -- 29: titanfall
        // -- 36 & 37: titanfall2
        // -- 47 - 51.1: apex_legends

        // TODO: must load lumps from .bsp_lump if version.minor == 1

        template <typename T>
        void load_lump(const int lump_index, std::vector<T> &lump_vector) {
            auto header = this->header.lumps[lump_index];
            lump_vector.clear();  lump_vector.resize(header.length / sizeof(T));
            this->file.seekg(header.offset);
            this->file.read(INTO(lump_vector[0]), header.length);
        }

        void load_lump_raw(const int lump_index, char* raw_lump) {
            auto header = this->header.lumps[lump_index];
            this->file.seekg(header.offset);
            this->file.read(raw_lump, header.length);
        }
    };
}

#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

#include "common.hpp"


namespace quake {
    struct LumpHeader { uint32_t offset, length; };
    struct BspHeader { uint32_t version; LumpHeader lumps[15]; };


    class Bsp { public:
        BspHeader      header;
        std::ifstream  file;

        Bsp(const char* filename) {
            this->file = std::ifstream(filename, std::ios::in | std::ios::binary);
            if   (!this->file.fail()) { this->file.read(INTO(this->header), sizeof(BspHeader)); }
            else                      { throw std::runtime_error("Failed to open file");        }
        }

        ~Bsp() {}

        bool is_valid() { return (this->header.version == 29); }

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

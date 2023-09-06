#pragma once

#include <cstdint>
#include <fstream>
#include <cstring>
#include <stdexcept>
#include <vector>

#include "common.hpp"


#define MAGIC_rBSP  MAGIC('r', 'B', 'S', 'P')

typedef struct { uint32_t offset, length, version, fourCC; } LumpHeader;
typedef struct { uint32_t magic, version, revision, _127; LumpHeader lumps[128]; } BspHeader;


class RespawnBsp { public:
    BspHeader      header;
    std::ifstream  file;

    RespawnBsp(const char* filename) {
        this->file = std::ifstream(filename, std::ios::in | std::ios::binary);
        if (!this->file.fail()) {
            this->file.read(INTO(this->header), sizeof(BspHeader));
        } else {
            throw std::runtime_error("Failed to open file");
        };
    }

    ~RespawnBsp() {}

    bool is_valid() {
        auto valid = (this->header.magic == MAGIC_rBSP
                   && this->header.version >= 29
                   && this->header.version <= 37);
        return valid;
    }

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

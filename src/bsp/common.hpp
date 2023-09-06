#pragma once

#define INTO(x)  reinterpret_cast<char*>(&x)
#define MAGIC(a, b, c, d)  ((a << 0) | (b << 8) | (c << 16) | (d << 24))
#define GET_LUMP(T, n, i)  std::vector<T>  n;  bsp.load_lump<T>(i, n)


#if 0
template <typename HeaderType>
class BaseBsp { public:
    std::ifstream  file;
    HeaderType     header;

    BaseBsp(const char* filename); /* {
        this->file = std::ifstream(filename, std::ios::in | std::ios::binary);
        if (!this->file.fail()) {
            this->file.read(INTO(this->header), sizeof(BspHeader));
        } else {
            // memset
            memset(this->header.lumps, 0xFF, sizeof(LumpHeader) * 128);
        };
    }
    */

    ~BaseBsp {}

    virtual bool is_valid();  // override per-subclass

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
#endif

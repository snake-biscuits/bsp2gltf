#pragma once

#include <fstream>


template <typename IndexType, typename VertexType, typename StateType>
void write_bin(StateType &state) {
    char           bin_filename[256];
    std::ofstream  outfile;
    // TODO: filename from state
    #define WRITE_BIN(n, T) \
        sprintf(bin_filename, "bsp.%s.bin", #n); \
        outfile.open(bin_filename, std::ios::out | std::ios::binary); \
        auto  buffer_##n##_length = state.buffer_##n.size() * sizeof(T); \
        outfile.write(reinterpret_cast<char*>(&state.buffer_##n[0]), buffer_##n##_length); \
        outfile.close();  outfile.clear()
    WRITE_BIN(indices,  IndexType);
    WRITE_BIN(vertices, VertexType);
    #undef WRITE_BIN
}

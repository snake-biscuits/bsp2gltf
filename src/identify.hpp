#pragma once


enum class BspClass {
    respawn,
    quake,

    unknown
};


enum class BspBranch {
    quake,
    titanfall,

    unknown
};


struct BspSpec {
    BspClass   bsp_class;
    BspBranch  branch;
};


BspSpec identify(const char* filename);

#pragma once

#include <cstddef>
#include <cstdint>


struct Vector {
    float x;
    float y;
    float z;
};
static_assert(sizeof(Vector) == 0xC);
static_assert(offsetof(Vector, x) == 0x0);
static_assert(offsetof(Vector, y) == 0x4);
static_assert(offsetof(Vector, z) == 0x8);


struct TexCoord {
    float u;
    float v;
};
static_assert(sizeof(TexCoord) == 0x8);
static_assert(offsetof(TexCoord, u) == 0x0);
static_assert(offsetof(TexCoord, v) == 0x4);


struct FloatRGB {
    float r;
    float g;
    float b;
};
static_assert(sizeof(FloatRGB) == 0xC);
static_assert(offsetof(FloatRGB, r) == 0x0);
static_assert(offsetof(FloatRGB, g) == 0x4);
static_assert(offsetof(FloatRGB, b) == 0x8);


struct ByteRGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
static_assert(sizeof(ByteRGBA) == 0x4);
static_assert(offsetof(ByteRGBA, r) == 0x0);
static_assert(offsetof(ByteRGBA, g) == 0x1);
static_assert(offsetof(ByteRGBA, b) == 0x2);
static_assert(offsetof(ByteRGBA, a) == 0x3);

#pragma once

#include <cstddef>

/**
 * A read-only buffer, providing its contents and length.
 */
struct OBuffer
{
    OBuffer(const unsigned char* contents, size_t length);
    OBuffer();

    void freeContents();

    const unsigned char* contents;
    size_t length;
};

#pragma once

// This is for size_t
// ReSharper disable once CppUnusedIncludeDirective
#include <cstddef>

/**
 * A read-only buffer, providing its contents and length.
 */
struct OBuffer
{
    OBuffer(const unsigned char* contents, size_t length);
    ~OBuffer();

    const unsigned char* contents;
    size_t length;
};

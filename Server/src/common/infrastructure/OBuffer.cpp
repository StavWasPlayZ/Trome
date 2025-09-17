#include "OBuffer.h"

OBuffer::OBuffer(const unsigned char *const contents, const size_t length) :
    contents(contents),
    length(length)
{}

OBuffer::~OBuffer()
{
    delete[] this->contents;
}

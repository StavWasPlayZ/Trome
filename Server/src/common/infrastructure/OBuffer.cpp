#include "OBuffer.h"

OBuffer::OBuffer(const unsigned char *const contents, const size_t length) :
    contents(contents),
    length(length)
{}

OBuffer::OBuffer() : OBuffer(nullptr, 0)
{}

void OBuffer::freeContents()
{
    delete this->contents;
    *this = OBuffer();
}

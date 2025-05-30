#include "Utils.h"

std::chrono::milliseconds utils::getCurrTimeMillis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    );
}

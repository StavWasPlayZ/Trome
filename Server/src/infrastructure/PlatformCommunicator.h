#pragma once

// This header will define PlatformCommunicator, based on the current active platform.

#ifdef _WIN32
    #include "platform/windows/infrastructure/WindowsCommunicator.h"

    /**
     * The Communicator that should be used on this platform.
     * 
     * Currently set as Windows.
     */
    using PlatformCommunicator = WindowsCommunicator;
#else
    //TODO: Add Linux
#endif

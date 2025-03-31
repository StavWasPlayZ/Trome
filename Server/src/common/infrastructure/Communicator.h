#pragma once

// This header will define PlatformCommunicator, based on the current active platform.

#ifdef _WIN32
    #include "windows/infrastructure/WindowsCommunicator.h"

    /**
     * The Communicator that should be used on this platform.
     * 
     * Currently set as Windows.
     */
    using Communicator = WindowsCommunicator;
#else
    #include "unix/infrastructure/UnixCommunicator.h"

    /**
     * The Communicator that should be used on this platform.
     * 
     * Currently set as Linux.
     */
    using Communicator = UnixCommunicator;
#endif

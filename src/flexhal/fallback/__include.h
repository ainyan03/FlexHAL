#pragma once

// Include sub-components from the fallback layer
#if __has_include("logger/__include.h")
 #include "logger/__include.h"
#endif

// Add other fallback components here if needed in the future

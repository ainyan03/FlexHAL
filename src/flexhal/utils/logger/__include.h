#pragma once

// Include logger interface AND the global access points
#include "ILogger.hpp"
#include "logger_globals.hpp"

// Include the LogProxy class definition AFTER the global declarations/definitions
// This makes the LogProxy class definition available to users of the logger.
#include "LogProxy.hpp"

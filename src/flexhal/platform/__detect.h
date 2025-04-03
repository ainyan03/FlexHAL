#pragma once

// Platform detection file
// This file (or files included by it) should define detection macros like:
// - FLEXHAL_DETECT_PLATFORM_XXX
// - FLEXHAL_DETECT_FRAMEWORK_XXX
// - FLEXHAL_DETECT_RTOS_XXX
// ... and potentially others

// For now, assume hardware and framework features might be used.
// The actual detection logic will reside in hardware/__detect.h and framework/__detect.h
// This just enables the inclusion of those __include.h files if they exist.
#define FLEXHAL_DETECT_HARDWARE 1
#define FLEXHAL_DETECT_FRAMEWORK 1

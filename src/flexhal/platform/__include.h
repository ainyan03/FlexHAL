#pragma once

// Include platform detection logic first
#include "__detect.h"

// Include hardware-specific headers if detected/needed
#if FLEXHAL_DETECT_HARDWARE
 #include "hardware/__include.h"
#endif

// Include framework-specific headers if detected/needed
#if FLEXHAL_DETECT_FRAMEWORK
 #include "framework/__include.h"
#endif

// Note: Specific platform implementations (like 'native') might be included
// elsewhere or determined by a more sophisticated detection mechanism.
// For now, this focuses on the hardware/framework split.

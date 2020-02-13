#include <nuttx/config.h>

#ifdef CONFIG_CHEWIE_ENABLE_LOG
// Log
#define CHEWIE_LOG(format, args...) printf(format, ##args)

#endif
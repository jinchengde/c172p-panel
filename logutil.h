#ifndef LOGUTIL_H
#define LOGUTIL_H

#include <cstdio>
/**
 * Android-style log utility.
 */
#ifndef NDEBUG
#define LOGD(fmt, ...) fprintf(stderr, "DEBUG: " __FILE__ ": " fmt "\n", ##__VA_ARGS__)
#else
#define LOGD(fmt, ...)
#endif

#define LOGI(fmt, ...) fprintf(stderr, "INFO: " __FILE__ ": " fmt "\n", ##__VA_ARGS__)
#define LOGW(fmt, ...) fprintf(stderr, "WARNING: " __FILE__ ": " fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) fprintf(stderr, "ERROR: " __FILE__ ": " fmt "\n", ##__VA_ARGS__)
#define LOGC(fmt, ...) fprintf(stderr, "CRITICAL: " __FILE__ ": " fmt "\n", ##__VA_ARGS__)

#endif // LOGUTIL_H

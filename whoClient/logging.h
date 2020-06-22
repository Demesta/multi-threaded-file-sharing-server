#ifndef WHOCLIENT__LOGGING_H_
#define WHOCLIENT__LOGGING_H_

#include <cstdio>
#include <mutex>

#define DEBUG
#ifdef DEBUG
extern std::mutex log_mutex;
#define LOG(...) do { std::lock_guard<std::mutex> lock(log_mutex); fprintf(stderr, "[INFO] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while(false)
#else
#define LOG(...)
#endif

#endif //WHOCLIENT__LOGGING_H_

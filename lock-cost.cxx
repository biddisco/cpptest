#include <time.h>

#include <cmath>
#include <atomic>
#include <mutex>

static std::atomic<bool> single_threaded(true);

int main(int argc, char *argv[])
{
    (void)argv;
    if (argc == 100001) { single_threaded = !single_threaded; /* to prevent compiler optimization later */ }
    int n = argc == 100000 ? -1 : 100000000;
    {
        std::mutex mutex;
        clock_t const begin = clock();
        unsigned int total = 0;
        for (int i = 0; i < n; ++i)
        {
            if (single_threaded)
            {
                total = ((total << 1) ^ i) + ((total >> 1) & i);
            }
            else
            {
                std::lock_guard<std::mutex> lock(mutex);
                total = ((total << 1) ^ i) + ((total >> 1) & i);
            }
        }
        clock_t const end = clock();
        double elapsed   = (unsigned int)((end - begin) * 1000U / CLOCKS_PER_SEC);
        double ns_per_lk = 1000000*elapsed/n;
        printf("Conditional: %u ms, total = %u\n", int(elapsed), int(total));
        printf("Conditional: %u ns\n", int(ns_per_lk));
    }
    {
        std::mutex mutex;
        clock_t const begin = clock();
        unsigned int total = 0;
        for (int i = 0; i < n; ++i)
        {
            std::lock_guard<std::mutex> lock(mutex);
            total = ((total << 1) ^ i) + ((total >> 1) & i);
        }
        clock_t const end = clock();
        double elapsed   = (unsigned int)((end - begin) * 1000U / CLOCKS_PER_SEC);
        double ns_per_lk = 1000000*elapsed/n;
        printf("Unconditional: %u ms, total = %u\n", int(elapsed), int(total));
        printf("Unconditional: %lu ns\n", std::lround(ns_per_lk));
    }
}

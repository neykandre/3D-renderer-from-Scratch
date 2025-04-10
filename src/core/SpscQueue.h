#pragma once
#include <atomic>
#include <cstddef>

template <typename T, std::size_t N>
class SpscQueue {
    static_assert((N & (N - 1)) == 0, "N must be a power of two");
    alignas(64) std::atomic<std::size_t> head_{ 0 };
    alignas(64) std::atomic<std::size_t> tail_{ 0 };
    T buffer_[N];

  public:
    bool enqueue(const T& item) {
        auto t   = tail_.load(std::memory_order_relaxed);
        auto nxt = (t + 1) & (N - 1);
        if (nxt == head_.load(std::memory_order_acquire)) {
            return false; // full
        }
        buffer_[t] = item;
        tail_.store(nxt, std::memory_order_release);
        return true;
    }

    bool dequeue(T& out) {
        auto h = head_.load(std::memory_order_relaxed);
        if (h == tail_.load(std::memory_order_acquire)) {
            return false; // empty
        }
        out = buffer_[h];
        head_.store((h + 1) & (N - 1), std::memory_order_release);
        return true;
    }
};

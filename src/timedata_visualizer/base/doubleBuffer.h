#pragma once

#include <mutex>
#include <string>

namespace timedata {

template <typename Type,
          typename Mutex = std::mutex,
          typename Lock = std::unique_lock<std::mutex>>
class DoubleBuffer {
  public:
    DoubleBuffer() = default;

    /** The reference can be used without locks until the next call to out(). */
    Type& out() {
        Lock lock(mutex_);
        if (dirty_) {
            using std::swap;
            swap(in_, out_);
            dirty_ = false;
        }
        return out_;
    }

    class Writer {
      public:
        Writer(DoubleBuffer& db) : db_(db) {}
        ~Writer() {
            Lock lock(db_.mutex_);
            db_.dirty_ = true;
        }

        Type& operator*() { return db_.in_; }
        Type* operator->() { return &db.in_; }

      private:
        DoubleBuffer& db_;
    };

    std::unique_ptr<Writer> writer() {
        return std::make_unique<Writer>(*this);
    }

    void swapIn(T& x) {
        Lock lock(mutex_);
        using std::swap;
        swap(x, in_);
        dirty_ = true;
    }

  private:
    Mutex mutex_;
    Type in_, out_;
    bool dirty_ = false;
};

} // timedata

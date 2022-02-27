#ifndef CPP_HW7_MY_FUTURE_H
#define CPP_HW7_MY_FUTURE_H

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

using namespace std;
namespace our {
    template<typename T>
    /* will be owned by both future and promise */
    class shared_state {
        mutex _mutex;
        condition_variable _cv;
        bool _ready;
        T _res;
    public:
        shared_state()
                : _ready(false), _res() { /* empty ctor body */ }

        void set(T &&value) {
            lock_guard<mutex> lock(_mutex);
            /* perfect forwarding */
            _res = forward<T>(value);
            _ready = true;
            _cv.notify_all();
        }

        bool ready() {
            lock_guard<mutex> lock(_mutex);
            return _ready;
        }

        T wait_and_take() {
            unique_lock<mutex> lock(_mutex);
            /* predict to protect against spurious wakeup and lost wakeup*/
            _cv.wait(lock, [this] { return this->_ready; });
            lock.unlock();
            return move(_res);
        }
    };
}
namespace our {
    template<typename T>
    class future {
        shared_ptr<our::shared_state<T>> _shared_state;
    public:
        future(shared_ptr<our::shared_state<T>> shared_state)
                : _shared_state(move(shared_state)) {}

        // Our future will not be copied
        future(const future<T> &) = delete;

        future<T> &operator=(const future<T> &) = delete;

        // Our future may be moved
        future(future<T> &&) = default;

        future<T> &operator=(future<T> &&) noexcept = default;


        // Get the value
        T get() {
            if (!_shared_state)
                throw logic_error("get twice!");

            T value = _shared_state->wait_and_take();
            // Remove our reference to the shared state
            _shared_state = nullptr;
            return move(value);
        }
    };
}

namespace our {
    template<typename T>
    class promise {
        shared_ptr<our::shared_state<T>> _shared_state;
    public:
        promise() {
            _shared_state = make_shared<our::shared_state<T>>();
        }

        // Our promise will not be copied
        promise(const promise<T> &) = delete;

        promise<T> &operator=(const promise<T> &) = delete;

        /* default move constructor, do a member-wise move.*/
        promise(promise<T> &&) = default;

        promise<T> &operator=(promise<T> &&) noexcept = default;

        void set_value(T &&value) {
            if (_shared_state) {
                _shared_state->set(forward<T>(value));
                /*make sure set_value can only take effect once */
                _shared_state = nullptr;
            }
        }

        future<T> get_future() {
            /*create the future*/
            /* _shared_state owned by both future and promise*/
            future<T> future(_shared_state);
            return future;
        }

    };
}


#endif //CPP_HW7_MY_FUTURE_H

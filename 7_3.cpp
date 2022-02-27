#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "my_future.h"

using namespace std;
using namespace our;

void product(promise<int> &&promise, int a, int b) {
    promise.set_value(a * b);
    throw out_of_range("fake out of range!");
}

void exception_handler(promise<int> &&promise, exception_ptr &eptr) {
    try {
        if (eptr) {
            std::rethrow_exception(eptr);
        }
    } catch (const std::exception &e) {
        std::cout << "Caught exception \"" << e.what() << "\"\n";
    }
}

int main() {
    promise<int> _promise;
    future<int> res = _promise.get_future();
    int a = 10;
    int b = -10;
    thread th(product, move(_promise), a, b);
    th.join();
    cout << res.get();


    return 0;
}

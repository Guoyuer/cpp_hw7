#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "my_future.h"

using namespace std;
using namespace MPCS51044;

void product(promise<int> &&promise, int a, int b) {
    promise.set_value(a * b);
}

void product1(promise<int> &&promise, int a, int b) {

    try {
        throw runtime_error("example error");
    } catch (...) {
        promise.set_exception(current_exception());
    }
}

int main() {
    promise<int> _promise;
    future<int> res = _promise.get_future();
    int a = 10;
    int b = -10;
    thread th(product, move(_promise), a, b);
    th.detach();
    res.wait();
    cout << res.get() << endl;

    promise<int> _promise1;
    future<int> res1 = _promise1.get_future();

    thread th1(product1, move(_promise1), a, b);
    th1.detach();
    try {
        cout << res1.get() << endl;
    } catch (const exception &e) {
        cout << e.what() << endl;
    }


    return 0;
}

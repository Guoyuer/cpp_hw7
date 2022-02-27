#include<future>
#include<iostream>

using namespace std;

template<typename Signature>
class my_packaged_task;


template<typename Res, typename... ArgTypes>
class my_packaged_task<Res(ArgTypes...)> {
private:
    function<Res(ArgTypes...)> fn;
public:
    /* normal constructor */
    explicit my_packaged_task(function<Res(ArgTypes...)> f) : fn(move(f)) {}

    /*move constructor*/
    my_packaged_task(my_packaged_task &&f) noexcept {
        fn = move(f.fn);
        _promise = move(f._promise);
    }

    template<typename ...Args>
    void operator()(Args &&...args) {
        _promise.set_value(fn(forward<Args>(args)...));
    }

    promise<Res> _promise;

    future<Res> get_future() {
        return move(_promise.get_future());
    }


};

int product(int a, int b) {
    return a * b;
}

int main() {
    my_packaged_task<int(int, int)> task{product};
    future<int> res = task.get_future();
    int a = 10;
    int b = 20;
    thread th(move(task), a, b);
    th.join();

//    cout << task(a, b);
    cout << res.get();

    return 0;
}
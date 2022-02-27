#include<future>
#include<iostream>

using namespace std;


int product(int a, int b) {
    return a * b;
}

template<typename Func, typename ...Args>
auto my_async(Func f, Args... args) {
    packaged_task<result_of_t<Func(Args...)>(Args...)> task{f};

    future res = task.get_future();
    thread th(move(task), forward<Args>(args)...);
    th.join();
    return res;
}

int main() {
    int a = 10;
    int b = 20;
//    async()
    future res = my_async(product, a, b);
//    static_assert(std::is_same<decltype(res.get()), int>::value);
    cout << res.get();

}
#include<future>
#include<iostream>

using namespace std;


int product(int a, int b) {
    return a * b;
}

template<typename Func, typename ...Args>
auto my_async(Func f, Args... args) {
    packaged_task<invoke_result_t<Func, Args...>(Args...)> task(f);

    auto res = task.get_future();
    thread th(move(task), forward<Args>(args)...);
    th.detach();
    return res;
}

int main() {
    int a = 10;
    int b = 20;
//    async()
    future<int> res = my_async(product, a, b);
//    static_assert(std::is_same<decltype(res.get()), int>::value);
    cout << res.get();

}
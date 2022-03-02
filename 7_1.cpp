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
    future<int> res = my_async(product, a, b);
    cout << res.get();

}
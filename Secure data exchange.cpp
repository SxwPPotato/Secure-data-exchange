#include <iostream>
#include <mutex>
#include <thread>

class Data {
private:
   
public:
    std::mutex g_mutex;
    int count = 5;
    bool selector = true;
    Data(int new_count, bool new_selector) : count{ new_count }, selector{new_selector} {
        
    }
    void show() {

        std::cout << count << "  " << selector << std::endl;
    }

};

void lock_swap(Data& obj1, Data& obj2) {

    std::lock(obj1.g_mutex, obj2.g_mutex);
    std::lock_guard<std::mutex> lk1(obj1.g_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lk2(obj2.g_mutex, std::adopt_lock);

    int tmp1 = std::move(obj1.count);
    obj1.count = std::move(obj2.count);
    obj2.count = std::move(tmp1);

    bool tmp2 = std::move(obj1.selector);
    obj1.selector = std::move(obj2.selector);
    obj2.selector = std::move(tmp2);
}

void unique_swap(Data& obj1, Data& obj2) {

    std::unique_lock<std::mutex> mutexLockGuard1(obj1.g_mutex, std::defer_lock);
    std::unique_lock<std::mutex> mutexLockGuard2(obj2.g_mutex, std::defer_lock);
    std::lock(mutexLockGuard1, mutexLockGuard2);

    int tmp1 = std::move(obj1.count);
    obj1.count = std::move(obj2.count);
    obj2.count = std::move(tmp1);

    bool tmp2 = std::move(obj1.selector);
    obj1.selector = std::move(obj2.selector);
    obj2.selector = std::move(tmp2);

}

void scoped_swap(Data& obj1, Data& obj2) {

    std::scoped_lock mutexLockGuard(obj1.g_mutex, obj2.g_mutex);

    int tmp1 = std::move(obj1.count);
    obj1.count = std::move(obj2.count);
    obj2.count = std::move(tmp1);

    bool tmp2 = std::move(obj1.selector);
    obj1.selector = std::move(obj2.selector);
    obj2.selector = std::move(tmp2);

}

int main()
{
    setlocale(LC_ALL, "russian");

    Data el1(4,true);
    Data el2(9,false);

    std::cout << "До сортировки: ";
    el1.show();
    el2.show();

    unique_swap(el1, el2);

    std::cout << "unique_swap: ";
    el1.show();
    el2.show();

    scoped_swap(el1, el2);

    std::cout << "scoped_swap: ";
    el1.show();
    el2.show();

    lock_swap(el1, el2);

    std::cout << "lock_swap: ";
    el1.show();
    el2.show();

    return 0;
}

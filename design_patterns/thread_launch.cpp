#include <iostream>
#include <thread>

using namespace std;


void do_some_work() {
    cout << "doing some work" << endl;
}

class background_task {
    public:

    void do_something() {
        cout << "do something" << endl;
    }

    void do_something_else() {
        cout << "do something else" << endl;
    }

    void operator! ()
    {
        do_something();
        do_something_else();
    }
};

int main() {
    thread t(do_some_work);
    t.join();
    return 0;
}
#include <iostream>

using namespace std;

class A{
public:
    A(){
        cout << "构造函数调用" << endl;
    }
    ~A(){
        cout << "析构函数调用" << endl;
    }
};
int main(){
    A *pa = new class A[5];
    delete[] pa;
    return 0;
}
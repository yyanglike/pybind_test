#include <csignal>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <thread>

namespace py = pybind11;

// 初始化Python解释器，仅在主线程中调用
void init_python() {
    py::initialize_interpreter();
}

// 清理Python解释器，仅在主线程中调用
void finalize_python() {
    py::finalize_interpreter();
}

// 定义C++回调函数的签名
using CallbackFunc = std::function<void(const int)>;

// C++回调函数，供Python回调
void cpp_callback(const int data) {
   py::gil_scoped_release release;
    std::cout << "C++ callback received data: ";
//    for (int value : data) {
//        std::cout << value << " ";
//    }
    std::cout << data << std::endl;
}

// Python函数，将数据传递给Python模块
void call_python_module(const std::string& module_name, int data) {
//    Py_BEGIN_ALLOW_THREADS
    py::gil_scoped_acquire acquire; // 锁定GIL
    try {
        py::module my_module = py::module::import(module_name.c_str());

        // 调用Python模块中的函数
        py::object set_callback_func = my_module.attr("set_callback");
        py::cpp_function cpp_callback_py = py::cpp_function([](int data) {
            cpp_callback(data);
        });
        
        set_callback_func(cpp_callback_py);

//        py::object receive_data_func = my_module.attr("receive_data");
//        receive_data_func(data);
       py::object receive_data_func = my_module.attr("main");
       receive_data_func();

    } catch (py::error_already_set &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    // py::gil_scoped_release release;  // 解锁GIL
//    Py_END_ALLOW_THREADS
}

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   exit(signum);  
}

int main() {
    signal(SIGINT, signalHandler);  
    // 在主线程中初始化Python解释器
     py::scoped_interpreter guard{};  // 初始化Python解释器
    try {
        py::module sys = py::module::import("sys");
        sys.attr("path").attr("insert")(0, "/Users/yiyang/Downloads/company/python/pybind_test");
    } catch (py::error_already_set &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }


    // 启动多个线程调用Python模块
    py::gil_scoped_release release;
    std::thread t1(call_python_module, "my_module", 5);
    std::thread t2(call_python_module, "my_module1", 10);

    // 等待线程完成
    t1.join();
    t2.join();

    // 在主线程中清理Python解释器
//    finalize_python();

    return 0;
}

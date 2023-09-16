#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace py = pybind11;

int main() {
    py::scoped_interpreter guard{}; // Start the Python interpreter

    // Import the Python module
    py::module module = py::module::import("example");

    // Get the module's dictionary
    py::dict module_dict = module.attr("__dict__");

    // Iterate through the dictionary
    py::list class_names;
    for (auto item : module_dict) {
        // Check if the item is a class
        if (py::isinstance<py::class_>(item.second)) {
            class_names.append(item.first);
        }
    }

    // Print the names of the classes in the module
    for (auto class_name : class_names) {
        std::string class_name_str = py::str(class_name);
        std::cout << "Class name: " << class_name_str << std::endl;

        // Get the class constructor
        py::object class_constructor = module.attr(class_name_str.c_str());

        // Instantiate the class
        py::object class_instance = class_constructor();

        // Call a method on the class instance (if needed)
        // py::object result = class_instance.attr("some_method")();

        // Do something with the class instance
        // ...

        // Optionally, release the class instance to avoid memory leaks
        // class_instance.release();
    }

    return 0;
}

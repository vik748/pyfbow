// Author: Andrius Mikonis (andrius.mikonis@gmail.com)
// License: BSD
// Last modified: Feb 12, 2019

// Wrapper for most external modules
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <exception>

// Opencv includes
#include <opencv2/opencv.hpp>

// np_opencv_converter
#include "np_opencv_converter.hpp"

// fbow
#include "fbow.h"
#include "vocabulary_creator.h"

namespace py = boost::python;

cv::Mat test_np_mat(const cv::Mat& in) {
    std::cerr << "in: " << in << std::endl;
    std::cerr << "sz: " << in.size() << std::endl;
    return in.clone();
}

cv::Mat test_with_args(const cv::Mat_<float>& in, const int& var1 = 1,
    const double& var2 = 10.0, const std::string& name = std::string("test_name")) {
    std::cerr << "in: " << in << std::endl;
    std::cerr << "sz: " << in.size() << std::endl;
    std::cerr << "Returning transpose" << std::endl;
    return in.t();
}

class GenericWrapper {
public:
    GenericWrapper(const int& _var_int = 1, const float& _var_float = 1.f,
        const double& _var_double = 1.f, const std::string& _var_string = std::string("test_string"))
        : var_int(_var_int), var_float(_var_float), var_double(_var_double), var_string(_var_string)
    {

    }

    cv::Mat process(const cv::Mat& in) {
        std::cerr << "in: " << in << std::endl;
        std::cerr << "sz: " << in.size() << std::endl;
        std::cerr << "Returning transpose" << std::endl;
        return in.t();
    }

private:
    int var_int;
    float var_float;
    double var_double;
    std::string var_string;
};

class Vocabulary
{
public:

    Vocabulary(int k = 10, int L = 6, int nthreads = 1, int maxIters = 0, bool verbose = true){
        voc_creator_params.k = k;
        voc_creator_params.L = L;
        voc_creator_params.nthreads = nthreads;
        voc_creator_params.maxIters = maxIters;
        voc_creator_params.verbose = verbose;
    }


    ~Vocabulary() {
        //delete voc;
        //delete voc_creator;
    }


    void create(const  std::vector<cv::Mat>   &training_features ) {
        std::cout << "Creating a " << voc_creator_params.k << "^" << voc_creator_params.L << " vocabulary..." << std::endl;

        voc_creator->create(*voc, training_features, std::string("NA"), voc_creator_params);
    }


    fbow::Vocabulary * voc;
    fbow::VocabularyCreator * voc_creator;
    fbow::VocabularyCreator::Params voc_creator_params;
};


// Wrap a few functions and classes for testing purposes
namespace fs {
    namespace python {

        BOOST_PYTHON_MODULE(pyfbow)
        {
            // Main types export
            fs::python::init_and_export_converters();
            py::scope scope = py::scope();

            // Class
            py::class_<Vocabulary>("Vocabulary")
                .def(py::init< py::optional<int, int, int, int, bool> >(
                    (py::arg("k") = 10, py::arg("L") = 5, py::arg("nthreads") = 1,
                     py::arg("maxIters") = 0, py::arg("verbose") = true )))
                .def("create", &Vocabulary::create);
            //  .def("load", &Vocabulary::load)
            //  .def("save", &Vocabulary::save)
            //  .def("transform", &Vocabulary::transform, py::return_value_policy<py::return_by_value>())
            //  .def("score", &Vocabulary::score)
            //  .def("clear", &Vocabulary::clear);


        }

    } // namespace fs
} // namespace python

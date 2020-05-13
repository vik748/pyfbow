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

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace py = boost::python;

void init_logging()
{
    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );
}


std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

class Vocabulary
{
public:

    Vocabulary(int k = 10, int L = 6, int nthreads = 1, int maxIters = 0, bool verbose = true){
        init_logging();
        voc_creator_params.k = k;
        voc_creator_params.L = L;
        voc_creator_params.nthreads = nthreads;
        voc_creator_params.maxIters = maxIters;
        voc_creator_params.verbose = verbose;
        _verbose = verbose;
        voc = new fbow::Vocabulary();
        voc_creator = new fbow::VocabularyCreator();
    }

    ~Vocabulary() {
        if (_verbose) std::cout << "Entering destructor" << std::endl;
        delete voc;
        delete voc_creator;
        if (_verbose) std::cout << "Exiting destructor" << std::endl;
    }


    void create(const  std::vector<cv::Mat>  &training_feat_vec ) {
        BOOST_LOG_TRIVIAL(info)  << "Creating vocabulary with: k=" << voc_creator_params.k
                                 << ", L=" << voc_creator_params.L
                                 << ", nthreads= " << voc_creator_params.nthreads
                                 << ", maxIters= " << voc_creator_params.maxIters ;

        BOOST_LOG_TRIVIAL(debug) << "Descriptor vec length: " << training_feat_vec.size();
        BOOST_LOG_TRIVIAL(debug) << "Descriptor 0 shape : "
                                 << training_feat_vec[0].rows << " rows x " << training_feat_vec[0].cols
                                 << " cols , type: " << type2str(training_feat_vec[0].type());

        std::srand(0);
        voc_creator->create(*voc, training_feat_vec, std::string("orb"), voc_creator_params);

        BOOST_LOG_TRIVIAL(debug) << "Vocabulary Created" << std::endl;

    }

    void clear() {
        voc->clear();
        BOOST_LOG_TRIVIAL(debug) << "Vocabulary cleared: ";
    }

    void readFromFile(const std::string& path) {
        BOOST_LOG_TRIVIAL(debug) << "Started reading file: " << path;
        voc->readFromFile(path);
        BOOST_LOG_TRIVIAL(debug) << "Completed reading file: " << path;
    }

    void saveToFile(const std::string& path) {
        BOOST_LOG_TRIVIAL(debug) << "Started writing to file: " << path;
        voc->saveToFile(path);
        BOOST_LOG_TRIVIAL(debug) << "Completed Writing to file: " << path;
    }

    fbow::Vocabulary * voc;
    fbow::VocabularyCreator * voc_creator;
    fbow::VocabularyCreator::Params voc_creator_params;
    bool _verbose;
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
            py::class_<Vocabulary>("Vocabulary", "Vocabulary class")
                .def(py::init< py::optional<int, int, int, int, bool> >(
                    (py::arg("k") = 10, py::arg("L") = 5, py::arg("nthreads") = 1,
                     py::arg("maxIters") = 0, py::arg("verbose") = true )))
                .def("create", &Vocabulary::create,
                     (py::arg("No feat x descriptor length numpy array"),
                      "Creates the vocabulary from features"))
                .def("saveToFile", &Vocabulary::saveToFile,
                     (py::arg("Filename"),
                      "Save vocabulary to file"))
                .def("readFromFile", &Vocabulary::readFromFile,
                     (py::arg("Filename"),
                      "Read vocabulary from file"));
        }

    } // namespace fs
} // namespace python

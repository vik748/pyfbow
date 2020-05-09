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

    /* DBOW3
     *
     Vocabulary::Vocabulary (int k, int L, WeightingType weighting, ScoringType scoring)
     : m_k(k), m_L(L), m_weighting(weighting), m_scoring(scoring),
     m_scoring_object(NULL) {
     createScoringObject();}


    Vocabulary(int k = 10, int L = 5, DBoW3::WeightingType weighting = DBoW3::TF_IDF, DBoW3::ScoringType scoring = DBoW3::L1_NORM, const std::string& path = std::string()) {
        vocabulary = new DBoW3::Vocabulary(k, L, weighting, scoring);
        if (!path.empty())
            load(path);
    }


    * fbow

    Vocabulary(): _data((char*)nullptr,&AlignedFree){}
    Vocabulary(Vocabulary&&) = default;
    *
    *   fbow::VocabularyCreator::Params params;
        params.k = stoi(cml("-k","10"));
        params.L = stoi(cml("-l","6"));
        params.nthreads=stoi(cml("-t","1"));
        params.maxIters=std::stoi (cml("-maxIters","0"));
        params.verbose=cml["-v"];
        srand(0);
        fbow::VocabularyCreator voc_creator;
        fbow::Vocabulary voc;
        cout << "Creating a " << params.k << "^" << params.L << " vocabulary..." << endl;
        auto t_start=std::chrono::high_resolution_clock::now();
        voc_creator.create(voc,features,desc_name, params);
        auto t_end=std::chrono::high_resolution_clock::now();

    */

/*
 *  example execution:
    Vocabulary(std::vector<cv::Mat> features){
        fbow::VocabularyCreator::Params params;
        params.k = 10;
        params.L = 6;
        params.nthreads = 1;
        params.maxIters = 0;
        params.verbose = true;

        fbow::VocabularyCreator voc_creator;
        fbow::Vocabulary voc;
        std::cout << "Creating a " << params.k << "^" << params.L << " vocabulary..." << std::endl;
        voc_creator.create(voc, features, std::string("NA"), params);
    }
    */

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

    /*

    void clear() {
        vocabulary->clear();
    }


    void load(const std::string& path) {
        vocabulary->load(path);
    }

    void save(const std::string& path, bool binary_compressed = true) {
        vocabulary->save(path, binary_compressed);
    }

    fbow::BowVector transform(const  std::vector<cv::Mat> & features) {
        fbow::BowVector word;
        vocabulary->transform(features, word);
        return word;
    }

    double score(const  fbow::BowVector &A, const fbow::BowVector &B) {
        return vocabulary->score(A, B);
    }
    */

    fbow::Vocabulary * voc;
    fbow::VocabularyCreator * voc_creator;
    fbow::VocabularyCreator::Params voc_creator_params;
};

/*
class Database
{
public:
    Database(const std::string& path = std::string()) {
        if (path.empty())
            database = new fbow::Database();
        else
            database = new fbow::Database(path);
    }
    ~Database() {
        delete database;
    }

    void setVocabulary(const Vocabulary& vocabulary, bool use_di, int di_levels=0) {
        database->setVocabulary(*vocabulary.vocabulary, use_di, di_levels);
    }

    unsigned int add(const  cv::Mat & features) {
        return database->add(features, NULL, NULL);
    }

    std::vector<fbow::Result> query(const  cv::Mat &features, int max_results = 1, int max_id = -1) {
        fbow::QueryResults results;
        database->query(features, results, max_results, max_id);
        return results;
    }

    void save(const std::string &filename) const {
        database->save(filename);
    }

    void load(const std::string &filename) {
        database->load(filename);
    }

    void loadVocabulary(const std::string &filename, bool use_di, int di_levels=0) {
        fbow::Vocabulary voc;
        voc.load(filename);
        database->setVocabulary(voc, use_di, di_levels);
    }


private:
    fbow::Database * database;
};
*/

// Wrap a few functions and classes for testing purposes
namespace fs {
    namespace python {

        BOOST_PYTHON_MODULE(pyfbow)
        {
            // Main types export
            fs::python::init_and_export_converters();
            py::scope scope = py::scope();


            /*
            py::enum_<fbow::WeightingType>("WeightingType")
                .value("TF_IDF", fbow::TF_IDF)
                .value("TF", fbow::TF)
                .value("IDF", fbow::IDF)
                .value("BINARY", fbow::BINARY);

            py::enum_<fbow::ScoringType>("ScoringType")
                .value("L1_NORM", fbow::L1_NORM)
                .value("L2_NORM", fbow::L2_NORM)
                .value("CHI_SQUARE", fbow::CHI_SQUARE)
                .value("KL", fbow::KL)
                .value("BHATTACHARYYA", fbow::BHATTACHARYYA)
                .value("DOT_PRODUCT", fbow::DOT_PRODUCT);

            py::class_<Vocabulary>("Vocabulary")
                .def(py::init<py::optional<int, int, DBoW3::WeightingType, DBoW3::ScoringType, std::string> >(
                (py::arg("k") = 10, py::arg("L") = 5, py::arg("weighting") = DBoW3::TF_IDF, py::arg("scoring") = DBoW3::L1_NORM,
                    py::arg("path") = std::string())))
            */

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

/*          py::class_<Database>("Database")
                .def(py::init<py::optional<std::string> >(py::arg("path") = std::string()))
                .def("setVocabulary", &Database::setVocabulary)
                .def("save", &Database::save)
                .def("load", &Database::load)
                .def("loadVocabulary", &Database::loadVocabulary)
                .def("add", &Database::add)
                .def("query", &Database::query, py::return_value_policy<py::return_by_value>());


            py::class_<fbow::Result>("Result")
                .def_readonly("Id", &fbow::Result::Id)
                .def_readonly("Score", &fbow::Result::Score)
                .def_readonly("nWords", &fbow::Result::nWords)
                .def_readonly("bhatScore", &fbow::Result::bhatScore)
                .def_readonly("chiScore", &fbow::Result::chiScore)
                .def_readonly("sumCommonVi", &fbow::Result::sumCommonVi)
                .def_readonly("sumCommonWi", &fbow::Result::sumCommonWi)
                .def_readonly("expectedChiScore", &fbow::Result::expectedChiScore);*/
        }

    } // namespace fs
} // namespace python

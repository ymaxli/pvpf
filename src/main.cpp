#include <iostream>
//#include <tbb/tbb.h>
//#include <opencv/cv.h>
//#include <rapidjson/rapidjson.h>
#include <boost/any.hpp>
//#include <CUDA/cuda.h>
#include <pvpf/data_io/buffer.h>

using namespace std;
using namespace pvpf;

int main() {
    std::cout << __VERSION__ << std::endl;
    data_io::Buffer b;
    b.write(3);
    std::cout << b.read() << std::endl;
    return 0;
}

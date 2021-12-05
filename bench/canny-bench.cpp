#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include "doctest/doctest.h"
#include <nanobench.h>
#include <PCanny/PCanny.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


namespace {


void gen(std::string const& typeName, char const* mustacheTemplate,

         ankerl::nanobench::Bench const& bench) {


    std::ofstream templateOut("mustache.template." + typeName);

    templateOut << mustacheTemplate;


    std::ofstream renderOut("mustache.render." + typeName);

    ankerl::nanobench::render(mustacheTemplate, bench, renderOut);

}


} // namespace

TEST_CASE("Test Individual Functions")
{
    std::string img_path = "../fixtures/bird.png";
    int low_threshold = 30;
    int high_threshold = 100;
    // Load Color Image
    int x,y,n;
    unsigned char *color_image_data = stbi_load(img_path.c_str(), &x, &y, &n, 0);
    size_t img_size = x * y * n;

    // Convert Grey Image
    unsigned char *gray_image_data = static_cast<unsigned char *>(calloc(x * y, sizeof(unsigned char)));
    size_t gray_img_size = x * y;
    for(unsigned char *p = color_image_data, *pg = gray_image_data; p != color_image_data + img_size; p += 3, pg += 1) {
         *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    }

    // setup Canny Edge Detector
    unsigned char *image_edges = static_cast<unsigned char *>(calloc(x * y, sizeof(unsigned char)));
    PCanny::PCanny::NoiseFilter filter = PCanny::PCanny::NoiseFilter::Gaus3x3;
    PCanny::PCanny canny(x, y);
    // // Call Function


    SUBCASE("All") {
        ankerl::nanobench::Bench bench;
        using namespace std::chrono_literals;
        bench.timeUnit(1ms, "ms");
        // bench.minEpochIterations(10);
        bench.run("Canny, All", [&]() {
            canny.edges(image_edges, gray_image_data, filter, low_threshold,
                        high_threshold);
        });
        gen("html", ankerl::nanobench::templates::htmlBoxplot(), bench);
    }

}

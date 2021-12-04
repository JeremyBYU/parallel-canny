#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

#include "PCanny/PCanny.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


void usage(char* s) {
    fprintf(stderr, "\n");
    fprintf(stderr, "canny edge detection. build: %s-%s \n", __DATE__,
            __TIME__);
    fprintf(stderr,
            "%s -s <source file> [-l <low threshold>] [-h <high threshold>] "
            "[-? help]",
            s);
    fprintf(stderr, "   s: path image file\n");
    fprintf(stderr, "   l: canny low threshold\n");
    fprintf(stderr, "   h: canny high threshold\n");
    fprintf(stderr,
            "\nexample:  ./canny -s img/Valve_original.PNG "
            "-l 30 -h 90\n");
    fprintf(stderr, "\n");
}


int main(int argc, char** argv) {
    int c;
    
    std::string img_path;
    std::string output_image_path;
    int low_threshold = 30;
    int high_threshold = 100;
    while (((c = getopt(argc, argv, "s:l:h:?"))) != -1) {
        switch (c) {
            case 's':
                img_path = optarg;
                break;
            case 'l':
                low_threshold = atoi(optarg);
                break;
            case 'h':
                high_threshold = atoi(optarg);
                break;
            case '?':
            default:
                usage(argv[0]);
                return -1;
        }
    }

    if (img_path.empty()) {
        usage(argv[0]);
        return -1;
    }

    output_image_path = img_path.substr(0, img_path.length() - 4) + "_edges.png";
    int x,y,n;
    unsigned char *data = stbi_load(img_path.c_str(), &x, &y, &n, 0);
    std::cout << "Size: (" << x << "x" << y << "x" << n << ")" << std::endl;

    // int w = img_gray.cols;
    // int h = img_ori.rows;

    // PCanny::Canny::NoiseFilter filter = keymolen::Canny::NoiseFilter::Gaus3x3;

    // while (1) {
    //     cv::Mat img_edge(h, w, CV_8UC1, cv::Scalar::all(0));

    //     PCanny::Canny canny(w, h);
    //     canny.edges(img_edge.data, img_gray.data, filter, low_threshold,
    //                 high_threshold);

    unsigned char *image_edges = static_cast<unsigned char *>(calloc(x * y, sizeof(unsigned char)));

    stbi_write_png(output_image_path.c_str(), x, y, 1, image_edges, 1);
    
    stbi_image_free(data);

    return 0;
}
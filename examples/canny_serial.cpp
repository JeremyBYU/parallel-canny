#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>
#include <bits/stdc++.h>

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

template<class T>
unsigned char* convert(T *image, int m, int n, double scale=1.0)
{
    unsigned char *new_data = static_cast<unsigned char *>(calloc(m * n, sizeof(unsigned char)));

    // normalize by the max value
    if (scale == 0.0)
    {
        auto value = std::max_element(image, image + m * n);
        scale = *value;
    }
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j)
        {
            int pos = j + (i * n);
            new_data[pos] = static_cast<uint8_t>((image[pos] / scale) * 255.0 ) ;
        }
    }
    return new_data;
}


void transform(std::string img_path, int low_threshold, int high_threshold)
{

    std::string output_image_path;
    // Load Color Image
    int x,y,n;
    unsigned char *color_image_data = stbi_load(img_path.c_str(), &x, &y, &n, 0);
    size_t img_size = x * y * n;
    std::cout << "Size: (" << x << "x" << y << "x" << n << ")" << std::endl;

    // Convert Grey Image
    unsigned char *gray_image_data = static_cast<unsigned char *>(calloc(x * y, sizeof(unsigned char)));
    size_t gray_img_size = x * y;
    for(unsigned char *p = color_image_data, *pg = gray_image_data; p != color_image_data + img_size; p += 3, pg += 1) {
         *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    }
    // Save gray image
    output_image_path = img_path.substr(0, img_path.length() - 4) + "_gray.png";
    stbi_write_png(output_image_path.c_str(), x, y, 1, gray_image_data, x * 1);

    // Setup Canny Edge Detector
    unsigned char *image_edges = static_cast<unsigned char *>(calloc(x * y, sizeof(unsigned char)));
    PCanny::Canny::NoiseFilter filter = PCanny::Canny::NoiseFilter::Gaus3x3;
    PCanny::Canny canny(x, y);
    // Call Function

    // Write Gaussian Edges
    canny.GaussianFilter(image_edges, gray_image_data, filter);
    output_image_path = img_path.substr(0, img_path.length() - 4) + "_gaussian.png";
    stbi_write_png(output_image_path.c_str(), x, y, 1, image_edges, x * 1);
    
    // Write Sobel Edges
    canny.SobelFilter(image_edges);
    auto converted_sobel =convert(canny.G_, y, x, 0.0);
    output_image_path = img_path.substr(0, img_path.length() - 4) + "_sobel.png";
    stbi_write_png(output_image_path.c_str(), x, y, 1, converted_sobel, x * 1);

    // Write Segmented Edges
    auto converted_segment =convert(canny.s_, y, x, 4.0);
    output_image_path = img_path.substr(0, img_path.length() - 4) + "_angle.png";
    stbi_write_png(output_image_path.c_str(), x, y, 1, converted_segment, x * 1);


    // Write Local Max Edges
    canny.LocalMaxima();
    auto converted_maxima =convert(canny.M_, y, x, 0.0);
    output_image_path = img_path.substr(0, img_path.length() - 4) + "_maxima.png";
    stbi_write_png(output_image_path.c_str(), x, y, 1, converted_maxima, x * 1);

    // Write Canny Edges
    canny.CannyEdges(image_edges, low_threshold,high_threshold);
    output_image_path = img_path.substr(0, img_path.length() - 4) + "_edges.png";
    stbi_write_png(output_image_path.c_str(), x, y, 1, image_edges, x * 1);


    stbi_image_free(color_image_data);
    // should free converted_sobel....

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

    transform(img_path, low_threshold, high_threshold);

    return 0;
}
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>


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


    return 0;
}
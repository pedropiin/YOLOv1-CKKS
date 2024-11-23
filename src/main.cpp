#include <iostream>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define IMG_W 221
#define IMG_H 221

void read_image(const char *filename) {
    int img_width = IMG_W;
    int img_height = IMG_H;
    int img_channels = 3;

    unsigned char *img_data = stbi_load(filename, &img_width, &img_height, &img_channels, 0);
}

int main(int argc, char *argv[]) {
    
    std::string img_path = "../dataset/test221.jpg";
    read_image(img_path.c_str());

    return 0;
}


#include <iostream>
#include <vector>
#include <string>

// Including OpenGL functions to read images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_FAILURE_USERMSG
#define STBI_ONLY_JPEG

#include "openfhe/pke/openfhe.h"

#define IMG_W 221
#define IMG_H 221

std::vector<double> read_image(const char *filename) {
    int img_width = IMG_W;
    int img_height = IMG_H;
    int img_channels = 3;

    unsigned char *img_data = stbi_load(filename, &img_width, &img_height, &img_channels, 0);

    if (!img_data) {
        std::cerr << "Could not read image '" << filename << "'." << std::endl;
        stbi_failure_reason();
        return std::vector<double>();
    }

    std::cout << "the number of img_channels is " << img_channels << std::endl;

    int img_size = img_width * img_height;
    std::vector<double> img_values(img_size * img_channels);
    
    for (int i = 0; i < img_size; i++) {
        // Copying 'R' values
        img_values[i] = static_cast<double>(img_data[3 * i]) / 255.0f;
    }
    
    for (int i = 0; i < img_size; i++) {
        // Copying 'G' values
        img_values[img_size + i] = static_cast<double>(img_data[(3 * i) + 1]) / 255.0f;
    }
    
    for (int i = 0; i < img_size; i++) {
        // Copying 'B' values
        img_values[(2 * img_size) + i] = static_cast<double>(img_data[(3 * i) + 2]) / 255.0f;
    }

    stbi_image_free(img_data);

    return img_values;
}

int main(int argc, char *argv[]) {
    
    std::string img_path = "../dataset/test221.jpg";
    std::vector<double> img_data = read_image(img_path.c_str());

    return 0;
}


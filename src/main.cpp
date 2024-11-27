#include <iostream>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_FAILURE_USERMSG
#define STBI_ONLY_JPEG

#include "openfhe.h"
// #include "ciphertext-ser.h"
// #include "scheme/ckksrns/ckksrns-ser.h"
// #include "cryptocontext-ser.h"
// #include "key/key-ser.h"

#define IMG_W 221
#define IMG_H 221

using namespace lbcrypto;

uint32_t batch_size;
std::vector<uint32_t> level_budget;

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

void config_params(CCParams<CryptoContextCKKSRNS> &parameters) {
    uint32_t scale_mod_size = 59;   // recommended value by OpenFHE
    parameters.SetScalingModSize(scale_mod_size);

    uint32_t first_mod_size = 60;   // recommended value by OpenFHE
    parameters.SetFirstModSize(first_mod_size);

    batch_size = 1 << 15;
    parameters.SetBatchSize(batch_size);

    ScalingTechnique scale_tech = FLEXIBLEAUTO;
    parameters.SetScalingTechnique(scale_tech);

    uint32_t num_large_dig = 3;
    parameters.SetNumLargeDigits(num_large_dig);

    SecretKeyDist secret_key_dist = UNIFORM_TERNARY;
    parameters.SetSecretKeyDist(secret_key_dist);

    uint32_t ring_dim = 1 << 16;
    parameters.SetRingDim(ring_dim);

    SecurityLevel sec_level = HEStd_NotSet;
    parameters.SetSecurityLevel(sec_level);

    level_budget = {5, 5};
    uint32_t levels_after_bootstrap = 10;
    uint32_t depth = levels_after_bootstrap + FHECKKSRNS::GetBootstrapDepth(level_budget, secret_key_dist);
    parameters.SetMultiplicativeDepth(depth);
}

int main(int argc, char *argv[]) {
    
    std::string img_path = "../dataset/test104.jpg";
    std::vector<double> img_data = read_image(img_path.c_str());
    std::cout << "--- Image reading done ---" << std::endl;

    // --- SETTING PARAMETERS ---
    CCParams<CryptoContextCKKSRNS> parameters;
    config_params(parameters);
    std::cout << "--- Parameter configuration done ---" << std::endl;

    // --- GENERATING CONTEXT ---
    CryptoContext<DCRTPoly> context = GenCryptoContext(parameters);
    std::cout << "--- Context generation done ---" << std::endl;

    // --- ENABLING FEATURES ---
    context->Enable(PKE);
    context->Enable(KEYSWITCH);
    context->Enable(LEVELEDSHE);
    context->Enable(ADVANCEDSHE);
    context->Enable(FHE);
    std::cout << "--- Enabled features ---" << std::endl;

    // --- GENERATING KEYS ---
    KeyPair<DCRTPoly> key_pair = context->KeyGen();
    context->EvalMultKeyGen(key_pair.secretKey);
    std::cout << "--- Key generation for Multiplication done ---" << std::endl;

    context->EvalBootstrapSetup(level_budget, {0, 0}, batch_size);
    std::cout << "--- Bootstrap setup done ---" << std::endl;
    context->EvalBootstrapKeyGen(key_pair.secretKey, batch_size);
    std::cout << "--- Key generation for Boostrapping done ---" << std::endl;

    std::cout << "--- Using Ring Dimension = " << context->GetRingDimension() << " ---" << std::endl;

    return 0;
}


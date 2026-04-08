#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <cuda_runtime.h>
#include <npp.h>
#include <nppi.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " <input.png> <output_base.png> <angle_deg> <num_rotations>\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputBase = argv[2];
    float angleDeg = std::stof(argv[3]);
    int numRotations = std::stoi(argv[4]);

    // Load image
    int width, height, channels;
    unsigned char* h_image = stbi_load(inputPath.c_str(), &width, &height, &channels, 3);
    if (!h_image) {
        std::cerr << "Error: Could not load image " << inputPath << std::endl;
        return 1;
    }

    std::cout << "✅ Loaded " << width << "x" << height << " RGB image (" << inputPath << ")" << std::endl;

    // Allocate GPU memory with pitch
    unsigned char *d_image = nullptr, *d_output = nullptr;
    size_t pitch;
    cudaMallocPitch(&d_image, &pitch, width * 3, height);
    cudaMallocPitch(&d_output, &pitch, width * 3, height);
    cudaMemcpy2D(d_image, pitch, h_image, width * 3, width * 3, height, cudaMemcpyHostToDevice);

    NppiSize oSizeROI = {width, height};
    NppiRect oROI = {0, 0, width, height};
    double angleRad = angleDeg * M_PI / 180.0;

    std::cout << "✅ Processing " << numRotations << " rotations on GPU using NPP..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < numRotations; i++) {
        // Corrected NPP Rotate call
        nppiRotate_8u_C3R(d_image, oSizeROI, (int)pitch, oROI,
                          d_output, (int)pitch, oROI,
                          angleRad, 0.0, 0.0, NPPI_INTER_LINEAR);

        // Save each rotated image with unique name
        std::string outFile = outputBase;
        size_t dotPos = outFile.find_last_of('.');
        if (dotPos != std::string::npos) {
            outFile.insert(dotPos, "_" + std::to_string(i+1));
        } else {
            outFile += "_" + std::to_string(i+1);
        }
        outFile += ".png";

        unsigned char* h_output = new unsigned char[width * height * 3];
        cudaMemcpy2D(h_output, width * 3, d_output, pitch, width * 3, height, cudaMemcpyDeviceToHost);
        stbi_write_png(outFile.c_str(), width, height, 3, h_output, width * 3);
        delete[] h_output;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "✅ Finished " << numRotations << " GPU rotations in " << (int)ms << " ms" << std::endl;
    std::cout << "✅ Successfully saved all " << numRotations << " rotated images!" << std::endl;

    // Cleanup
    stbi_image_free(h_image);
    cudaFree(d_image);
    cudaFree(d_output);

    return 0;
}

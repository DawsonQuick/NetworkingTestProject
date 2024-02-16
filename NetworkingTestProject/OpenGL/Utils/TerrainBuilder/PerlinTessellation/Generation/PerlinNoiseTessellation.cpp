#include "PerlinNoiseTessellation.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include <numeric>
#include <stdlib.h>     /* srand, rand */
#include <thread>


vector22 PerlinNoiseTessellation::randomGradient(int ix, int iy) {
    // Use the seed to initialize the random number generation
    unsigned int combinedSeed = m_Seed + ix + iy;

    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix + combinedSeed, b = iy + combinedSeed;
    a *= 3284157443;

    b ^= a << s | a >> w - s;
    b *= 1911520717;

    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    vector22 v;
    v.x = cos(random);
    v.y = sin(random);

    return v;
}

// Computes the dot product of the distance and gradient vectors.
float PerlinNoiseTessellation::dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vector22 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

float PerlinNoiseTessellation::interpolate(float a0, float a1, float w) {
    // Using standard cubic interpolation formula
    float t = (1.0 - cos(w * 3.14159265)) * 0.5;
    return a0 * (1.0 - t) + a1 * t;
}



float PerlinNoiseTessellation::fade(float t) {
    // This is a simple cubic fade function to smooth out transitions
    return t * t * t * (t * (t * 6 - 15) + 10);
}
// Sample Perlin noise at coordinates x, y
float PerlinNoiseTessellation::perlin(float x, float y , float frequency, float amplitude) {
    float xFrequency = x * frequency;
    float yFrequency = y * frequency;

    // Determine grid cell corner coordinates
    int x0 = (int)(xFrequency);
    int y0 = (int)(yFrequency);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float fractionalX = xFrequency - x0;
    float fractionalY = yFrequency - y0;

    float sx = fade(fractionalX);
    float sy = fade(fractionalY);

    // Compute and interpolate top two corners
    float n0 = dotGridGradient(x0, y0, xFrequency, yFrequency);
    float n1 = dotGridGradient(x1, y0, xFrequency, yFrequency);

    float ix0 = interpolate(n0, n1, sx);
    

    // Compute and interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, xFrequency, yFrequency);
    n1 = dotGridGradient(x1, y1, xFrequency, yFrequency);
    float ix1 = interpolate(n0, n1, sx);

    // Final step: interpolate between the two previously interpolated values, now in y
    float value = interpolate(ix0, ix1, sy);

    return value * amplitude;
}



PerlinNoiseTessellation::PerlinNoiseTessellation(int m_rez, int width, int height, float* vertices, float* heightMap, float* derivativeMap, unsigned int seed)
:m_Seed(seed)
{

    // Define the number of threads
    const int num_threads = 2; // Adjust as needed

    // Calculate the range of iterations each thread will process
    int iterations_per_thread = m_rez / num_threads;
    std::thread VerticiesThread([this,iterations_per_thread, m_rez, width, height, vertices]() {
        auto start_time = std::chrono::high_resolution_clock::now();
    // Create threads
    std::vector<std::thread> threads3;
  
    for (int threadNum = 0; threadNum < num_threads; ++threadNum) {
        int start_iteration = threadNum * iterations_per_thread;
        int end_iteration = (threadNum == num_threads - 1) ? m_rez : (threadNum + 1) * iterations_per_thread;

        threads3.emplace_back([this,m_rez,width,height, start_iteration, end_iteration, vertices]() {
            for (int i = start_iteration; i < end_iteration; ++i) {
                for (int j = 0; j < m_rez; ++j) {
                    float xCoord = width * i / static_cast<float>(m_rez);
                    float yCoord = height * j / static_cast<float>(m_rez);

                    // Calculate the array index
                    int index = (i * m_rez + j) * 20;

                    //Generates the 4 corners used for the patch for tessellation
                    vertices[index++] = xCoord; // v.x
                    vertices[index++] = 0.0; // v.y
                    vertices[index++] = yCoord; // v.z
                    vertices[index++] = i / static_cast<float>(m_rez); // u
                    vertices[index++] = j / static_cast<float>(m_rez); // v

                    vertices[index++] = xCoord + width / static_cast<float>(m_rez); // v.x
                    vertices[index++] = 0.0; // v.y
                    vertices[index++] = yCoord; // v.z
                    vertices[index++] = (i + 1) / static_cast<float>(m_rez); // u
                    vertices[index++] = j / static_cast<float>(m_rez); // v

                    vertices[index++] = xCoord; // v.x
                    vertices[index++] = 0.0; // v.y
                    vertices[index++] = yCoord + height / static_cast<float>(m_rez); // v.z
                    vertices[index++] = i / static_cast<float>(m_rez); // u
                    vertices[index++] = (j + 1) / static_cast<float>(m_rez); // v

                    vertices[index++] = xCoord + width / static_cast<float>(m_rez); // v.x
                    vertices[index++] = 0.0; // v.y
                    vertices[index++] = yCoord + height / static_cast<float>(m_rez); // v.z
                    vertices[index++] = (i + 1) / static_cast<float>(m_rez); // u
                    vertices[index++] = (j + 1) / static_cast<float>(m_rez); // v
                }
            }
            });
    }

    // Wait for threads to finish
    for (auto& thread : threads3) {
        thread.join();
    }



    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution VertiiesThread time: " << duration_ms.count() << " milliseconds" << std::endl;

    });

    std::thread HeightMapThread([this, width, height, heightMap, derivativeMap]() {
        auto start_time = std::chrono::high_resolution_clock::now();


        // Calculate the range of rows each thread will process
        int rows_per_thread = height / num_threads;
        auto heightMapStart_time = std::chrono::high_resolution_clock::now();
        // Create threads
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            int start_row = i * rows_per_thread;
            int end_row = (i == num_threads - 1) ? height : (i + 1) * rows_per_thread;

            threads.emplace_back([this, width, start_row, end_row, heightMap]() {
                int index = start_row * width;
                for (int y = start_row; y < end_row; ++y) {
                    for (int x = 0; x < width; ++x) {
                        //TODO: Possibly convert to run in parallel (SIMD)
                        float perlinValue = perlin(static_cast<float>(x), static_cast<float>(y), 0.0003, 1000);
                            perlinValue += perlin(static_cast<float>(x), static_cast<float>(y), 0.003, 250);
                            perlinValue += perlin(static_cast<float>(x), static_cast<float>(y), 0.009, 50);
                            perlinValue += perlin(static_cast<float>(x), static_cast<float>(y), 0.01, 15);
                            perlinValue += perlin(static_cast<float>(x), static_cast<float>(y), 0.03, 7);

                        heightMap[y * width + x] = perlinValue;
                    }
                }
                });
        }

        // Wait for threads to finish
        for (auto& thread : threads) {
            thread.join();
        }

        auto heightMapEnd_time = std::chrono::high_resolution_clock::now();
        auto heightMapduration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(heightMapEnd_time - heightMapStart_time);
        std::cout << "Execution HeightMap Generation time: " << heightMapduration_ms.count() << " milliseconds" << std::endl;


        auto DerivativeStart_time = std::chrono::high_resolution_clock::now();
        // Create threads
        std::vector<std::thread> threads2;
        for (int i = 0; i < num_threads; ++i) {
            int start_row = i * rows_per_thread;
            int end_row = (i == num_threads - 1) ? height : (i + 1) * rows_per_thread;

            threads2.emplace_back([this, width, height, start_row, end_row, heightMap, derivativeMap]() {
                int index = start_row * width;
                for (int y = start_row; y < end_row; ++y) {
                    for (int x = 0; x < width; ++x) {

                        float top, bottom, left, right;
                        float indexValue = heightMap[index];
                        if (y == 0) {
                            top =  heightMap[index];
                        }
                        else {
                            top =  heightMap[index - width];
                        }

                        if (y == (height - 1)) {
                            bottom =  heightMap[index];
                        }
                        else {
                            bottom =  heightMap[index + width]; 
                        }

                        if (x == 0) {
                            left =  heightMap[index];
                        }
                        else {
                            left =  heightMap[index - 1];
                        }

                        if (x == (width - 1)) {
                            right =  heightMap[index];
                        }
                        else {
                            right =  heightMap[index + 1];
                        }
                        // Compute derivatives/changes
                        float dx = right - left;
                        float dy = bottom - top;

                        
                        float derivative = 0.5 * (((dy + dx) / 2) * ((dy + dx) / 2));
                        derivativeMap[index++] = (derivative);
                        
                    }
                }
                });
        }
        // Wait for threads to finish
        for (auto& thread : threads2) {
            thread.join();
        }
        auto DerivativeEnd_time = std::chrono::high_resolution_clock::now();
        auto derivativeduration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(DerivativeEnd_time - DerivativeStart_time);
        std::cout << "Execution HeightMap Generation time: " << derivativeduration_ms.count() << " milliseconds" << std::endl;

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Execution HeightMapThread time: " << duration_ms.count() << " milliseconds" << std::endl;

        });

    VerticiesThread.join();
    HeightMapThread.join();
}


PerlinNoiseTessellation::~PerlinNoiseTessellation() {

}
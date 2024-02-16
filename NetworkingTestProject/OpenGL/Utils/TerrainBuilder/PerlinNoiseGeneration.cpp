#include "PerlinNoiseGeneration.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include <numeric>
#include <stdlib.h>     /* srand, rand */



vector2 PerlinNoiseGeneration::randomGradient(int ix, int iy) {
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
    vector2 v;
    v.x = cos(random);
    v.y = sin(random);

    return v;
}

// Computes the dot product of the distance and gradient vectors.
float PerlinNoiseGeneration::dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

float PerlinNoiseGeneration::interpolate(float a0, float a1, float w) {
    // Using standard cubic interpolation formula
    float t = (1.0 - cos(w * 3.14159265)) * 0.5;
    return a0 * (1.0 - t) + a1 * t;
}

float interpolateSmoothstep(float a0, float a1, float t) {
    t = t * t * (3.0 - 2.0 * t);
    return (1.0 - t) * a0 + t * a1;
}


float PerlinNoiseGeneration::fade(float t) {
    // This is a simple cubic fade function to smooth out transitions
    return t * t * t * (t * (t * 6 - 15) + 10);
}
// Sample Perlin noise at coordinates x, y
float PerlinNoiseGeneration::perlin(float x, float y , float frequency, float amplitude) {

    // Determine grid cell corner coordinates
    int x0 = (int)(x * frequency);
    int y0 = (int)(y * frequency);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float fractionalX = x * frequency - x0;
    float fractionalY = y * frequency - y0;

    float sx = fade(fractionalX);
    float sy = fade(fractionalY);

    // Compute and interpolate top two corners
    float n0 = dotGridGradient(x0, y0, x * frequency, y * frequency);
    float n1 = dotGridGradient(x1, y0, x * frequency, y * frequency);
    float ix0;
    if (frequency < 0.001){
        ix0 = interpolate(n0, n1, sx);
    }
    else {
        ix0 = interpolateSmoothstep(n0, n1, sx);
    }

    // Compute and interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, x * frequency, y * frequency);
    n1 = dotGridGradient(x1, y1, x * frequency, y * frequency);
    float ix1 = interpolate(n0, n1, sx);

    // Final step: interpolate between the two previously interpolated values, now in y
    float value;
    if (frequency < 0.001) {
        value = interpolate(ix0, ix1, sy);
    }
     else {
        value = interpolateSmoothstep(ix0, ix1, sy);
    }
    if (value * amplitude < 16) {
        return value * amplitude/2;
    }
    return value * amplitude;
}



PerlinNoiseGeneration::PerlinNoiseGeneration(int width, int height, float* verticesArray, unsigned int* indicesArray, unsigned int seed)
:m_Seed(seed)
{
    int vertexIndex = 0;
    int indexIndex = 0;
    int smoothness = 25;
    int courseStrength = 10;
    // Theoretical number of vertices and indices
    int theoreticalVertices = width * height;
    int theoreticalIndices = (width - 1) * (height - 1) * 6;

    std::cout << "Theoretical number of vertices: " << theoreticalVertices << std::endl;
    std::cout << "Theoretical number of indices: " << theoreticalIndices << std::endl;

    // Precompute weights and offsets
    std::vector<float> weights;
    std::vector<std::pair<int, int>> offsets;

    for (unsigned int i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            if (i < width - 1 && j < height - 1) {
                // Triangle 1
                indicesArray[indexIndex++] = i * height + j;
                indicesArray[indexIndex++] = (i + 1) * height + j;
                indicesArray[indexIndex++] = i * height + (j + 1);

                // Triangle 2
                indicesArray[indexIndex++] = i * height + (j + 1);
                indicesArray[indexIndex++] = (i + 1) * height + j;
                indicesArray[indexIndex++] = (i + 1) * height + (j + 1);
            }
            // Calculate normalized texture coordinates
            float normalizedS = static_cast<float>(i) / static_cast<float>(width);
            float normalizedT = static_cast<float>(j) / static_cast<float>(height);
            // Adjust the scaling factor based on your requirements
            vertexIndex = (i * height + j) * 5;
            verticesArray[vertexIndex] = i;
            verticesArray[vertexIndex + 1] += perlin(static_cast<float>(i), static_cast<float>(j), 0.0005, 500);
            verticesArray[vertexIndex + 1] += perlin(static_cast<float>(i), static_cast<float>(j), 0.005, 150);
           // verticesArray[vertexIndex + 1] += perlin(static_cast<float>(i), static_cast<float>(j), 0.06, 10);
            verticesArray[vertexIndex + 2] = j;

            verticesArray[vertexIndex + 3] = normalizedS;
            verticesArray[vertexIndex + 4] = normalizedT;

        }
    }
    
    
    std::cout << "Number of vertices: " << vertexIndex / 5 << std::endl;
    std::cout << "Number of indices: " << indexIndex << std::endl;
}


PerlinNoiseGeneration::~PerlinNoiseGeneration() {

}
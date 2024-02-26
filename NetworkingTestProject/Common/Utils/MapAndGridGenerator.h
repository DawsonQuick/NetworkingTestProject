/*---------------------------------------------------------------------------------------
*                               Code to generate grid  
 ---------------------------------------------------------------------------------------*/

void generateGrid(float maxSize, float gridSize, std::vector<float>& vertices) {
    // Calculate the number of tiles along one side of the grid
    int numTiles = static_cast<int>(maxSize / gridSize);

    // Generate horizontal lines
    for (int i = 0; i <= numTiles; ++i) {
        float y = -maxSize / 2.0f + i * gridSize;
        vertices.push_back(-maxSize / 2.0f); // x-coordinate of the start point
        vertices.push_back(y);               // y-coordinate of the start point
        vertices.push_back(0.0f);            // z-coordinate
        vertices.push_back(maxSize / 2.0f);  // x-coordinate of the end point
        vertices.push_back(y);               // y-coordinate of the end point
        vertices.push_back(0.0f);            // z-coordinate
    }

    // Generate vertical lines
    for (int i = 0; i <= numTiles; ++i) {
        float x = -maxSize / 2.0f + i * gridSize;
        vertices.push_back(x);               // x-coordinate of the start point
        vertices.push_back(-maxSize / 2.0f); // y-coordinate of the start point
        vertices.push_back(0.0f);            // z-coordinate
        vertices.push_back(x);               // x-coordinate of the end point
        vertices.push_back(maxSize / 2.0f);  // y-coordinate of the end point
        vertices.push_back(0.0f);            // z-coordinate
    }
}
//---------------------------------------------------------------------------------------


/*---------------------------------------------------------------------------------------
*                               Code to generate tiles
 ---------------------------------------------------------------------------------------*/
void generateGridQuads(float maxSize, float gridSize, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    // Calculate the number of tiles along one side of the grid
    int numTiles = static_cast<int>(maxSize / gridSize);

    // Generate quads in the middle of each grid square
    //TODO: Add points for texture mapping
    for (int i = 0; i < numTiles; ++i) {
        for (int j = 0; j < numTiles; ++j) {
            float x = -maxSize / 2.0f + j * gridSize;
            float y = -maxSize / 2.0f + i * gridSize;
            float quadSize = gridSize * 0.5f; // Half the size of each grid square
            // Define vertices for the quad
            vertices.push_back(x - quadSize); // Bottom-left
            vertices.push_back(y - quadSize);
            vertices.push_back(0.0f);
            vertices.push_back(x + quadSize); // Bottom-right
            vertices.push_back(y - quadSize);
            vertices.push_back(0.0f);
            vertices.push_back(x + quadSize); // Top-right
            vertices.push_back(y + quadSize);
            vertices.push_back(0.0f);
            vertices.push_back(x - quadSize); // Top-left
            vertices.push_back(y + quadSize);
            vertices.push_back(0.0f);
            // Define indices for the quad
            unsigned int baseIndex = static_cast<unsigned int>(vertices.size() / 3) - 4; // Index of the first vertex of the quad
            indices.push_back(baseIndex);     // Bottom-left
            indices.push_back(baseIndex + 1); // Bottom-right
            indices.push_back(baseIndex + 2); // Top-right
            indices.push_back(baseIndex);     // Bottom-left
            indices.push_back(baseIndex + 2); // Top-right
            indices.push_back(baseIndex + 3); // Top-left
        }
    }
}
//---------------------------------------------------------------------------------------
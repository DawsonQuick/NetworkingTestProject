#include "GlobalConfigurations.h"
#include <vector>
/*---------------------------------------------------------------------------------------
*                               Code to generate grid  
 ---------------------------------------------------------------------------------------*/
void generateGridQuads(float maxWidth, float maxHeight, float gridSizeWidth, float gridSizeHeight);
void generateGrid(float maxWidth, float maxHeight, float gridSizeWidth, float gridSizeHeight, std::vector<float>& vertices) {
    // Calculate the number of tiles along the width and height of the grid
    int numTilesWidth = static_cast<int>(maxWidth / gridSizeWidth);
    int numTilesHeight = static_cast<int>(maxHeight / gridSizeHeight);

    // Generate horizontal lines
    for (int i = -numTilesHeight/2; i <= numTilesHeight/2; ++i) {
        float y =  i * gridSizeHeight;
        vertices.push_back(-maxWidth / 2.0f); // x-coordinate of the start point
        vertices.push_back(y);                // y-coordinate of the start point
        vertices.push_back(0.0f);             // z-coordinate
        vertices.push_back(maxWidth / 2.0f);  // x-coordinate of the end point
        vertices.push_back(y);                // y-coordinate of the end point
        vertices.push_back(0.0f);             // z-coordinate
    }

    // Generate vertical lines
    for (int i = -numTilesWidth/2; i <= numTilesWidth/2; ++i) {
        float x = i * gridSizeWidth;
        vertices.push_back(x);                // x-coordinate of the start point
        vertices.push_back(-maxHeight / 2.0f);// y-coordinate of the start point
        vertices.push_back(0.0f);             // z-coordinate
        vertices.push_back(x);                // x-coordinate of the end point
        vertices.push_back(maxHeight / 2.0f); // y-coordinate of the end point
        vertices.push_back(0.0f);             // z-coordinate
    }

    generateGridQuads(maxWidth,maxHeight,gridSizeWidth,gridSizeHeight);
}
//---------------------------------------------------------------------------------------


/*---------------------------------------------------------------------------------------
*                               Code to generate tiles
 ---------------------------------------------------------------------------------------*/
void generateGridQuads(float maxWidth, float maxHeight, float gridSizeWidth, float gridSizeHeight) {
    std::vector<TileInfo> tileInformation;

    // Calculate the number of tiles along one side of the grid
    int numTilesWidth = static_cast<int>(maxWidth / gridSizeWidth);
    int numTilesHeight = static_cast<int>(maxHeight / gridSizeHeight);

    // Generate quads in the middle of each grid square
    for (int i = -numTilesWidth/2; i < numTilesWidth/2; ++i) {
        for (int j = -numTilesHeight/2; j < numTilesHeight/2; ++j) {
            TileInfo perTileInfo;
            float x =  i * gridSizeWidth;
            float y = j * gridSizeHeight;
            float quadSize = gridSizeWidth * 0.5f; // Half the size of each grid square


            perTileInfo.centerPos.x = x- quadSize;
            perTileInfo.centerPos.y = y - quadSize;
            perTileInfo.layoutIndex.x = i;
            perTileInfo.layoutIndex.y = j;
            // Define vertices for the quad
            perTileInfo.vertexPos[0][0] = (x - quadSize); // Bottom-left
            perTileInfo.vertexPos[0][1] = (y - quadSize);
            perTileInfo.vertexPos[0][2] = (0.0f);
            perTileInfo.vertexPos[1][0] = (x + quadSize); // Bottom-right
            perTileInfo.vertexPos[1][1] = (y - quadSize);
            perTileInfo.vertexPos[1][2] = (0.0f);
            perTileInfo.vertexPos[2][0] = (x + quadSize); // Top-right
            perTileInfo.vertexPos[2][1] = (y + quadSize);
            perTileInfo.vertexPos[2][2] = (0.0f);
            perTileInfo.vertexPos[3][0] = (x - quadSize); // Top-left
            perTileInfo.vertexPos[3][1] = (y + quadSize);
            perTileInfo.vertexPos[3][2] = (0.0f);

            perTileInfo.isTraversable = true;

            tileInformation.push_back(perTileInfo);

        }
    }
    GlobalConfigurations::getInstance().setMapTileInformation(tileInformation);

}
//---------------------------------------------------------------------------------------
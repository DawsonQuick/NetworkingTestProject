#pragma once
#include <vector>

//Variables used for multi-target spells
static std::vector<float> cachedCursorPoints;
static std::vector<glm::vec2> cachedTargetLocations;
static std::vector<float> currentCursorPos;

//Variable used to store the result of the Line-of-sight check for spells/projectiles
static bool isSpellLocatonValid = true;


/*-------------------------------------------------------------------------------------------------------------*
 *                                         Geometry functionality                                              *
 *-------------------------------------------------------------------------------------------------------------*/

void drawCircle(float centerX, float centerY, float radius, int numSegments, std::vector<float>& vertices) {

    // Calculate the angle between each segment
    float angleIncrement = 2.0f * 3.1415 / numSegments;

    // Generate vertices around the circumference
    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleIncrement;
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);
        // Add vertices to the buffer
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // Assuming z-coordinate is 0
    }

}

bool pointInsideCircle(double x, double y, double cx, double cy, double radius) {
    double distance = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2));
    return distance <= radius;
}
//TODO: Possible Change to base logic off of gridcell and not circular (There is less range for diagonal ranges doing circles)
/*-------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------*
 *                                         Geometry functionality                                              *
 *-------------------------------------------------------------------------------------------------------------*/

void drawSquare(float centerX, float centerY, float sideLength, std::vector<float>& vertices) {
    // Calculate half the side length
    float halfLength = sideLength;

    // Define the vertices of the square as line segments
    float verticesArray[] = {
        centerX - halfLength, centerY - halfLength, 0.0f, // Bottom-left
        centerX + halfLength, centerY - halfLength, 0.0f, // Bottom-right

        centerX + halfLength, centerY - halfLength, 0.0f, // Bottom-right
        centerX + halfLength, centerY + halfLength, 0.0f, // Top-right

        centerX + halfLength, centerY + halfLength, 0.0f, // Top-right
        centerX - halfLength, centerY + halfLength, 0.0f, // Top-left

        centerX - halfLength, centerY + halfLength, 0.0f, // Top-left
        centerX - halfLength, centerY - halfLength, 0.0f, // Bottom-left
    };

    // Add the vertices to the buffer
    vertices.insert(vertices.end(), std::begin(verticesArray), std::end(verticesArray));
}

bool pointInsideSquare(double x, double y, double squareX, double squareY, double sideLength) {
    // Calculate the boundaries of the square
    double leftBoundary = squareX - sideLength;
    double rightBoundary = squareX + sideLength;
    double topBoundary = squareY + sideLength;
    double bottomBoundary = squareY - sideLength;

    // Check if the point is within the boundaries
    return (x >= leftBoundary && x <= rightBoundary && y >= bottomBoundary && y <= topBoundary);
}

//TODO: If l2 lands directly on a point in l1 then intersection is missed, need to fix
glm::vec2 intersection(glm::mat2x2 l1, glm::mat2x2 l2) {
    double x1 = l1[0][0];
    double y1 = l1[0][1];
    double x2 = l1[1][0];
    double y2 = l1[1][1];
    double x3 = l2[0][0];
    double y3 = l2[0][1];
    double x4 = l2[1][0];
    double y4 = l2[1][1];

    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) {
        return glm::vec2(-1.0f,-1.0f); // lines are parallel
    }

    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

    if (0.00001 <= t && t <= 0.99999 && 0.00001 <= u && u <= 0.99999) {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        return glm::vec2(x, y );
    }
    else {
        return glm::vec2(-1.0f, -1.0f); // lines do not intersect
    }
}
/*-------------------------------------------------------------------------------------------------------------*/



/*
* When a click is pressed determine which action is currently selected, then perform the execution of the selected action
*/
void processClick(float posX, float posY) {
    if (GlobalConfigurations::getInstance().getgridPaintMode()) {
        GlobalConfigurations::getInstance().updateTileMapInformation(GlobalConfigurations::getInstance().getCurrentlyHoveredTile().layoutIndex.x,
            GlobalConfigurations::getInstance().getCurrentlyHoveredTile().layoutIndex.y);
    }
    else {



        std::string action = GlobalConfigurations::getInstance().getselectedAction();
        if (action == "Move") {
            if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {

                if (pointInsideCircle(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), ((30.0f / 5.0f) * (GlobalConfigurations::getInstance().getScale())))) {
                    PlayerDatabase::getInstance().getPlayer(playerName).setPositionX(posX);
                    PlayerDatabase::getInstance().getPlayer(playerName).setPositionY(posY);
                }

            }
            else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                if (pointInsideSquare(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), ((30.0f / 5.0f) * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)))) {
                    PlayerDatabase::getInstance().getPlayer(playerName).setPositionX(posX);
                    PlayerDatabase::getInstance().getPlayer(playerName).setPositionY(posY);
                }
            }



        }
        if (action == "Cast Spell") {
            if (isSpellLocatonValid) {


                std::shared_ptr<Spell> spell = PlayerDatabase::getInstance().getPlayer(playerName).getSelectedSpell();
                if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                    if (pointInsideCircle(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), (spell->getRange() * (GlobalConfigurations::getInstance().getScale())))) {
                        
                        //-------------------------------------------------------------------------------------------------------------------------
                        // Logic for spells with multiple target selection before cast, also works for single target spells
                        //-------------------------------------------------------------------------------------------------------------------------
                        cachedTargetLocations.push_back(glm::vec2(posX, posY));
                        cachedCursorPoints.insert(cachedCursorPoints.end(), currentCursorPos.begin(), currentCursorPos.end());
                        float tmpCount = PlayerDatabase::getInstance().getPlayer(playerName).getCurrentSpellShotCount();
                        tmpCount++;
                        PlayerDatabase::getInstance().getPlayer(playerName).setCurrentSpellShotCount(tmpCount);
                        if (spell->getMaxNumberofShots() == PlayerDatabase::getInstance().getPlayer(playerName).getCurrentSpellShotCount() - 1) {
                            for (glm::vec2 targetPosition : cachedTargetLocations) {
                                PlayerDatabase::getInstance().getPlayer(playerName).castSpell(targetPosition.x, targetPosition.y);
                            }
                            cachedCursorPoints.clear();
                            cachedTargetLocations.clear();
                            PlayerDatabase::getInstance().getPlayer(playerName).setCurrentSpellShotCount(1.0f); //reset spell shot count after completing 
                        }
                        //------------------------------------------------------------------------------------------------------------------------
                    }
                }
                else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                    if (pointInsideSquare(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), (spell->getRange() * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)))) {
                       
                        //-------------------------------------------------------------------------------------------------------------------------
                        // Logic for spells with multiple target selection before cast, also works for single target spells
                        //-------------------------------------------------------------------------------------------------------------------------
                        cachedTargetLocations.push_back(glm::vec2(posX, posY));
                        cachedCursorPoints.insert(cachedCursorPoints.end(), currentCursorPos.begin(), currentCursorPos.end());
                        float tmpCount = PlayerDatabase::getInstance().getPlayer(playerName).getCurrentSpellShotCount();
                        tmpCount++;
                        PlayerDatabase::getInstance().getPlayer(playerName).setCurrentSpellShotCount(tmpCount);
                        if (spell->getMaxNumberofShots() == PlayerDatabase::getInstance().getPlayer(playerName).getCurrentSpellShotCount() - 1) {
                            for (glm::vec2 targetPosition : cachedTargetLocations) {
                                PlayerDatabase::getInstance().getPlayer(playerName).castSpell(targetPosition.x, targetPosition.y);
                            }
                            cachedCursorPoints.clear();
                            cachedTargetLocations.clear();
                            PlayerDatabase::getInstance().getPlayer(playerName).setCurrentSpellShotCount(1.0f); //reset spell shot count after completing 
                        }
                        //------------------------------------------------------------------------------------------------------------------------
                    }
                }

            }




        }
    }
}



/*
* Function used to determine the shape of the cursor based off of the action and then to poplate the vertex array for rendering
*/
void calculateCursor(float posX,float posY,std::vector<float>& vertices) {

    //Since this is called every frame it has an almost immediate responce
    //This checks to see if the uses has switch selected spells are actions, and if so reset the stored cursors
    if (PlayerDatabase::getInstance().getPlayer(playerName).hasSpellChanged()) {
        cachedCursorPoints.clear();
        cachedTargetLocations.clear();
        PlayerDatabase::getInstance().getPlayer(playerName).setCurrentSpellShotCount(1.0f); //reset spell shot count 
        PlayerDatabase::getInstance().getPlayer(playerName).resetChangeState(false);
    }


    /*
    * Does not make the most sense to place this here, but this is getting the vertex array from the DurationSpellManager
    * Which shows the the range of all currently active duration/concentration spells and just appends them onto the cursor vertex array
    */
    std::vector<float> tmpDurationSpellVertexArray = DurationSpellManager::getInstance().getDurationSpellVertexArray();
    vertices.insert(vertices.end(), tmpDurationSpellVertexArray.begin(), tmpDurationSpellVertexArray.end());
    std::string action = GlobalConfigurations::getInstance().getselectedAction();

    if (action == "Move") {

         std::vector<float> tmp =GlobalConfigurations::getInstance().getA_Starpath();
         vertices.insert(vertices.end(), tmp.begin(), tmp.end());

        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (0.5f * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (0.5f * (GlobalConfigurations::getInstance().getScale())), vertices);
        }
      
       
    }
    if (action == "Cast Spell") {
        std::vector<float> tmp;
        tmp.push_back((float)PlayerDatabase::getInstance().getPlayer(playerName).getPositionX());
        tmp.push_back((float)PlayerDatabase::getInstance().getPlayer(playerName).getPositionY());
        tmp.push_back(0.0f);
        tmp.push_back((float)(posX));
        tmp.push_back((float)(posY));
        tmp.push_back(0.0f);
        vertices.insert(vertices.end(), tmp.begin(), tmp.end());

        /*---------------------------------------------------------------------------------------------------------------------*
         *                                     Projectile/Spell Line of Sight Check                                            *
         *---------------------------------------------------------------------------------------------------------------------*/
        float scale = GlobalConfigurations::getInstance().getScale()/2;
        glm::mat2x2 cursorLine = {
            (float)PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(),
            (float)PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(),
            (float)(posX),(float)(posY)
        };
        for (glm::mat2x2 tile : GlobalConfigurations::getInstance().getBlockedTileWalls()) {
            glm::mat2x2 tmp = { tile[0][0] - scale,tile[0][1] - scale,tile[1][0] - scale ,tile[1][1] - scale };

            if (intersection(tmp, cursorLine) != glm::vec2(-1.0f, -1.0f)) {
                isSpellLocatonValid = false;
                GlobalConfigurations::getInstance().setCursorColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));
                break;
            }
            else {
                isSpellLocatonValid = true;
            }
        }
        /*--------------------------------------------------------------------------------------------------------------------*/

        std::shared_ptr<Spell> spell = PlayerDatabase::getInstance().getPlayer(playerName).getSelectedSpell();
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (spell->getImpactRadius() * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (spell->getImpactRadius() * (GlobalConfigurations::getInstance().getScale())+ (GlobalConfigurations::getInstance().getScale() / 2.0f)), vertices);
        }
        
        
    }
    
    //Save the current cursor vector data, used for multi-target spell/projectile selection
    currentCursorPos = vertices;
    if (!cachedCursorPoints.empty()) {
        vertices.insert(vertices.end(), cachedCursorPoints.begin(), cachedCursorPoints.end());
    }
    

}

/*
* Function used to calculate the range that the selected action has, populates a vertex array to then to render to show the range
*/
void calculateRange(std::vector<float>& vertices) {
    float posX = PlayerDatabase::getInstance().getPlayer(playerName).getPositionX();
    float posY = PlayerDatabase::getInstance().getPlayer(playerName).getPositionY();
    std::string action = GlobalConfigurations::getInstance().getselectedAction();
    if (action == "Move") {
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, ((30.0f/5.0f) * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, ((30.0f/5.0f) * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() /2.0f)), vertices);
        }
       
        
    }
    if (action == "Cast Spell") {
        std::shared_ptr<Spell> spell = PlayerDatabase::getInstance().getPlayer(playerName).getSelectedSpell();
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (spell->getRange() * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (spell->getRange() * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)), vertices);
        }
        


    }
}
#pragma once
#include <vector>
static std::vector<float> cachedCursorPoints;
static std::vector<glm::vec2> cachedTargetLocations;
static std::vector<float> currentCursorPos;
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

/*-------------------------------------------------------------------------------------------------------------*/

void processClick(float posX, float posY) {
    
    std::string action = GlobalConfigurations::getInstance().getselectedAction();
    if(action == "Move") {
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
        
            if (pointInsideCircle(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), ((30.0f / 5.0f) * (GlobalConfigurations::getInstance().getScale())))) {
                PlayerDatabase::getInstance().getPlayer(playerName).setPositionX(posX);
                PlayerDatabase::getInstance().getPlayer(playerName).setPositionY(posY);
            }
        
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            if (pointInsideSquare(posX,posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), ((30.0f / 5.0f) * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)))) {
                PlayerDatabase::getInstance().getPlayer(playerName).setPositionX(posX);
                PlayerDatabase::getInstance().getPlayer(playerName).setPositionY(posY);
            }
        }



    }
    if (action == "Cast Spell") {
        std::shared_ptr<Spell> spell = GlobalConfigurations::getInstance().getSelectedSpell();
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                if (pointInsideCircle(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), (spell->getRange() * (GlobalConfigurations::getInstance().getScale())))) {
                    cachedTargetLocations.push_back(glm::vec2(posX, posY));
                    cachedCursorPoints.insert(cachedCursorPoints.end(), currentCursorPos.begin(), currentCursorPos.end());
                    float tmpCount = GlobalConfigurations::getInstance().getCurrentSpellShotCount();
                    tmpCount++;
                    GlobalConfigurations::getInstance().setCurrentSpellShotCount(tmpCount);
                    if (spell->getMaxNumberofShots() == GlobalConfigurations::getInstance().getCurrentSpellShotCount()-1) {
                        for (glm::vec2 targetPosition : cachedTargetLocations) {
                               spell->castSpell(targetPosition.x, targetPosition.y);
                        }
                        cachedCursorPoints.clear();
                        cachedTargetLocations.clear();
                        GlobalConfigurations::getInstance().setCurrentSpellShotCount(1.0f); //reset spell shot count after completing 
                    }
                }
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                if (pointInsideSquare(posX, posY, PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(), (spell->getRange() * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)))) {
                    cachedTargetLocations.push_back(glm::vec2(posX, posY));
                    cachedCursorPoints.insert(cachedCursorPoints.end(), currentCursorPos.begin(), currentCursorPos.end());
                    float tmpCount = GlobalConfigurations::getInstance().getCurrentSpellShotCount();
                    tmpCount++;
                    GlobalConfigurations::getInstance().setCurrentSpellShotCount(tmpCount);
                    if (spell->getMaxNumberofShots() == GlobalConfigurations::getInstance().getCurrentSpellShotCount() - 1) {
                        for (glm::vec2 targetPosition : cachedTargetLocations) {
                            spell->castSpell(targetPosition.x, targetPosition.y);
                        }
                        cachedCursorPoints.clear();
                        cachedTargetLocations.clear();
                        GlobalConfigurations::getInstance().setCurrentSpellShotCount(1.0f); //reset spell shot count after completing 
                    }
                }
        }
       

        


        
    }
}




void calculateCursor(float posX,float posY,std::vector<float>& vertices) {
    //Since this is called every frame, add check here to see if spell has been changed
    if (GlobalConfigurations::getInstance().hasSpellChanged()) {
        cachedCursorPoints.clear();
        cachedTargetLocations.clear();
        GlobalConfigurations::getInstance().setCurrentSpellShotCount(1.0f); //reset spell shot count 
        GlobalConfigurations::getInstance().resetChangeState(false);
    }

    std::string action = GlobalConfigurations::getInstance().getselectedAction();
    if (action == "Move") {
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (0.5f * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (0.5f * (GlobalConfigurations::getInstance().getScale())), vertices);
        }
      
       
    }
    if (action == "Cast Spell") {
        std::shared_ptr<Spell> spell = GlobalConfigurations::getInstance().getSelectedSpell();
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (spell->getImpactRadius() * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (spell->getImpactRadius() * (GlobalConfigurations::getInstance().getScale())+ (GlobalConfigurations::getInstance().getScale() / 2.0f)), vertices);
        }
        
        
    }
    currentCursorPos = vertices;
    if (!cachedCursorPoints.empty()) {
        vertices.insert(vertices.end(), cachedCursorPoints.begin(), cachedCursorPoints.end());
    }
    

}

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
        std::shared_ptr<Spell> spell = GlobalConfigurations::getInstance().getSelectedSpell();
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (spell->getRange() * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (spell->getRange() * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)), vertices);
        }
        


    }
}
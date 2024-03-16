#pragma once
#ifndef INPUTPROCESS_H
#define INPUTPROCESS_H
#include <vector>
#include "./../../Common/Messages/Utils/MessageFactory.h"
#include "./../../Common/Utils/TestGlobalClientInstance.h"
//Variables used for multi-target spells
static std::vector<float> cachedCursorPoints;
static std::vector<glm::vec2> cachedTargetLocations;
static std::vector<float> currentCursorPos;

//Variable used to store the result of the Line-of-sight check for spells/projectiles
static bool isSpellLocatonValid = true;
static bool isMoveLocationValid = true;


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

bool pointOnLine(float x1, float y1, float x2, float y2, float x3, float y3) {
    // Check if the slopes of the line segments (x1, y1)-(x2, y2) and (x1, y1)-(x3, y3) are equal
    float slope1 = (y2 - y1) / (x2 - x1);
    float slope2 = (y3 - y1) / (x3 - x1);

    // Check if the slopes are approximately equal (considering floating-point arithmetic)
    if (fabs(slope1 - slope2) < 0.00001) {
        return true; // The point lies on the line
    }
    else {
        return false; // The point does not lie on the line
    }
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

    if (pointOnLine(x3, y3, x4, y4, x1, y1) || pointOnLine(x3, y3, x4, y4, x2, y2)) {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        return glm::vec2(x, y);
    }

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
            if(isMoveLocationValid){
            if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {

                if (pointInsideCircle(posX, posY, PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX(), PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY(), ((30.0f / 5.0f) * (GlobalConfigurations::getInstance().getScale())))) {
                    PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setPositionX(posX);
                    PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setPositionY(posY);

                    MessageFactory factory;
                    std::stringstream ss;
                    ss << posX << '|' << posY << '|' << 0.0f << '|';
                    TestGlobalClientInstance::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, getCurrentTimeInSeconds(),
                        PlayerFields::POSITION, GlobalConfigurations::getInstance().getPlayerName(), std::move(ss)
                    ).serialize().c_str());

                }

            }
            else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                if (pointInsideSquare(posX, posY, PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX(), PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY(), ((30.0f / 5.0f) * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)))) {
                    PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setPositionX(posX);
                    PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setPositionY(posY);
                    MessageFactory factory;
                    std::stringstream ss;
                    ss << posX << '|' << posY << '|' << 0.0f << '|';
                    TestGlobalClientInstance::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, getCurrentTimeInSeconds(),
                        PlayerFields::POSITION, GlobalConfigurations::getInstance().getPlayerName(), std::move(ss)
                    ).serialize().c_str());
                }
            }
        }


        }
        if (action == "Cast Spell") {
            if (isSpellLocatonValid) {


                std::shared_ptr<Spell> spell = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getSelectedSpell();
                if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                    if (pointInsideCircle(posX, posY, PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX(), PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY(), (spell->getRange() * (GlobalConfigurations::getInstance().getScale())))) {
                        
                        //-------------------------------------------------------------------------------------------------------------------------
                        // Logic for spells with multiple target selection before cast, also works for single target spells
                        //-------------------------------------------------------------------------------------------------------------------------
                        cachedTargetLocations.push_back(glm::vec2(posX, posY));
                        cachedCursorPoints.insert(cachedCursorPoints.end(), currentCursorPos.begin(), currentCursorPos.end());
                        float tmpCount = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getCurrentSpellShotCount();
                        tmpCount++;
                        PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setCurrentSpellShotCount(tmpCount);
                        if (spell->getMaxNumberofShots() == PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getCurrentSpellShotCount() - 1) {
                            MessageFactory factory;
                            std::stringstream ss;
                            float numberOfShots = 0;
                            for (glm::vec2 targetPosition : cachedTargetLocations) {

                                PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).castSpell(targetPosition.x, targetPosition.y);
                                ss << spell->getName() << "|" << spellTypeToString(spell->getSpellType()) << "|" << targetPosition.x << "|" << targetPosition.y << "|";
                                numberOfShots++;


                            }  
                            TestGlobalClientInstance::getInstance().sendMsg(factory.generateCastSpellMessage(MessageType::CASTSPELL, 1, getCurrentTimeInSeconds(),
                                GlobalConfigurations::getInstance().getPlayerName(), numberOfShots,std::move(ss)
                                ).serialize().c_str());
                            cachedCursorPoints.clear();
                            cachedTargetLocations.clear();
                            PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setCurrentSpellShotCount(1.0f); //reset spell shot count after completing 
                        }
                        //------------------------------------------------------------------------------------------------------------------------
                    }
                }
                else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                    if (pointInsideSquare(posX, posY, PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX(), PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY(), (spell->getRange() * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)))) {
                       
                        //-------------------------------------------------------------------------------------------------------------------------
                        // Logic for spells with multiple target selection before cast, also works for single target spells
                        //-------------------------------------------------------------------------------------------------------------------------
                        cachedTargetLocations.push_back(glm::vec2(posX, posY));
                        cachedCursorPoints.insert(cachedCursorPoints.end(), currentCursorPos.begin(), currentCursorPos.end());
                        float tmpCount = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getCurrentSpellShotCount();
                        tmpCount++;
                        PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setCurrentSpellShotCount(tmpCount);
                        if (spell->getMaxNumberofShots() == PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getCurrentSpellShotCount() - 1) {
                            
                            MessageFactory factory;
                            std::stringstream ss;
                            float numberOfShots = 0;
                            for (glm::vec2 targetPosition : cachedTargetLocations) {
                                PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).castSpell(targetPosition.x, targetPosition.y);
                                ss << spell->getName() << "|" << spellTypeToString(spell->getSpellType()) << "|" << targetPosition.x << "|" << targetPosition.y << "|";
                                numberOfShots++;

                            }


                            TestGlobalClientInstance::getInstance().sendMsg(factory.generateCastSpellMessage(MessageType::CASTSPELL, 1, getCurrentTimeInSeconds(),
                                GlobalConfigurations::getInstance().getPlayerName(), numberOfShots, std::move(ss)
                            ).serialize().c_str());


                            cachedCursorPoints.clear();
                            cachedTargetLocations.clear();
                            PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setCurrentSpellShotCount(1.0f); //reset spell shot count after completing 
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
    if (PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).hasSpellChanged()) {
        cachedCursorPoints.clear();
        cachedTargetLocations.clear();
        PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setCurrentSpellShotCount(1.0f); //reset spell shot count 
        PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).resetChangeState(false);
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


         //Checks to see if the a* was able to find a path to the target location,
         //If not set a flag this either allows the player to move or not
         if ((tmp.size() <= 0) || ((tmp.size() / 3) / 2) > (PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getMovementSpeed() / 5.0) + 1) {
             isMoveLocationValid = false; 
             GlobalConfigurations::getInstance().setCursorColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
         }
         else { isMoveLocationValid = true; }

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
        tmp.push_back((float)PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX());
        tmp.push_back((float)PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY());
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
            (float)PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX(),
            (float)PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY(),
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

        std::shared_ptr<Spell> spell = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getSelectedSpell();
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
    float posX = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX();
    float posY = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY();
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
        std::shared_ptr<Spell> spell = PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getSelectedSpell();
        if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
            drawCircle(posX, posY, (spell->getRange() * (GlobalConfigurations::getInstance().getScale())), 50, vertices);
        }
        else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
            drawSquare(posX, posY, (spell->getRange() * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f)), vertices);
        }
        


    }
}
#endif
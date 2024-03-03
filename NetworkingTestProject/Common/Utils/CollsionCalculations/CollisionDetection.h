#pragma once
#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include <cmath>

bool RectangleVsCircle(float circlex, float circley, float circleRadius,
    float rect1x, float rect1y, float rect2x, float rect2y) {
    float distX = std::abs(circlex - ((rect2x + rect1x) / 2.0));
    float distY = std::abs(circley - ((rect2y + rect1y) / 2.0));

    if (distX > ((rect2x - rect1x) / 2.0) + circleRadius) {
        return false;
    }
    if (distY > ((rect2y - rect1y) / 2.0) + circleRadius) {
        return false;
    }

    if (distX <= ((rect2x - rect1x) / 2.0)) {
        return true;
    }
    if (distY <= ((rect2y - rect1y) / 2.0)) {
        return true;
    }

    float dx = distX - ((rect2x - rect1x) / 2.0);
    float dy = distY - ((rect2y - rect1y) / 2.0);
    return (dx * dx + dy * dy <= ((circleRadius * circleRadius)));
}


bool SquareVsSquare(float x1, float y1, float sideLength1,
    float x2, float y2, float sideLength2) {

    // Calculate the minimum and maximum coordinates for each square
    float minX1 = x1 - sideLength1;
    float maxX1 = x1 + sideLength1;
    float minY1 = y1 - sideLength1;
    float maxY1 = y1 + sideLength1;

    float minX2 = x2 - sideLength2;
    float maxX2 = x2 + sideLength2;
    float minY2 = y2 - sideLength2;
    float maxY2 = y2 + sideLength2;

    // Check if there's no intersection along the x-axis
    if (maxX1 < minX2 || maxX2 < minX1) {
        return false;
    }

    // Check if there's no intersection along the y-axis
    if (maxY1 < minY2 || maxY2 < minY1) {
        return false;
    }

    // If there's intersection along both axes, then squares intersect
    return true;
}
#endif
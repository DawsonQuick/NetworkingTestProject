#pragma once
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "./../../vendor/glm/glm.hpp"
struct CameraInfo {
	glm::vec3 m_CameraPos = glm::vec3(0, 0, 0);
	glm::vec3 m_CameraFront = glm::vec3(0, 0, -1);
	glm::vec3 m_CameraUp = glm::vec3(0, 1, 0);

};
class Camera{
public:
	Camera(int id);
	~Camera();
	void update(GLFWwindow* window,float currentFrame);
	CameraInfo getCameraInfo();
	void processInput(GLFWwindow* window);
	void toggleCursorMode(GLFWwindow* window);
	void mouse_callback(double xposIn, double yposIn);
private:
	CameraInfo m_CameraInfo;
	bool m_FirstMouse, m_trackingEnabled;
	float m_DeltaTime, m_LastFrame, m_Yaw, m_Pitch, m_LastX, m_LastY;
};
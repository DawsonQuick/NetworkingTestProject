#include "Camera.h"



Camera::Camera(int id):
    m_DeltaTime(0.0f), m_LastFrame(0.0f),
    m_Yaw(-90.0f), m_Pitch(0), m_FirstMouse(true),
    m_LastX(1920.0f / 2.0f), m_LastY(1080.0f / 2.0f)

{

}
Camera::~Camera() {

}

void Camera::update(GLFWwindow* window , float currentFrame) {
    m_DeltaTime = currentFrame - m_LastFrame;
    m_LastFrame = currentFrame;

    if (m_FirstMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    //Call updates on all the hardware inputs
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouse_callback(mouseX, mouseY);
    processInput(window);
}

CameraInfo Camera::getCameraInfo() {
    return m_CameraInfo;
}

//Presses keyboard inputs
void Camera::processInput(GLFWwindow* window)
{
    static auto lastToggleTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastToggleTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && elapsed.count() > 200) {
        toggleCursorMode(window);
        lastToggleTime = currentTime;
    }
    float cameraSpeed = static_cast<float>(50 * m_DeltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_CameraInfo.m_CameraPos += cameraSpeed * m_CameraInfo.m_CameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_CameraInfo.m_CameraPos -= cameraSpeed * m_CameraInfo.m_CameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_CameraInfo.m_CameraPos -= glm::normalize(glm::cross(m_CameraInfo.m_CameraFront, m_CameraInfo.m_CameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_CameraInfo.m_CameraPos += glm::normalize(glm::cross(m_CameraInfo.m_CameraFront, m_CameraInfo.m_CameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_CameraInfo.m_CameraPos += m_CameraInfo.m_CameraUp * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        m_CameraInfo.m_CameraPos -= m_CameraInfo.m_CameraUp * cameraSpeed;
    }


}

//If the Escape key is pressed the flip the toggle boolean and the change state of mouse tracking
void Camera::toggleCursorMode(GLFWwindow* window) {
    m_trackingEnabled = !m_trackingEnabled;
    glfwSetInputMode(window, GLFW_CURSOR, m_trackingEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

//Processes mouse events and location
void Camera::mouse_callback(double xposIn, double yposIn)
{
    //If the mouse tracking toggle is enables
    if (m_trackingEnabled) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (m_FirstMouse)
        {
            m_LastX = xpos;
            m_LastY = ypos;
            m_FirstMouse = false;
        }

        float xoffset = xpos - m_LastX;
        float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top
        m_LastX = xpos;
        m_LastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_CameraInfo.m_CameraFront = glm::normalize(front);
    }
}


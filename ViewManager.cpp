#include "ViewManager.h"

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>    
#include <iostream>

// Declaration of the global variables and defines
namespace {
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 800;
    const char* g_ViewName = "view";
    const char* g_ProjectionName = "projection";

    Camera* g_pCamera = nullptr;

    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    float gDeltaTime = 0.0f;
    float gLastFrame = 0.0f;

    float cameraSpeed = 2.5f;
    float mouseSensitivity = 0.5f; // Increased for better responsiveness
}

ViewManager::ViewManager(ShaderManager* pShaderManager)
{
    m_pShaderManager = pShaderManager;
    m_pWindow = NULL;
    m_bPerspective = true; // Initialize to perspective

    g_pCamera = new Camera();
    g_pCamera->Position = glm::vec3(0.5f, 5.5f, 10.0f);
    g_pCamera->Front = glm::vec3(0.0f, -0.5f, -2.0f);
    g_pCamera->Up = glm::vec3(0.0f, 1.0f, 0.0f);
    g_pCamera->Zoom = 80;
}

ViewManager::~ViewManager()
{
    m_pShaderManager = NULL;
    m_pWindow = NULL;
    if (NULL != g_pCamera)
    {
        delete g_pCamera;
        g_pCamera = NULL;
    }
}

GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle)
{
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // Lock and hide cursor for fluid camera movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Register callbacks
    glfwSetCursorPosCallback(window, ViewManager::Mouse_Position_Callback);
    glfwSetScrollCallback(window, ViewManager::Scroll_Callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_pWindow = window;

    return(window);
}

void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos)
{
    if (gFirstMouse)
    {
        gLastX = xMousePos;
        gLastY = yMousePos;
        gFirstMouse = false;
    }

    float xOffset = xMousePos - gLastX;
    float yOffset = gLastY - yMousePos;

    gLastX = xMousePos;
    gLastY = yMousePos;

    g_pCamera->ProcessMouseMovement(xOffset * mouseSensitivity, yOffset * mouseSensitivity);
}

void ViewManager::Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset)
{
    g_pCamera->ProcessMouseScroll(yOffset); // Nuanced camera control
}

void ViewManager::ProcessKeyboardEvents()
{
    if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_pWindow, true);

    // Handle Projection Toggles
    if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS) m_bPerspective = true;
    if (glfwGetKey(m_pWindow, GLFW_KEY_O) == GLFW_PRESS) m_bPerspective = false;

    if (NULL == g_pCamera) return;

    if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS) g_pCamera->ProcessKeyboard(FORWARD, gDeltaTime * cameraSpeed);
    if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS) g_pCamera->ProcessKeyboard(BACKWARD, gDeltaTime * cameraSpeed);
    if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS) g_pCamera->ProcessKeyboard(LEFT, gDeltaTime * cameraSpeed);
    if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS) g_pCamera->ProcessKeyboard(RIGHT, gDeltaTime * cameraSpeed);
    if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS) g_pCamera->ProcessKeyboard(UP, gDeltaTime * cameraSpeed);
    if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS) g_pCamera->ProcessKeyboard(DOWN, gDeltaTime * cameraSpeed);
}

void ViewManager::PrepareSceneView()
{
    float currentFrame = glfwGetTime();
    gDeltaTime = currentFrame - gLastFrame;
    gLastFrame = currentFrame;

    ProcessKeyboardEvents();

    glm::mat4 view = g_pCamera->GetViewMatrix();
    glm::mat4 projection;

    if (m_bPerspective) {
        projection = glm::perspective(glm::radians(g_pCamera->Zoom),
            (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    }
    else {
        float scale = 10.0f;
        projection = glm::ortho(-scale, scale, -scale, scale, 0.1f, 100.0f);
    }

    if (m_pShaderManager)
    {
        m_pShaderManager->setMat4Value(g_ViewName, view);
        m_pShaderManager->setMat4Value(g_ProjectionName, projection);
        m_pShaderManager->setVec3Value("viewPosition", g_pCamera->Position);
    }
}
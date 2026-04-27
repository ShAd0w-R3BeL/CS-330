#pragma once
#include "ShaderManager.h"
#include "camera.h"
#include "GLFW/glfw3.h" 

class ViewManager
{
public:
    ViewManager(ShaderManager* pShaderManager);
    ~ViewManager();

    static void Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos);
    static void Scroll_Callback(GLFWwindow* window, double xOffset, double yOffset); // Added

    GLFWwindow* CreateDisplayWindow(const char* windowTitle);
    void PrepareSceneView();

private:
    ShaderManager* m_pShaderManager;
    GLFWwindow* m_pWindow;
    bool m_bPerspective = true; // Added for projection toggle

    void ProcessKeyboardEvents();
};
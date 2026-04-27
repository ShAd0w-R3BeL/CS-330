///////////////////////////////////////////////////////////////////////////////
// shadermanager.h
// ============
// manage the loading and rendering of 3D scenes
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

/***********************************************************
 * SceneManager
 *
 * This class contains the code for preparing and rendering
 * 3D scenes, including the shader settings.
 ***********************************************************/
class SceneManager
{
public:
    // constructor
    SceneManager(ShaderManager* pShaderManager);
    // destructor
    ~SceneManager();

    // properties for loaded texture access
    struct TEXTURE_INFO
    {
        std::string tag;
        uint32_t ID = 0; // Initialized to 0 to resolve uninitialized warning
    };

    // properties for object materials
    struct OBJECT_MATERIAL
    {
        float ambientStrength;
        glm::vec3 ambientColor;
        glm::vec3 diffuseColor;
        glm::vec3 specularColor;
        float shininess;
        std::string tag;
    };

    // Structure for Light Sources
    struct LIGHT_SOURCE
    {
        glm::vec3 position;
        glm::vec3 ambientColor;
        glm::vec3 diffuseColor;
        glm::vec3 specularColor;
        float focalStrength;
        float specularIntensity;
    };

    static const int TOTAL_LIGHTS = 4;

private:
    // pointer to shader manager object
    ShaderManager* m_pShaderManager;
    // pointer to basic shapes object
    ShapeMeshes* m_basicMeshes;
    // the number of loaded textures
    int m_loadedTextures;
    // loaded textures info
    TEXTURE_INFO m_textureIDs[16];

    // Member variables for light and material management
    LIGHT_SOURCE m_lightSources[TOTAL_LIGHTS];
    OBJECT_MATERIAL m_material;

    // methods for managing OpenGL textures
    bool CreateGLTexture(const char* filename, std::string tag);
    void BindGLTextures();
    void DestroyGLTextures();
    int FindTextureID(std::string tag);
    int FindTextureSlot(std::string tag);

    // set the transformation values into the transform buffer
    void SetTransformations(
        glm::vec3 scaleXYZ,
        float XrotationDegrees,
        float YrotationDegrees,
        float ZrotationDegrees,
        glm::vec3 positionXYZ);

    // set the color values into the shader
    void SetShaderColor(
        float redColorValue,
        float greenColorValue,
        float blueColorValue,
        float alphaValue);

    // set the texture data into the shader
    void SetShaderTexture(
        std::string textureTag);

    // set the texture UV scale into the shader
    void SetTextureUVScale(float u, float v);

public:
    // Methods for scene management
    void PrepareScene();
    void RenderScene();
    void LoadSceneTextures();

    // Added methods to resolve identifier errors
    void SetupLightSource(int index, glm::vec3 position, glm::vec3 ambientColor,
        glm::vec3 diffuseColor, glm::vec3 specularColor,
        float focalStrength, float specularIntensity);

    void SetupMaterial(glm::vec3 ambientColor, float ambientStrength,
        glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess);
};
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "MeshType.h"
#include "Renderable.h"
#include "BasicTypes.h"

class Material;
class SceneNode;
class SingleMesh;
class BoundingBox;
class EffectedModel;
class ShaderVariable;
enum class BOUNDING_BOX_TYPE;

class GameObject : public Renderable
{
public:
    GameObject();

    // Model with given name already loaded.
    GameObject(const std::string &gameObjectName, const std::string &modelName,
        MESH_TYPE meshType);

    GameObject(const std::string &gameObjectName, const std::string &modelName,
        std::shared_ptr<EffectedModel> model);

    // Doesn't initialize mesh, shader or material, assume all are 
    // loaded already.
    GameObject(const std::string &gameObjectName, const std::string &modelName, 
        const std::string &meshName, const std::string &materialName);

    // Doesn't initialize mesh or shader, but initializes the material using 
    // the given shader and texture.
    GameObject(const std::string &gameObjectName, const std::string &modelName,
        const std::string &meshName, const std::string &materialName,
        const std::string &shaderName, const std::string &diffuseTexture = "",
        bool loadAsCube = false);

    // Initialize mesh, but not material or shader.
    GameObject(const std::string &gameObjectName, const std::string &modelName, 
        const std::string &meshName, const std::string &materialName,
        PrimitiveType prim, std::vector<Vertex> vertices,
        std::vector<GLuint> indices);

    // Initialize material and shader, but not mesh.
    GameObject(const std::string &gameObjectName, const std::string &modelName,
        const std::string &meshName, const std::string &materialName,
        const std::string &shaderName, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath, 
        std::vector<ShaderVariable> shaderVars, 
        const std::string &diffuseTexture = "", bool loadAsCube = false, 
        bool printShaderLoadStatus = false);

    // Initialization of everything, mesh, material and shader.
    GameObject(const std::string &gameObjectName, const std::string &modelName,
        const std::string &materialName, const std::string &shaderName, 
        PrimitiveType prim, std::vector<Vertex> vertices, 
        std::vector<GLuint> indices, const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath,
        std::vector<ShaderVariable> shaderVars,
        const std::string &diffuseTexture = "", 
        bool printShaderLoadStatus = false);

    void setGameObjectName(const std::string &name);
    std::string getGameObjectName() const;

    void setModelName(const std::string &modelName, MESH_TYPE meshType);
    MESH_TYPE getMeshType() const;
    std::string getModelName() const;
    MeshType* getModel() const;

    void setParentNode(std::shared_ptr<SceneNode> parent);
    SceneNode* getParentNode();

    void setIsVisible(bool visible);
    bool getIsVisible() const;

    void setIsEnabled(bool enabled);
    bool getIsEnabled() const;

    void setAreBoundingBoxesVisible(bool visible);
    bool getAreBoundingBoxesVisible();

    void setScript(std::function<void(SceneNode*, double)> script);
    std::function<void(SceneNode*, double)> getScript() const;

    std::vector<BoundingBox*> getBoundingBoxes() const;

    // GameObject(SceneNode &parent, string modelPath); When model loader 
    // completed.

    void update(double deltaTime);

    std::string className() const { return "GameObject"; }

protected:
    void init(const std::string &gameObjectName, const std::string &modelName,
        MESH_TYPE meshType);

    std::shared_ptr<SceneNode> parentNode;
    std::string gameObjectName, modelName;
    std::function<void(SceneNode*, double)> script;
    std::vector<std::unique_ptr<BoundingBox>> boundingBoxes;
    bool isVisible;        // Determines if being rendered.
    bool isEnabled;        // Determines if scripts active/inactive, collisions.
    bool areBoundingBoxesVisible;
    MESH_TYPE meshType;

private:
    void createBoundingBox(const SingleMesh* sm, BOUNDING_BOX_TYPE bbt);
};
#include "GameObject.h"

#include "SceneNode.h"
#include "BoundingBox.h"
#include "ModelManager.h"
#include "ShaderVariable.h"

using std::string;
using std::vector;
using std::function;
using std::unique_ptr;
using std::shared_ptr;

GameObject::GameObject()
{
	init("");
}

GameObject::GameObject(const string &modelName)
{
	init(modelName);
}

GameObject::GameObject(const string &modelName, shared_ptr<EffectedModel> model)
{
	init(modelName);
	try
	{
		bool success = ModelManager::getInstance()->loadModel(modelName, model);
		if (!success)
			throw std::runtime_error("Model " + modelName + " did not load successfully.");
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

// Initialize neither - assume both model, material are already loaded.
GameObject::GameObject(const string &modelName, const string &meshName, const string &materialName)
{
	init(modelName);
	try
	{
		bool success = ModelManager::getInstance()->loadModel(modelName, meshName, materialName);
		if (!success)
			throw std::runtime_error("Model " + modelName + " did not load successfully.");
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

// Doesn't initialize mesh or shader, but initializes the material using the given shader
// and texture.
GameObject::GameObject(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, const string &diffuseTexture, bool loadAsCube)
{
	init(modelName);
	try
	{
		bool success;
		if (loadAsCube)
			success = ModelManager::getInstance()->loadModel(modelName, meshName,
				materialName, shaderName, diffuseTexture);
		else
			success = ModelManager::getInstance()->loadCubeModel(modelName, meshName,
				materialName, shaderName, diffuseTexture);

		if (!success)
			throw std::runtime_error("Model " + modelName + " did not load successfully.");
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

// Initialize mesh, but not material.
GameObject::GameObject(const string &modelName, const string &meshName, const string &materialName,
	PrimitiveType prim, vector<Vertex> vertices, vector<GLuint> indices)
{
	init(modelName);
	try
	{
		bool success = ModelManager::getInstance()->loadModel(modelName, meshName, materialName,
			prim, vertices, indices);
		if (!success)
			throw std::runtime_error("Model " + modelName + " did not load successfully.");
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

// Initialize material, but not mesh.
GameObject::GameObject(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, const string &vertexShaderPath, const string &fragmentShaderPath,
	vector<ShaderVariable> shaderVars, const string &diffuseTexture,
	bool loadAsCube, bool printShaderLoadStatus)
{
	init(modelName);
	try
	{
		bool success;
		if (loadAsCube)
			success = ModelManager::getInstance()->loadCubeModel(modelName, meshName,
				materialName, shaderName, vertexShaderPath, fragmentShaderPath,
				shaderVars, diffuseTexture, printShaderLoadStatus);
		else
			success = ModelManager::getInstance()->loadModel(modelName, meshName,
				materialName, shaderName, vertexShaderPath, fragmentShaderPath,
				shaderVars, diffuseTexture, printShaderLoadStatus);

		if (!success)
			throw std::runtime_error("Model " + modelName + " did not load successfully.");
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

// Initialization of everything.
GameObject::GameObject(const string &modelName, const string &meshName, const string &materialName,
	const string &shaderName, PrimitiveType prim, vector<Vertex> vertices,
	vector<GLuint> indices, const string &vertexShaderPath,
	const string &fragmentShaderPath, vector<ShaderVariable> shaderVars,
	const string &diffuseTexture, bool printShaderLoadStatus)
{
	init(modelName);
	try
	{
		bool success = ModelManager::getInstance()->loadModel(modelName, meshName, materialName,
			shaderName, prim, vertices, indices, vertexShaderPath,
			fragmentShaderPath, shaderVars, diffuseTexture, 
			printShaderLoadStatus);
		if (!success)
			throw std::runtime_error("Model " + modelName + " did not load successfully.");
	}
	catch (std::exception &e)
	{
		throw e;
	}
}

void
GameObject::setMaterialName(const string &matName)
{ 
	getModel()->setMaterialName(matName);
}

string
GameObject::getMaterialName()
{
	return getModel()->getMaterialName();
}

Material* 
GameObject::getMaterial()
{
	return getModel()->getMaterial();
}

void 
GameObject::setMeshName(const string &meshName)
{ 
	getModel()->setMeshName(meshName);
}

string
GameObject::getMeshName()
{ 
	return getModel()->getMeshName();
}

SingleMesh*
GameObject::getMesh()
{
	return getModel()->getMesh();
}

void 
GameObject::setModelName(const string &modelName)
{ 
	this->modelName = modelName;
}

string
GameObject::getModelName()
{
	return modelName; 
}

EffectedModel*
GameObject::getModel()
{
	return ModelManager::getInstance()->get(modelName);
}

void
GameObject::setParentNode(shared_ptr<SceneNode> parent)
{
	this->parentNode = parent;

	for (size_t i = 0; i < boundingBoxes.size(); i++)
		boundingBoxes[i].get()->updateBounds(parent.get()->getModelMatrix());
}

SceneNode* 
GameObject::getParentNode()
{
	return parentNode.get();
}

void 
GameObject::setIsVisible(bool visible)
{
	isVisible = visible;
}

bool
GameObject::getIsVisible()
{
	return isVisible;
}

void 
GameObject::setIsEnabled(bool enabled)
{
	isEnabled = enabled;
}

bool
GameObject::getIsEnabled()
{
	return isEnabled;
}


void
GameObject::setScript(function<void(SceneNode*, double)> script)
{
	this->script = script;
}

function<void(SceneNode*, double)>
GameObject::getScript()
{
	return script;
}

vector<BoundingBox*>
GameObject::getBoundingBoxes()
{
	vector<BoundingBox*> bbs;
	for (size_t i = 0; i < boundingBoxes.size(); i++)
		bbs.push_back(boundingBoxes[i].get());
	return bbs;
}

void
GameObject::update(double deltaTime)
{
	if (isEnabled)
		script(parentNode.get(), deltaTime);
}

void 
GameObject::init(const string &modelName)
{
	this->modelName = modelName;

	if (!getModel())
		int x = 5;

	script = [](SceneNode*, double) {};
	isEnabled = true;
	isVisible = true;

	// TODO: Change when PolyMesh implemented.
	if (SingleMesh* m = dynamic_cast<SingleMesh*>(getMesh()))
		boundingBoxes.push_back(std::make_unique<BoundingBox>(*m, BOUNDING_BOX_TYPE::OBB));
}
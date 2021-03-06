#include "Enemy.h"

#include "BoundingBox.h"
#include "SceneNode.h"

namespace MazeCrisis
{
    Enemy::Enemy(int maxHealth) : GameObject() 
    {
        setMaxHealth(maxHealth);
        setCurrentHealth(maxHealth);
    }

    Enemy::Enemy(const std::string &gameObjectName,
        const std::string &modelName, int maxHealth) 
        : GameObject(gameObjectName, modelName, MESH_TYPE::SINGLE_MESH)
    {
        setMaxHealth(maxHealth);
        setCurrentHealth(maxHealth);
    }

    int
    Enemy::getMaxHealth() const
    {
        return maxHealth;
    }

    void
    Enemy::setMaxHealth(int maxHealth)
    {
        this->maxHealth = maxHealth;
    }

    int
    Enemy::getCurrentHealth() const
    {
        return currentHealth;
    }

    void
    Enemy::setCurrentHealth(int currentHealth)
    {
        if (currentHealth <= 0)
        {
            this->currentHealth = 0;
            setIsEnabled(false);
            getParentNode()->getParentNode()->removeChildNode(getParentNode());
        }
        else
            this->currentHealth = currentHealth;
    }
}

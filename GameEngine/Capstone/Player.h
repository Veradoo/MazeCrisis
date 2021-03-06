#pragma once
#include <vector>
#include <GL/glew.h>

#include "GLFW\glfw3.h"
#include "RayQuery.h"
#include "Gun.h"
#include "Common.h"

class AbstractRenderer;

namespace MazeCrisis
{
    class UserInterface;

    class Player
    {
    public:
        Player(UserInterface *ui, WEAPON startingWeapon, int maxHealth,
            const std::string &gruntSoundPrefix, unsigned int numGruntSounds, 
            const std::string &gruntSoundFiletype, std::string &deathSoundPath,
            const std::string &pistolSoundPath, 
            const std::string &rifleSoundPath, 
            const std::string &sniperSoundPath,    
            const std::string &emptyClipSoundPath, 
            const std::string &reloadSoundPath,
            const std::string &sayReloadSoundFilePath);

        int getMaxHealth() const;
        void setMaxHealth(int maxHealth);

        int getCurrentHealth() const;
        void setCurrentHealth(int currentHealth);

        WEAPON getCurrentWeapon() const;
        void setCurrentWeapon(WEAPON newGun);

        bool getIsShooting() const;
        void setIsShooting(bool isShooting);

        bool getIsReloading() const;
        void setIsReloading(bool isReloading);

        Gun getCurrentGun() const;

        UserInterface *getUserInterface() const;
        void setUserInterface(UserInterface *ui);

        std::string getSayReloadSoundFilePath() const;
        void setSayReloadSoundFilePath(
            const std::string &sayReloadSoundFilePath);
            
        std::string getDeathSoundFilePath() const;
        void setDeathSoundFilePath(const std::string &deathSoundFilePath);

        void getGruntSoundParameters(std::string &gruntSoundPrefix, 
            std::string &gruntSoundFiletype, unsigned int &numGruntSounds) 
            const;
        void setGruntSoundParameters(const std::string &gruntSoundPrefix,
            const std::string &gruntSoundFiletype, unsigned int numGruntSounds);

        void playRandomGruntSound();

        void update(float deltaTime);
        void mouseHandler(GLFWwindow* window, int button, int action, int mods,
            Ray *ray);
        void keyHandler(GLFWwindow* window, int key, int scancode, int action, 
            int mods);
        void cursorPosHandler(GLFWwindow* window, double x, double y, Ray *ray);

    private:
        int currentHealth, maxHealth, lastMouseState;
        bool isShooting, isReloading;;
        WEAPON currentWeapon;
        std::vector<Gun> guns;
        Ray lastRay;
        UserInterface *ui;

        // Sounds
        std::string sayReloadSoundPath, gruntSoundPrefix, gruntSoundFiletype,
            deathSoundPath;
        unsigned int numGruntSounds;

        void initWeapons(const std::string &pistolSound,
            const std::string &rifleSound, const std::string &sniperSound,
            const std::string &emptyClipSound,
            const std::string &reloadSoundPath);
    };
}
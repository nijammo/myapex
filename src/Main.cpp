#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include "Utils.cpp"
#include "Offsets.cpp"
#include "Level.cpp"
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Sense.cpp"
#include "NoRecoil.cpp"
#include "Aimbot.cpp"
#include "X11Utils.cpp"
#include "ConfigLoader.cpp"

std::string m_fileName = "myapex.ini";

int main(int argc, char *argv[])
{

    if(argc > 1 && !strcmp(argv[1], "--testoffset") == 0) m_fileName = argv[1];


    ConfigLoader *configLoader = new ConfigLoader();

    if (getuid())
    {
        printf("MUST RUN AS ROOT!\n");
        return -1;
    }
    if (mem::GetPID() == 0)
    {
        printf("GAME NOT FOUND. EXITING!\n");
        return -1;
    }

    Level *level = new Level();
    LocalPlayer *localPlayer = new LocalPlayer();
    X11Utils *x11Utils = new X11Utils();
    std::vector<Player *> *players = new std::vector<Player *>;
    
    for (int i = 0; i < 60; i++)
    {
        players->push_back(new Player(i));
    }

    Sense *sense = new Sense(configLoader, level, localPlayer, players, x11Utils);
    NoRecoil *noRecoil = new NoRecoil(configLoader, level, localPlayer, players, x11Utils);
    Aimbot *aimbot = new Aimbot(configLoader, level, localPlayer, players, x11Utils);

    

    if(argc > 1 && strcmp(argv[1], "--testoffset") == 0) {
	printf("Offset testing will begin in 3\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("2\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("1\n");	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("Testing IN_ATTACK\n");
	for(int i=0; i<4; i++) {
		std::cout << localPlayer->isInAttack() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	} 
	printf("Testing TEAM_NUMBER\n");
	for(int i=0; i<4; i++) {
                std::cout << localPlayer->getTeamNumber() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }	
	printf("Testing NAME\n");
	for(int i=0; i<4; i++) {
                std::cout << level->getName() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
	printf("Testing LIFE_STATE\n");
        for(int i=0; i<4; i++) {
                std::cout << localPlayer->isDead() << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }
	
	return 0;
    }




    // Main loop
    printf("Initial config has finished\n");
    while (1)
    {
        try
        {
            // resolve pointers
            localPlayer->markForPointerResolution();
            for (int i = 0; i < players->size(); i++)
            {
                Player *player = players->at(i);
                player->markForPointerResolution();
            }

            // run features
            if (configLoader->isAimbotOn())
                aimbot->update();

            if (configLoader->isNorecoilOn())
                noRecoil->update();

            if (configLoader->isSenseOn())
                sense->update();

        }
        catch (...)
        {
            printf("Loading Screen detected; pausing execution until finsished\n");
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }
}

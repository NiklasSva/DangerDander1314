//GameState II source file

#include "stdafx.h"
#include "GameStateII.h"

#include "EntityManager.h"
#include "SpriteManager.h"
#include "MusicManager.h"
#include "CollisionManager.h"

#include "SoundManager.h"

#include "SpawnerAOEenemy.h"

#include "Player.h"
#include "Level.h"

GameStateII::GameStateII()
{
	m_done = false;
}

bool GameStateII::Enter(SpriteManager* spritemanager, MusicManager* musicmanager)
{
	m_done = false;

	m_direction.x = 0;
	m_direction.y = 0;
	m_angle = 0;

	//Managers
	m_spriteManager = spritemanager;
	m_musicManager = musicmanager;

	m_musicManager->LoadMusic("soundtrack_long_1.wav");
	m_musicManager->Play();

	//new manager and spawners
	m_soundManager = new SoundManager;
	m_soundManager->Initialize("../Sounds/");
	
	m_entityManager = new EntityManager(m_spriteManager->GetSprite("player_weapon.png", 2048, 256), sf::Vector2f(950, 540), m_spriteManager->GetSprite("player_attack.png", 1024, 256), m_spriteManager->GetSprite("player_death.png", 256, 256) , m_spriteManager->GetSprite("pow_effect.png", 128, 128), m_soundManager, m_musicManager);
	m_entityManager->AddPumpMeter(m_spriteManager->GetSprite("new_pumpmeter.png", 800, 246), m_spriteManager->GetSprite("pumpmeter_indicator.png", 70, 94), m_spriteManager->GetSprite("indicator_effect.png", 150, 150), m_spriteManager->GetSprite("meter_effect_left.png", 800, 246), m_spriteManager->GetSprite("meter_effect_right.png", 800, 246), sf::Vector2f(0, 0));
	

	m_spawnerAOEenemy = new SpawnerAOEenemy(m_spriteManager->GetSprite("AOE.png", 1024, 128), m_spriteManager->GetSprite("aoe_attack.png", 256, 256), sf::Vector2f(400, -100) );


	m_levelTop = new Level( m_spriteManager->GetSprite("new_gamespace.png", 1920, 1080), sf::Vector2f(0, 0));
	m_levelBottom = new Level( m_spriteManager->GetSprite("new_gamespace.png", 1920, 1080), sf::Vector2f(0, -1079) );


	return true;
}

void GameStateII::Exit()
{
	if(m_soundManager != nullptr)
	{
		delete m_soundManager;
		m_soundManager = nullptr;
	}

	if(m_entityManager != nullptr)
	{
		delete m_entityManager;
		m_entityManager = nullptr;
	}

	if(m_spawnerAOEenemy != nullptr)
	{
		delete m_spawnerAOEenemy;
		m_spawnerAOEenemy = nullptr;
	}

	if(m_levelTop != nullptr)
	{
		delete m_levelTop;
		m_levelTop = nullptr;
	}

	if(m_levelBottom != nullptr)
	{
		delete m_levelBottom;
		m_levelBottom = nullptr;
	}

}

//State loop

bool GameStateII::Update(float &deltatime)
{
	//std::cout << "GameStateII" << std::endl;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		m_nextState = "MenuStateII";
		m_done = true;
	}
	
	Input();

	m_spawnerAOEenemy->Update(deltatime);
	if(m_spawnerAOEenemy->GetSpawnState())
	{
		m_entityManager->AddEnemyAOE( m_spawnerAOEenemy->Spawn() );
	}

	m_entityManager->Update(m_angle, m_direction, deltatime);	

	m_levelTop->Update(deltatime);
	m_levelBottom->Update(deltatime);

	if(m_entityManager->m_player->GetHP() == 100 || m_entityManager->m_player->GetHP() == 0)
	{
		GameOver();
	}

	return m_done;
}

void GameStateII::Draw(sf::RenderWindow* window)
{
	window->draw(*m_levelTop->GetSprite());
	window->draw(*m_levelBottom->GetSprite());

	m_entityManager->Draw(window);

}

//state switch

std::string GameStateII::Next()
{
	return m_nextState;
}

bool GameStateII::IsType(const std::string &type)
{
	return type.compare("GameStateII") == 0;
}

//private

void GameStateII::Input()
{
	//Attack Input
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) )
	{
		m_entityManager->m_player->Attack();
	}

	//Movement Input
	m_direction.x = 0;
	m_direction.y = 0;			
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_direction.x = -1;
		m_direction.y = -1;
		m_angle = 315;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_direction.x = 1;
		m_direction.y = -1;
		m_angle = 45;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_direction.y = -1;
		m_angle = 0;
	}		
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_direction.x = -1;
		m_direction.y = 1;
		m_angle = 225;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_direction.x = 1;
		m_direction.y = 1;
		m_angle = 135;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_direction.y = 1;
		m_angle = 180;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_direction.x = -1;
		m_angle = 270;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_direction.x = 1;
		m_angle = 90;
	}


}

void GameStateII::GameOver()
{
	


	if(m_entityManager->m_player->GetHP() == 100)
	{
		m_nextState = "GameOverHeart";
		m_done = true;
	}
	else if(m_entityManager->m_player->GetHP() == 0)
	{
		m_nextState = "GameOverSleep";
		m_done = true;
	}

}
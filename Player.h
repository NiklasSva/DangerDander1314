//Player header file

#pragma once

#include "Character.h"

class Player : public Character
{
public:
	Player(sf::Sprite* sprite, sf::Vector2f &position, sf::Sprite* attackSprite, SoundManager* soundmanager, sf::Sprite* deathAnimation);
	~Player();
	
	void Initialize();
	void Cleanup();	
	void Update(float &angle, sf::Vector2f &direction, float &deltatime);
	void HandleCollision();
	void Attack();
	void WeaponStick();
	void ChangeHP(int value);

	int GetWeaponSize();
	
	float GetAttackTimer();

	bool GetAttacking();
	bool GetAttackAnimation();

	sf::Sprite* GetSprite();
	sf::Sprite* GetAttackSprite();

	void SetAttackAnimationStop();
		
	//   power ups on 1,2 and 3?

private:
	SoundManager* m_soundManager;

	float m_moveSoundTimer;
	float m_attackTimer;
	float m_drainTimer;

	int m_weaponSize;
	int m_hpDrain;
	
	bool m_isAttacking;
	bool m_attackAnimation;
	
	sf::Sprite* m_attackSprite;
	float m_attackFrameCounter;
	int m_attackImageNR;

	sf::Sprite* m_deathSprite;
	float m_deathFrameCounter;
	int m_deathImageNR;

};
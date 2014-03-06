//EnemyRanged.cpp

#include "stdafx.h"
#include "EnemyRanged.h"
#include "Collider.h"

#include <cmath>

EnemyRanged::EnemyRanged(sf::Sprite* sprite, sf::Vector2f &position)
{
	m_points = 1;
	m_hp = 1;
	m_isAttacking = false;
	m_attackAnimation = false;
	m_attackTimer = 0.0f;

	m_extension.x = 128;
	m_extension.y = 128;
	m_position = position;
	m_name = "EnemyRanged";
	m_speed = 250;

	m_collider = new Collider;
	m_colliderCircle = true;
	m_collider->SetRadius(60);
	m_collider->SetExtension(GetExtension());
	m_collider->SetPosition(GetPosition());

	m_sprite = sprite;
	m_sprite->setPosition(m_position);
	m_sprite->setOrigin(64, 64);
	
	m_imageNR = 0;
	m_frameCounter = 0.0f;

	m_Direction.y = 1;
	m_Direction.x = 0;
}

EnemyRanged::~EnemyRanged()
{
	m_collisions.clear();

	delete m_collider;
	m_collider = nullptr;
}


void EnemyRanged::Update(float &deltatime, sf::Vector2f refpos)
{
	if(m_position.y < 100)
	{
		m_position.y += m_Direction.y * m_speed * deltatime;
	}
	float deltax = m_position.x - refpos.x;
	float deltay = m_position.y - refpos.y;
	float distance = sqrtf(deltax * deltax + deltay * deltay);
	float angle = atan2(deltay, deltax);
	m_Direction.x = cos(angle) * -1;
	m_Direction.y = sin(angle) * -1;
	m_isAttacking = false;

	if(distance > 400)
	{
		m_position += m_Direction * m_speed * deltatime;
	}
	else 
	{	
		RangedAttack();				
	}

	if(m_attackTimer <= 0.3 && m_attackTimer > 0.0)
	{
		m_attackAnimation = true;

		if(m_attackTimer <= 0.2 && m_attackTimer > 0.0)
			m_isAttacking = true;		
	}

	m_attackTimer -= deltatime;
	if(m_attackTimer < -10)
	{
		m_attackTimer = 0.0f;
	}	

	m_sprite->setTextureRect(sf::IntRect( 128 * m_imageNR, 0, 128, 128));
	m_frameCounter += deltatime;
	if(m_frameCounter >= 0.1f)
	{
		m_imageNR++;
		m_frameCounter = 0.0f;
		if(m_imageNR > 7)
			m_imageNR = 0;
	}
	

	HandleCollision();

	//X-Bounds
	if(m_position.x < 360)
		m_position.x = 360;
	if(m_position.x > 1560)
		m_position.x = 1560;
	//Y-Bounds
	if(m_position.y < -400)
	{
		m_position.y = 0;
	}
	m_collider->SetPosition(m_position);
	m_sprite->setPosition(m_position);
	m_sprite->setRotation(angle);
}

void EnemyRanged::HandleCollision()
{
	for(int i = 0; i < m_collisions.size(); i++)
	{
	 	if(m_collisions[i].first->GetName() == "PlayerAttack")
		{
			m_hp -= 1;
		}
	}

	m_collisions.clear();
}

bool EnemyRanged::GetAttacking()
{
	return m_isAttacking;
}

sf::Vector2f EnemyRanged::GetDirection()
{
	return m_Direction;
}

void EnemyRanged::RangedAttack()
{
	if(m_attackTimer <= 0)
		m_attackTimer = 2.0;
}
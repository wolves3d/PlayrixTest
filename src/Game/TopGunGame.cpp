#include "stdafx.h"
#include "TopGunGame.h"
#include "Bubble.h"
#include "AudioWrapper.h"

#include "GameStates/GameTitle.h"
#include "GameStates/TimesIsUp.h"
#include "GameStates/YouWin.h"


CTopGunGame * g_Game = NULL;


CTopGunGame::CTopGunGame()
	: m_weaponCulldownDeadline(0)
	, m_bubbleTexture(NULL)
	, m_bulletTexture(NULL)
	, m_cannonTexture(NULL)
	, m_maxBubbleCount(200)
	, m_bulletSpeed(200)
	, m_bubbleSpeed(50)
	, m_bubbleSpeedSpread(30)
	, m_stageTime(50)
	, m_isGameStarted(false)
	, m_gameTime(0)
	, m_enableCollisions(true)
	, m_bubbleCount(0)
	, m_cannonNode(NULL)
{
	g_Game = this;
}


void CTopGunGame::Init()
{
	const int screenWidth = Render::device.Width();
	const int screenHeight = Render::device.Height();
	m_grid.InitGridMap(screenWidth, screenHeight, 100);

	m_bubbleTexture = Core::resourceManager.Get<Render::Texture>("Circle");
	m_bulletTexture = Core::resourceManager.Get<Render::Texture>("cur_normal");
	m_cannonTexture = Core::resourceManager.Get<Render::Texture>("Cannon");

	ReadConfig();

	m_stateMgr.PushState(new CGameTitle());

	// --

	m_cannonNode = new CGameNode();
	m_cannonNode->InitWithTexture(m_cannonTexture);
	m_cannonNode->SetAnchorPoint(0, 0.5f);
	m_cannonNode->SetPosition(0.5f * screenWidth, -20);
	m_scene.AddNode(m_cannonNode);
}


void CTopGunGame::ReadConfig()
{
	IO::FileStream fs("input.txt");
	IO::TextReader reader(&fs);

	while (true)
	{
		string line, name, value;

		line = reader.ReadLine();
		if (true == line.empty())
		{
			break;
		}

		if (false == utils::ReadNvp(line, name, value))
		{
			// wrong line?
			continue;
		}

		if ("CountTarget" == name)
		{
			m_maxBubbleCount = utils::lexical_cast<uint>(value);
		}
		else if ("Speed" == name)
		{
			m_bulletSpeed = utils::lexical_cast<uint>(value);
		}
		else if ("Time" == name)
		{
			m_stageTime = utils::lexical_cast<uint>(value);
		}
		else if ("SpeedSpread" == name)
		{
			m_bubbleSpeedSpread = utils::lexical_cast<int>(value);
		}
		else if ("EnableSound" == name)
		{
			AudioWrapper::SetEnabled(utils::lexical_cast<bool>(value));
		}
		else if ("BubbleSpeed" == name)
		{
			m_bubbleSpeed = utils::lexical_cast<uint>(value);
		}
	}
}


void CTopGunGame::Update(float dt)
{
	m_gameTime += dt;

	if (true == m_enableCollisions)
	{
		TestBorders();

		// will call ProcessNeighbourNodes callback many times
		m_grid.Collide(&m_collisionVector, this);
	}
	
	m_scene.Update(dt);
	m_effectsContainer.Update(dt);

	CBaseState * state = m_stateMgr.GetState();
	if (NULL != state)
		state->OnUpdate(dt);

	m_stateMgr.Update();

	RotateCannon();

	// Time's up test ----------------------------------------------------------

	if (true == m_isGameStarted)
	{
		if (0 == m_bubbleCount)
		{
			m_isGameStarted = false;
			m_stateMgr.PopState();
			m_stateMgr.PushState(new CYouWin());
		}
		else if (0 == GetStageTime())
		{
			m_isGameStarted = false;
			m_stateMgr.PopState();
			m_stateMgr.PushState(new CTimeIsUp());
		}
	}
}


void CTopGunGame::RotateCannon()
{
	Vector3 targetPoint = Core::mainInput.GetMousePos();
	Vector3 spawnPos = m_cannonNode->GetPosition();

	Vector3 dir(Vector3(targetPoint) - spawnPos);
	dir.Normalize();

	float angle = fabsf(atan2f(dir.y, dir.x));
	angle = math::max(0.2617f, math::min(angle, 2.8797f)); // limit to 15° gorizont

	m_cannonNode->SetRotation(-angle);
}


void CTopGunGame::Draw()
{
	m_scene.Draw();
	m_effectsContainer.Draw();

	CBaseState * state = m_stateMgr.GetState();
	if (NULL != state)
		state->OnDraw();
}


void CTopGunGame::AddNode(CGameNode * node)
{
	m_scene.AddNode(node);
	m_gameNodes.push_back(node);
	m_grid.AddBody(node);
}


void CTopGunGame::RemoveNode(CGameNode * node)
{
	m_scene.RemoveNode(node);

	m_grid.RemoveBody(node);

	uint i = m_gameNodes.size();
	while (i--)
	{
		if (node == m_gameNodes[i])
		{
			m_gameNodes.erase(m_gameNodes.begin() + i);
			break;
		}
	}
}


void CTopGunGame::RemoveAllNodes()
{
	uint i = m_gameNodes.size();
	while (i--)
	{
		CGameNode * node = m_gameNodes[i];
		m_gameNodes.erase(m_gameNodes.begin() + i);

		m_scene.RemoveNode(node);
		m_grid.RemoveBody(node);

		delete node;
	}
}


uint CTopGunGame::GetStageTime()
{
	if (m_gameDeadline > m_gameTime)
	{
		return (uint)(m_gameDeadline - m_gameTime);
	}
	else
	{
		return 0;
	}
}


void CTopGunGame::CreateBubbles()
{
	RemoveAllNodes();

	m_bubbleCount = 0;
	m_isGameStarted = true;
	m_enableCollisions = true;

	m_gameTime = 0;
	m_weaponCulldownDeadline = 0;
	m_gameDeadline = (m_gameTime + m_stageTime);

	const int screenWidth = Render::device.Width();
	const int screenHeight = Render::device.Height();

	for (uint i = 0; i < m_maxBubbleCount; ++i)
	{
		CBubble * bubble = new CBubble;
		bubble->InitWithTexture(m_bubbleTexture);
		//bubble->SetPosition(RND_INTERVAL(0, screenWidth), RND_INTERVAL(0, screenHeight));
		bubble->SetPosition((0.5f * screenWidth), (0.5f * screenHeight));
		bubble->SetScale(RND_INTERVAL(0.2f, 0.55f));
		bubble->SetTag(BUBBLE_TAG);
		
		// default speed
		Vector3 speedDirection = (Vector3::One * (float)m_bubbleSpeed);

		// apply speed spread
		speedDirection *= (1 + (0.01f * RND_INTERVAL(-m_bubbleSpeedSpread, +m_bubbleSpeedSpread)));

		// set random direction
		RotateVector(speedDirection, RND_INTERVAL(0, 2 * math::PI));
		
		CRigidBody * body = bubble->GetBody();
		body->SetSpeed(speedDirection);
		body->SetRadius(64);

		AddNode(bubble);

		++m_bubbleCount;
	}

	// Playrix connection bug workaround
	m_scene.Update(0);
}



void CTopGunGame::ProcessNeighbourNodes(NodeVector * nodeVector)
{
	const uint nodeCount = nodeVector->size();

	for (uint i = 0; i < nodeCount; ++i)
	{
		CGameNode * unitA = nodeVector->at(i);
		CRigidBody * bodyA = unitA->GetBody();

		for (uint j = (i + 1); j < nodeCount; ++j)
		{
			CGameNode * unitB = nodeVector->at(j);
			CRigidBody * bodyB = unitB->GetBody();

			const Vector3 posA(unitA->GetPosition());
			const Vector3 posB(unitB->GetPosition());

			const float radiusA = (bodyA->GetRadius() * unitA->GetScale());
			const float radiusB = (bodyB->GetRadius() * unitB->GetScale());

			const float radiusSum = radiusA + radiusB;
			const float minDistanceSq = (radiusSum * radiusSum);
			const float actualDistanceSq = posA.DistanceSq(posB);

			if (actualDistanceSq < minDistanceSq)
			{
				// collision detected
				OnCollision(unitA, unitB);

				//Vector3::Distance()

				const float penetrationDepth = -(posA.Distance(posB) - radiusSum);
				const Vector3 pushVector = (posB - posA).Normalize() * (0.1f * penetrationDepth);

				// pull objects
				unitA->SetPosition(posA - (pushVector * (1.0f - bodyA->GetWeight())));
				unitB->SetPosition(posB + (pushVector * (1.0f - bodyB->GetWeight())));
			}
		}
	}

}


void CTopGunGame::TestBorders()
{
	const int screenWidth = Render::device.Width();
	const int screenHeight = Render::device.Height();

	uint i = m_gameNodes.size();
	while (i--)
	{
		CGameNode * node = m_gameNodes[i];
		CRigidBody * body = node->GetBody();

		const Vector3 & speed = body->GetSpeed();
		Vector3 nodePos = node->GetPosition();
		const float radius = (body->GetRadius()) * node->GetScale();

		Vector3 speedMult(1, 1, 1);
		bool hitTest = false;
		const float reflectionRatio = -1.f;

		// x-axis bounce

		if (nodePos.x < radius)
		{
			node->SetPosition(radius, nodePos.y);
			hitTest = true;
			speedMult.x *= reflectionRatio;
		}
		else if ((nodePos.x + radius) > screenWidth)
		{
			node->SetPosition((screenWidth - radius), nodePos.y);
			hitTest = true;
			speedMult.x *= reflectionRatio;
		}

		// y-axis bounce

		if (true == hitTest)
		{
			// update node position (was changed)
			nodePos = node->GetPosition();
		}

		if (nodePos.y < radius)
		{
			node->SetPosition(nodePos.x, radius);
			hitTest = true;
			speedMult.y *= reflectionRatio;
		}
		else if ((nodePos.y + radius) > screenHeight)
		{
			node->SetPosition(nodePos.x, (screenHeight - radius));
			hitTest = true;
			speedMult.y *= reflectionRatio;
		}

		if (true == hitTest)
		{
			OnBounce(node, (speed * speedMult));
		}

		m_grid.AddBody(node); // misprint, code is OK, not actual adding

		//node->SetRotation(node->GetRotation() + 0.01f);
	}
}


void CTopGunGame::OnMouseMove()
{
	CBaseState * state = m_stateMgr.GetState();
	if (NULL != state)
		state->OnMouse();

	if (true == m_isGameStarted)
	{
		if (Core::mainInput.GetMouseLeftButton() || Core::mainInput.GetMouseRightButton())
		{
			if (false == IsWeaponCooldown())
			{
				OnShot(Core::mainInput.GetMousePos(), Core::mainInput.GetMouseRightButton());
			}
		}
	}
}


bool CTopGunGame::IsWeaponCooldown() const
{
	if (0 == m_weaponCulldownDeadline)
		return false;

	return (m_gameTime < m_weaponCulldownDeadline);
}


void CTopGunGame::OnBounce(CGameNode * node, const Vector3 & newDirection)
{
	node->GetBody()->SetSpeed(newDirection);

	if (BULLET_TAG == node->GetTag())
	{
		Vector3 dir(newDirection); // -node->GetPosition());
		dir.Normalize();
		const float angle = -atan2f(dir.y, dir.x);
		node->SetRotation(angle);
	
		AudioWrapper::Play("Miss");



		int collisionCount = node->GetUserData();
		--collisionCount;

		if (collisionCount < 0)
		{
			node->Kill();
		}
		else
		{
			// only one bouce allowed
			node->SetUserData(0);
		}
	}
}


void CTopGunGame::OnShot(const IPoint & targetPoint, bool isSniper)
{
	float culldownTime = (true == isSniper)
		? 0.8f
		: 0.2f;

	m_weaponCulldownDeadline = (m_gameTime + culldownTime); // culldown

	Vector3 cannonGunPoint(100, 16, 0);
	const Vector3 spawnPos = cannonGunPoint.TransformCoord(m_cannonNode->GetMatrix());
	//const Vector3 spawnPos = m_cannonNode->GetPosition() + cannonGunPoint;

	//const Vector3 spawnPos(Render::device.Width() * 0.5f, 50, 0);
	CBubble * bullet = new CBubble;
	bullet->InitWithTexture(m_bulletTexture);
	bullet->SetPosition(spawnPos);
	bullet->SetAnchorPoint(1.0f, 0.5f);
	bullet->SetScale(0.7f);
	bullet->SetTag(BULLET_TAG);

	ParticleEffect * trail = m_effectsContainer.AddEffect("Iskra");
	trail->SetPos(spawnPos.x, spawnPos.y);
	trail->Reset();
	//trail->Finish();
	bullet->SetEffect(trail);

	Vector3 dir(Vector3(targetPoint) - spawnPos);
	const float shootDistance = dir.Length();
	dir.Normalize();
	const float angle = -atan2f(dir.y, dir.x);
	bullet->SetRotation(angle);

	CRigidBody * body = bullet->GetBody();
	body->SetWeight(1.0f);
	body->SetRadius(1);
	body->SetSpeed(dir * (float)m_bulletSpeed);

	if (true == isSniper)
		body->SetSpeed(2 * body->GetSpeed());

	const int collisionCount = (true == isSniper)
		? 5
		: 1;

	bullet->SetUserData(collisionCount);

	AddNode(bullet);

	// sound
	string sampleName("Shot_0");
	sampleName += utils::lexical_cast((int)RND_INTERVAL(1.0f, 2.5f));
	AudioWrapper::Play(sampleName.c_str());

	// cannon bang vfx
	ParticleEffect * splashVFX = m_effectsContainer.AddEffect("Iskra");
	splashVFX->SetPos(spawnPos.x, spawnPos.y);
	splashVFX->Reset();
	splashVFX->Finish();
}


void CTopGunGame::OnCollision(CGameNode * nodeA, CGameNode * nodeB)
{
	CGameNode * bubble = NULL;
	CGameNode * bullet = NULL;
	
	if (BULLET_TAG == nodeA->GetTag())
	{
		bullet = nodeA;
	}
	else if (BULLET_TAG == nodeB->GetTag())
	{
		bullet = nodeB;
	}

	if (NULL == bullet)
	{
		// two bubble collision? early exit
		return;
	}

	if (BUBBLE_TAG == nodeA->GetTag())
	{
		bubble = nodeA;
	}
	else if (BUBBLE_TAG == nodeB->GetTag())
	{
		bubble = nodeB;
	}

	if (NULL == bubble)
	{
		// two bullet collision? early exit
		return;
	}

	// sound
	string sampleName("BubbleSplash_0");
	sampleName += utils::lexical_cast((int)RND_INTERVAL(1.0f, 2.5f));
	AudioWrapper::Play(sampleName.c_str());

	// vfx
	ParticleEffect * splashVFX = m_effectsContainer.AddEffect("FindCoin2");
	splashVFX->SetPos(bubble->GetPosition().x, bubble->GetPosition().y);
	splashVFX->Reset();
	splashVFX->Finish();

	m_grid.RemoveBody(bubble);
	bubble->Kill();
	--m_bubbleCount;

	// bullet life time

	int collisionCount = bullet->GetUserData();
	bullet->SetUserData(--collisionCount);

	if (collisionCount < 1)
	{
		m_grid.RemoveBody(bullet);
		bullet->Kill();
	}

}


void CTopGunGame::ShrinkBubbles()
{
	m_enableCollisions = false;

	const Vector3 screenCenter(
		(Render::device.Width() / 2),
		(Render::device.Height() / 2), 0);


	uint i = m_gameNodes.size();
	while (i--)
	{
		CGameNode * node = m_gameNodes[i];
		CRigidBody * body = node->GetBody();

		if (BULLET_TAG == node->GetTag())
		{
			node->Kill();
			continue;
		}

		const Vector3 nodePos = node->GetPosition();
		Vector3 newSpeed = (nodePos - screenCenter);

		const float flyTime = 0.2f;
		const float flyDistance = newSpeed.Length();

		newSpeed.Normalize();
		newSpeed *= (flyDistance / flyTime);


		body->SetSpeed(newSpeed);
	}
}
#include "stdafx.h"
#include "GameNode.h"
#include "TopGunGame.h"


CGameNode::CGameNode()
	: m_gridIndex(-1)
	, m_tag(0)
	, m_userData(0)
	, m_killFlag(false)
{}


void CGameNode::Update(float dt)
{
	if (true == m_killFlag)
	{
		g_Game->RemoveNode(this);
		return;
	}

	SetPosition(GetPosition() + (m_body.GetSpeed() * dt));
	CSpriteNode::Update(dt);
}
#include "stdafx.h"
#include "Bubble.h"


CBubble::CBubble()
	: m_effect(NULL)
{
}


void CBubble::Update(float dt)
{
	CGameNode::Update(dt);

	if (NULL != m_effect)
	{
		m_effect->SetPos(m_localPos.x, m_localPos.y);
	}
}


void CBubble::Kill()
{
	if (NULL != m_effect)
	{
		m_effect->Finish();
	}

	CGameNode::Kill();
}
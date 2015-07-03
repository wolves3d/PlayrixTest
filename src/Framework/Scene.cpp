#include "stdafx.h"
#include "Scene.h"


void CScene::AddNode(CSceneNode * node)
{
	m_nodes.push_back(node);
}


void CScene::RemoveNode(CSceneNode * node)
{
	uint i = m_nodes.size();

	while (i--)
	{
		if (node == m_nodes[i])
		{
			m_nodes.erase(m_nodes.begin() + i);
			break;
		}
	}
}


void CScene::Update(float dt)
{
	uint i = m_nodes.size();

	while (i--) // backward update to allow node remove on update
	{
		CSceneNode * node = m_nodes[i];
		node->Update(dt);
	}
}


void CScene::Draw()
{
	const uint nodeCount = m_nodes.size();
	for (uint i = 0; i < nodeCount; ++i)
	{
		CSceneNode * node = m_nodes[i];
		node->Draw();
	}
}

#include "stdafx.h"
#include "SpriteNode.h"


CSpriteNode::CSpriteNode()
	: m_texture(NULL)
	, m_UV(0, 1, 0, 1)
	, m_isMatrixDirty(true)
	, m_anchorPoint(0.5f, 0.5f, 0)
	, m_scale(1)
	, m_angle(0)
{
}


bool CSpriteNode::InitWithTexture(Render::Texture * texture)
{
	if (NULL == texture)
		return false;

	m_texture = texture;
	m_rect = FRect(m_texture->getBitmapRect());

	return true;
}


void CSpriteNode::Update(float dt)
{
	if (m_isMatrixDirty == true)
	{
		const Vector3 anchorPos(
			(-m_anchorPoint.x * (m_scale * (m_rect.xEnd - m_rect.xStart))),
			(-m_anchorPoint.y * (m_scale * (m_rect.yEnd - m_rect.yStart))),
			0);
		
		// anchor & scale
		m_worldMatrix = Matrix4::Compose(
			Vector3(m_scale, m_scale, m_scale),
			Quaternion::Identity,
			anchorPos);

		// rotate & rotate
		m_worldMatrix *= Matrix4::Compose(
			Vector3::One,
			Quaternion(0, 0, m_angle),
			m_localPos);

		m_isMatrixDirty = false;
	}
}


void CSpriteNode::Draw()
{
	if (NULL == m_texture)
		return;

	Render::device.PushMatrix();
	//Render::device.ResetMatrix();
	Render::device.MatrixMultiply(m_worldMatrix);
	
	m_texture->Bind();
	Render::DrawRect(m_rect, m_UV);

	Render::device.PopMatrix();
}
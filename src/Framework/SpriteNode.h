#ifndef __SpriteNode_h_included__
#define __SpriteNode_h_included__


#include "Scene.h"


class CSpriteNode : public CSceneNode
{
public:
	CSpriteNode();
	bool InitWithTexture(Render::Texture * texture);
	virtual void Update(float dt);
	virtual void Draw();

	const Vector3 & GetPosition() const { return m_localPos; }
	void SetPosition(float x, float y) { m_localPos.x = x; m_localPos.y = y; m_isMatrixDirty = true; }
	void SetPosition(const Vector3 & pos) { SetPosition(pos.x, pos.y); }
	void SetAnchorPoint(float x, float y) { m_anchorPoint.x = x; m_anchorPoint.y = y; m_isMatrixDirty = true; }

	void SetRotation(float radianAngle) { m_angle = radianAngle; m_isMatrixDirty = true; }
	float GetRotation() const { return m_angle; }

	void SetScale(float scale) { m_scale = scale; m_isMatrixDirty = true; }
	float GetScale() const { return m_scale; }

protected:
	float m_scale;
	float m_angle;
	Vector3 m_localPos;

private:
	Render::Texture * m_texture;
	FRect m_rect;
	FRect m_UV;

	bool m_isMatrixDirty;
	Matrix4 m_worldMatrix;
	Vector3 m_anchorPoint;
};


#endif __SpriteNode_h_included__
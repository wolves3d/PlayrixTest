#ifndef __GameNode_h_included__
#define __GameNode_h_included__


#include "SpriteNode.h"
#include "RigidBody.h"


class CGameNode : public CSpriteNode
{
public:
	CGameNode();
	virtual void Update(float dt);
	CRigidBody * GetBody() { return &m_body; }

	const int GetGridIndex() const { return m_gridIndex; }
	void SetGridIndex(int gridIndex) { m_gridIndex = gridIndex; }

	const int GetTag() const { return m_tag; }
	void SetTag(int tag) { m_tag = tag; }

	virtual void Kill() { m_killFlag = true; }

private:
	CRigidBody m_body;
	int m_gridIndex;
	int m_tag;
	bool m_killFlag;
};

#endif __GameNode_h_included__
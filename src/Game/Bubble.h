#ifndef __Bubble_h_included__
#define __Bubble_h_included__


#include "GameNode.h"


class CBubble : public CGameNode
{
public:
	CBubble();
	virtual void Update(float dt);
	virtual void Kill();

	void SetEffect(ParticleEffect * effect) { m_effect = effect; }

private:
	ParticleEffect * m_effect;
};


#endif __Bubble_h_included__
#ifndef __TopGunGame_h_included__
#define __TopGunGame_h_included__


#include "Scene.h"
#include "UniformGrid.h"
#include "GameStates/BaseState.h"


class CTopGunGame : public IUniformGridDelegate
{
public:
	
	CTopGunGame();
	void Init();
	void ReadConfig();
	void Update(float dt);
	void Draw();
	void OnMouseMove();

	void AddNode(CGameNode * node);
	void RemoveNode(CGameNode * node);

	CStateManager & GetStateManager() { return m_stateMgr; }

	uint GetStageTime();


	void ShrinkBubbles();
	void CreateBubbles();

private:

	void TestBorders();
	void RemoveAllNodes();

	virtual void ProcessNeighbourNodes(NodeVector * nodeVector);

	bool IsWeaponCooldown() const;
	void OnShot(const IPoint & targetPoint, bool isSniper);
	void RotateCannon();

	void OnBounce(CGameNode * node, const Vector3 & newDirection);
	void OnCollision(CGameNode * nodeA, CGameNode * nodeB);


	CScene m_scene;
	vector <CGameNode *> m_gameNodes;

	CUniformGrid m_grid;
	NodeVector m_collisionVector;

	float m_weaponCulldownDeadline;

	// texture cache
	Render::Texture * m_bubbleTexture;
	Render::Texture * m_bulletTexture;
	Render::Texture * m_cannonTexture;

	CGameNode * m_cannonNode;
	EffectsContainer m_effectsContainer;

	enum ENodeTag
	{
		BUBBLE_TAG = 1,
		BULLET_TAG = 2,
	};

	// Game parameters

	uint m_maxBubbleCount;
	uint m_bubbleCount;
	uint m_bubbleSpeed;
	int m_bubbleSpeedSpread;
	int m_bulletSpeed;
	uint m_stageTime;

	bool m_isGameStarted;
	bool m_enableCollisions;
	float m_gameTime;
	float m_gameDeadline;

	CStateManager m_stateMgr;
};


#endif __TopGunGame_h_included__
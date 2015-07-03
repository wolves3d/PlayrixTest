#ifndef __Scene_h_included__
#define __Scene_h_included__


class CSceneNode
{
public:
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
};


class CScene
{
public:
	void AddNode(CSceneNode * node);
	void RemoveNode(CSceneNode * node);
	void Update(float dt);
	void Draw();

private:
	vector <CSceneNode *> m_nodes;
};


#endif __Scene_h_included__
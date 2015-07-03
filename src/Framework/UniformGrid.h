#ifndef __uniform_grid_h_included__
#define __uniform_grid_h_included__


struct IUniformGridDelegate
{
	virtual void ProcessNeighbourNodes(NodeVector * nodeVector) = 0;
};


class CUniformGrid
{
public:
	CUniformGrid();
	void InitGridMap(float width, float height, float cellSize);

	void AddBody(CGameNode * node);
	void RemoveBody(CGameNode * node);

	void Collide(NodeVector * outVector, IUniformGridDelegate * delegate);

	void GetUnitsInRadius(NodeVector * outVector, const Vector3 & point, float radius);

private:

	void CellCoordFromMapPoint(int * outX, int * outY, const Vector3 & mapPoint);
	int GetCellIndex(const Vector3 & point);
	NodeVector * GetCell(int x, int y);

	NodeVector * m_GridArray;
	int m_Width, m_Height;
	float m_CellSize;

//	CCSize m_MapSize;
	float m_gridMapWidth;
	float m_gridMapHeight;
};


#endif // #ifndef __uniform_grid_h_included__
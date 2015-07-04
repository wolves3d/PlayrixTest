#include "stdafx.h"
#include "UniformGrid.h"
#include "GameNode.h"


CUniformGrid::CUniformGrid()
	: m_GridArray(NULL)
	, m_CellSize(0.0f)
	, m_Width(0)
	, m_Height(0)
{
}


void CUniformGrid::InitGridMap(float width, float height, float cellSize)
{
	m_gridMapWidth = width;
	m_gridMapHeight = height;

	m_CellSize = cellSize;
	m_Width = (int)ceilf((m_gridMapWidth / m_CellSize));
	m_Height = (int)ceilf((m_gridMapHeight / m_CellSize));

	const uint cellCount = (m_Width * m_Height);

	m_GridArray = new NodeVector[cellCount];
}


void CUniformGrid::CellCoordFromMapPoint(int * outX, int * outY, const Vector3 & mapPoint)
{

	(*outX) = (int)floorf(mapPoint.x / m_CellSize);
	(*outY) = (int)floorf(mapPoint.y / m_CellSize);
}


int CUniformGrid::GetCellIndex(const Vector3 & point)
{
	int x, y;
	CellCoordFromMapPoint(&x, &y, point);

	return ((y * m_Width) + x);
}


void CUniformGrid::AddBody(CGameNode * node)
{
	const int CurrentIndex = node->GetGridIndex();
	const int NewIndex = GetCellIndex(node->GetPosition());

	if (-1 != CurrentIndex)
	{
		if (NewIndex == CurrentIndex)
		{
			// Nothing to do, already in right place
			return;
		}
		else
		{
			// Got this unit but in wrong cell, so remove first
			RemoveBody(node);
		}
	}

	m_GridArray[NewIndex].push_back(node);
	node->SetGridIndex(NewIndex);
}


void CUniformGrid::RemoveBody(CGameNode * node)
{
	const int Idx = node->GetGridIndex();
	if (-1 != Idx)
	{
		NodeVector * cellList = (m_GridArray + Idx);
		NodeVector::iterator it = cellList->begin();

		while (cellList->end() != it)
		{
			if (node == (*it))
			{
				cellList->erase(it);
				return;
			}

			++it;
		}
	}
}


void CUniformGrid::Collide(NodeVector * dstVector, IUniformGridDelegate * delegate)
{
	for (int y = 0; y < m_Height; ++y)
	{
		for (int x = 0; x < m_Width; ++x)
		{
			dstVector->clear();

			NodeVector * cellList = GetCell(x, y);
			dstVector->insert(dstVector->end(), cellList->begin(), cellList->end());

			cellList = GetCell(x + 1, y);
			if (NULL != cellList)
			{
				dstVector->insert(dstVector->end(), cellList->begin(), cellList->end());
			}

			cellList = GetCell(x + 1, y + 1);
			if (NULL != cellList)
			{
				dstVector->insert(dstVector->end(), cellList->begin(), cellList->end());
			}

			cellList = GetCell(x, y + 1);
			if (NULL != cellList)
			{
				dstVector->insert(dstVector->end(), cellList->begin(), cellList->end());
			}
			
			if (dstVector->size() > 0)
			{
				delegate->ProcessNeighbourNodes(dstVector);
			}
		}
	}
}


NodeVector * CUniformGrid::GetCell(int x, int y)
{
	if (x < m_Width)
	{
		if (y < m_Height)
		{
			return (m_GridArray + ((y * m_Width) + x));
		}
	}

	return NULL;
}


void CUniformGrid::GetUnitsInRadius(NodeVector * dstVector, const Vector3 & point, float radius)
{
	const Vector3 radiusPoint(radius, radius, 0);

	int minX, minY;
	CellCoordFromMapPoint(&minX, &minY, point - radiusPoint);
	minX = math::max(0, math::min(minX, (m_Width - 1)));
	minY = math::max(0, math::min(minY, (m_Height - 1)));

	int maxX, maxY;
	CellCoordFromMapPoint(&maxX, &maxY, point + radiusPoint);
	maxX = math::max(0, math::min(maxX, (m_Width - 1)));
	maxY = math::max(0, math::min(maxY, (m_Height - 1)));

	dstVector->clear();

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			// FIXME: non optimal - square, refactor me
			NodeVector *  gridVector = GetCell(x, y);
			dstVector->insert(dstVector->end(), gridVector->begin(), gridVector->end());
		}
	}
}
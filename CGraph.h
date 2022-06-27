#pragma once
#include "define.h"

enum class SEARCH
{
	DFS,
	BFS
};

template <typename T>
class Graph
{
private:
	T** m_matrix;
	int m_rows;
	int m_cols;
public:
	Graph(int _rows, int _cols);
	~Graph();

private:
	void DepthFirstSearch(int _now, bool* _visited);
	void BreadthFirstSearch(int _start);

public:
	void SetMatrix(T** _matrix);

public:
	void Search(SEARCH _search);
	void Print();
};

template <typename T>
Graph<T>::Graph(int _rows, int _cols)
	: m_rows(_rows)
	, m_cols(_cols)
{
	m_matrix = new int* [m_rows];
	for (int i = 0; i < m_rows; ++i)
		m_matrix[i] = new int[m_cols];
}

template <typename T>
Graph<T>::~Graph()
{
	for (int i = 0; i < m_rows; ++i)
	{
		delete[] m_matrix[i];
	}
	delete[] m_matrix;
}

template <typename T>
void Graph<T>::SetMatrix(T** _matrix)
{
	m_matrix = _matrix;
}

template <typename T>
void Graph<T>::Print()
{
	for (int i = 0; i < m_rows; ++i)
	{
		for (int j = 0; j < m_cols; ++j)
		{
			std::cout << m_matrix[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

template <typename T>
void Graph<T>::DepthFirstSearch(int _now, bool* _visited)
{
	// 깊이 우선 탐색
	printf("%d\n", _now);
	_visited[_now] = true;
	for (int next = 0; next < m_cols; ++next)
	{
		// 연결되어 있지 않은 경우
		if (m_matrix[_now][next] == 0)
			continue;

		// 연결되어 있지만 방문한 경우
		if (_visited[next])
			continue;

		// 다음으로 넘어가서 순회.
		DepthFirstSearch(next, _visited);
	}
}

template <typename T>
void Graph<T>::BreadthFirstSearch(int _start)
{
	bool* found = new bool[m_cols];
	bool* parent = new bool[m_cols];
	bool* dist = new bool[m_cols];
	queue<int> q;
	q.push(_start);
	found[_start] = true;
	parent[_start] = _start;
	dist[_start] = 0;

	while (q.size() > 0)
	{
		int now = q.front();
		q.pop();
		std::cout << now << '\n';

		for (int next = 0; next < m_cols; ++next)
		{
			if (m_matrix[now][next] == 0)
				continue;
			
			if (found[next] == true)
				continue;

			q.push(next);
			found[next] = true;
			parent[next] = now;
			dist[next] = dist[now] + 1;
		}
	}

	delete[] found;
}

template <typename T>
void Graph<T>::Search(SEARCH _search)
{
	switch (_search)
	{
	case SEARCH::DFS:
	{
		bool* visited = new bool[m_cols];
		std::fill_n(visited, m_cols, false);
		for (int now = 0; now < m_cols; ++now)
		{
			if (visited[now] == false)
			{
				DepthFirstSearch(now, visited);
			}
		}
		delete[] visited;
		break;
	}
	case SEARCH::BFS:
	{
		BreadthFirstSearch(0);
		break;
	}
	}

}


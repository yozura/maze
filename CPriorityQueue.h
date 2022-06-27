#pragma once
#include "define.h"

template <typename T>
class PriorityQueue
{
private:
	vector<T> m_heap;
	 
public:
	PriorityQueue();

public:
	// O(LogN) 의 시간복잡도를 가진다.
	void Push(T _data);
	T Pop();
	int Count();
};

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
}

template <typename T>
void PriorityQueue<T>::Push(T _data)
{
	// 힙의 맨 끝에 데이터를 삽입한다.
	m_heap.push_back(_data);

	int now = m_heap.size() - 1;
	while (now > 0)
	{
		// 힙 트리의 부모 인덱스
		int next = (now - 1) / 2;

		// 만약 현재 위치의 부모보다 값이 작은 경우에는 바로 종료
		if (m_heap[now] < m_heap[next])
			break;

		// 부모와 값 교환
		int temp = m_heap[now];
		m_heap[now] = m_heap[next];
		m_heap[next] = temp;

		// 검사 위치를 이동한다.
		now = next;
	}
}

template <typename T>
T PriorityQueue<T>::Pop()
{
	// 반환할 데이터를 저장한다.
	T ret = m_heap.front();

	// 마지막 데이터를 루트로 이동하고 마지막 공간을 지운다.
	m_heap[0] = m_heap.back();
	m_heap.pop_back();

	// 왼쪽 노드와 오른쪽 노드의 크기를 비교해 더 큰 노드와 교환한다.
	int lastIndex = m_heap.size() - 1;
	int now = 0;
	while (true)
	{
		int left = 2 * now + 1;
		int right = 2 * now + 2;

		int next = now;
		if (left <= lastIndex && m_heap[next] < m_heap[left])
			next = left;

		if (right <= lastIndex && m_heap[next] < m_heap[right])
			next = right;

		// 왼쪽 / 오른쪽 모두 현재값보다 작으면 종료
		if (next == now)
			break;

		// 두 값을 교체한다.
		int temp = m_heap[now];
		m_heap[now] = m_heap[next];
		m_heap[next] = temp;

		// 검사 위치를 이동한다.
		now = next;
	}

	return ret;
}

template <typename T>
int PriorityQueue<T>::Count()
{
	return m_heap.size();
}
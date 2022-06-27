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
	// O(LogN) �� �ð����⵵�� ������.
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
	// ���� �� ���� �����͸� �����Ѵ�.
	m_heap.push_back(_data);

	int now = m_heap.size() - 1;
	while (now > 0)
	{
		// �� Ʈ���� �θ� �ε���
		int next = (now - 1) / 2;

		// ���� ���� ��ġ�� �θ𺸴� ���� ���� ��쿡�� �ٷ� ����
		if (m_heap[now] < m_heap[next])
			break;

		// �θ�� �� ��ȯ
		int temp = m_heap[now];
		m_heap[now] = m_heap[next];
		m_heap[next] = temp;

		// �˻� ��ġ�� �̵��Ѵ�.
		now = next;
	}
}

template <typename T>
T PriorityQueue<T>::Pop()
{
	// ��ȯ�� �����͸� �����Ѵ�.
	T ret = m_heap.front();

	// ������ �����͸� ��Ʈ�� �̵��ϰ� ������ ������ �����.
	m_heap[0] = m_heap.back();
	m_heap.pop_back();

	// ���� ���� ������ ����� ũ�⸦ ���� �� ū ���� ��ȯ�Ѵ�.
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

		// ���� / ������ ��� ���簪���� ������ ����
		if (next == now)
			break;

		// �� ���� ��ü�Ѵ�.
		int temp = m_heap[now];
		m_heap[now] = m_heap[next];
		m_heap[next] = temp;

		// �˻� ��ġ�� �̵��Ѵ�.
		now = next;
	}

	return ret;
}

template <typename T>
int PriorityQueue<T>::Count()
{
	return m_heap.size();
}
#pragma once
#include"QueueADT.h"
#include<iostream>
template <class T>
class SFJqueue :public QueueADT<T>
{
private:
	Node<T>* head;
	int queueCount;
	Node<T>* CopyChain(Node<T>* originChainPtr);
public:
	SFJqueue();
	SFJqueue(const SFJqueue& pQueue);
	bool isEmpty() const;
	int getcount() const;
	bool enqueue(const T& anEntry);
	bool dequeue(T& x);
	bool peek(T& x) const;
	void print() const;
	virtual ~SFJqueue();
};

template<class T>
inline Node<T>* SFJqueue<T>::CopyChain(Node<T>* originChainPtr)
{
	Node<T>* copiedChainPtr;
	if (originChainPtr)
	{
		copiedChainPtr = new Node<T>(originChainPtr->getItem());
		copiedChainPtr->setNext(CopyChain(originChainPtr->setNext()));
	}
	return copiedChainPtr;
}

template<class T>
inline SFJqueue<T>::SFJqueue() :queueCount(0), head(nullptr)
{
}

template<class T>
inline SFJqueue<T>::SFJqueue(const SFJqueue& pQueue)
{
	head = CopyChain(pQueue);
	queueCount = pQueue.queueCount;
}

template<class T>
inline bool SFJqueue<T>::isEmpty() const
{
	return queueCount == 0;
}

template<class T>
inline int SFJqueue<T>::getcount() const
{
	return queueCount;
}

template<class T>
inline bool SFJqueue<T>::enqueue(const T& anEntry)
{
	Node<T>* newNodePtr = new Node<T>(anEntry);
	Node<T>* p = head;
	queueCount++;
	if (!head)
	{
		head = newNodePtr;
		return true;
	}
	if (*(newNodePtr->getItem()) < *(head->getItem()))
	{
		newNodePtr->setNext(head);
		head = newNodePtr;
		return true;
	}
	while (p->getNext())
	{
		if (*p->getNext()->getItem() > *newNodePtr->getItem())
		{
			break;
		}
		p = p->getNext();
	}
	newNodePtr->setNext(p->getNext());
	p->setNext(newNodePtr);
	return true;
}

template<class T>
inline bool SFJqueue<T>::dequeue(T& x)
{
	if (isEmpty())
	{
		return false;
	}
	Node<T>* temp = head;
	x = head->getItem();
	head = head->getNext();
	delete temp;
	queueCount--;
	return true;
}

template<class T>
inline bool SFJqueue<T>::peek(T& x) const
{
	if (isEmpty())
		return false;
	x = head->getItem();
	return true;
}

template<class T>
inline void SFJqueue<T>::print() const
{
	Node<T>* ptr = head;
	while (ptr) // not null
	{
		std::cout << *(ptr->getItem());
		if (ptr->getNext())
			std::cout << ", "; // If there are still elements then separate by commas 
		ptr = ptr->getNext();
	}
}

template<class T>
inline SFJqueue<T>::~SFJqueue()
{
}

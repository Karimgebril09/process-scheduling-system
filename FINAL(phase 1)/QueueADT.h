#pragma once
#include "Node.h"
using namespace std;
template <typename T>
class QueueADT
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	QueueADT()
	{
	}
	virtual bool isEmpty() const = 0;
	virtual bool enqueue(const T& newEntry) = 0;
	virtual bool dequeue(T& frntEntry) = 0;
	virtual bool peek(T& frntEntry)  const = 0;
	virtual int getcount() const = 0;
	virtual void print() const = 0;
	virtual~QueueADT();
};

template<typename T>
inline QueueADT<T>::~QueueADT()
{
}

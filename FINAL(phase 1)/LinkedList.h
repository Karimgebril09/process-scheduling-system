#pragma once
#include "Node.h" 
#include "process.h"
#include<iostream>
template< class ItemType>
class LinkedList
{
protected:
	Node<ItemType>* headPtr; // Pointer to first node in the chain 
	// (contains the first entry in the list) 
	int itemCount; // Current count of list items
	// Locates a specified node in a linked list. 
	// @pre position is the number of the desired node; 
	// position >= 1 and position <= itemCount. 
	// @post The node is found and a pointer to it is returned. 
	// @param position The number of the node to locate. 
	// @return A pointer to the node at the given position. 
	Node<ItemType>* getNodeAt(int position) const;
public:
	LinkedList();
	LinkedList(const LinkedList<ItemType>& origlist);
	virtual ~LinkedList();
	bool isEmpty() const;
	int getLength() const;
	bool insert(int newPosition, const ItemType& newEntry);
	bool remove(int position);
	void clear();
	ItemType getEntry(int position) const;
	bool setEntry(int position, const ItemType& newEntry);
	void print() const;
}; // end LinkedList 

template< class ItemType>
LinkedList<ItemType>::LinkedList() : headPtr(nullptr), itemCount(0)
{
}

template<class ItemType>
inline LinkedList<ItemType>::LinkedList(const LinkedList<ItemType>& origlist)
{
	//Node<ItemType>* origlistptr = origlist.Head;
	//if (origlistptr == nullptr)
	//{
	//	headPtr = nullptr;
	//}
	//else
	//{
	//	Node<ItemType>* newlistptr = new Node<ItemType>();
	//	newlistptr->setItem(origlistptr->getItem());
	//	headPtr = newlistptr;
	//	while (origlistptr->getNext())
	//	{
	//		origlistptr = origlistptr->getNext();
	//		Node<ItemType>* newnodeptr = new Node<ItemType>(origlistptr->getItem());
	//		newlistptr->setNext(newnodeptr);
	//		newlistptr = newlistptr->getNext();
	//	}
	//}
}

template<class ItemType>
inline LinkedList<ItemType>::~LinkedList()
{
	clear();
}

template<class ItemType>
inline bool LinkedList<ItemType>::isEmpty() const
{
	return itemCount == 0;
}

template<class ItemType>
inline int LinkedList<ItemType>::getLength() const
{
	return itemCount;
}


template< class ItemType>
Node<ItemType>* LinkedList<ItemType>::getNodeAt(int position) const
{
	// Count from the beginning of the chain 
	Node<ItemType>* curPtr = headPtr;
	for (int skip = 0; skip < position; skip++)
		curPtr = curPtr->getNext();
	return curPtr;
} // end getNodeAt


template< class ItemType>
ItemType LinkedList<ItemType>::getEntry(int position) const
{
	// Enforce precondition 
	bool ableToGet = (position >= 0) && (position < itemCount);
	if (ableToGet)
	{
		Node<ItemType>* nodePtr = getNodeAt(position);
		return nodePtr->getItem();
	}
} // end getEntry

template<class ItemType>
inline bool LinkedList<ItemType>::setEntry(int position, const ItemType& newEntry)
{
	if (position<0 || position>itemCount)
	{
		return false;
	}
	int x = 0;
	Node<ItemType>* p = headPtr;
	while (x != position)
	{
		p = p->getNext();
	}
	p->setItem(newEntry);
	return true;
}
template<>
inline void LinkedList<process*>::print() const
{
	Node<process*>* ptr = headPtr;
	while (ptr) // not null
	{
		std::cout << *(ptr->getItem());
		if (ptr->getNext())
			std::cout << ", "; // If there are still elements then separate by commas 
		ptr = ptr->getNext();
	}
}
template<class ItemType>
inline void LinkedList<ItemType>::print() const
{
	Node<ItemType>* start = headPtr;
	while (start)
	{
		std::cout << *(start->getItem());
		if (start->getNext())
			std::cout << ", ";
		start = start->getNext();
	}
}


template< class ItemType>
bool LinkedList<ItemType>::insert(int newPosition,
	const ItemType& newEntry)
{
	bool ableToInsert = (newPosition >= 0) &&
		(newPosition <= itemCount);
	if (ableToInsert)
	{
		// Create a new node containing the new entry
		Node<ItemType>* newNodePtr = new Node<ItemType>(newEntry);
		// Attach new node to chain 
		if (newPosition == 0)
		{
			// Insert new node at beginning of chain 
			newNodePtr->setNext(headPtr);
			headPtr = newNodePtr;
		}
		else
		{
			// Find node that will be before new node 
			Node<ItemType>* prevPtr = getNodeAt(newPosition - 1);
			// Insert new node after node to which prevPtr points 
			newNodePtr->setNext(prevPtr->getNext());
			prevPtr->setNext(newNodePtr);
		} // end if 
		itemCount++; // Increase count of entries 
	} // end if 
	return ableToInsert;
} // end insert


template< class ItemType>
bool LinkedList<ItemType>::remove(int position)
{
	bool ableToRemove = (position >= 0) && (position < itemCount);
	if (ableToRemove)
	{
		Node<ItemType>* curPtr = nullptr;
		if (position == 0)
		{
			// Remove the first node in the chain 
			curPtr = headPtr; // Save pointer to node 
			headPtr = headPtr->getNext();
		}
		else
		{
			// Find node that is before the one to delete 
			Node<ItemType>* prevPtr = getNodeAt(position - 1);
			// Point to node to delete 
			curPtr = prevPtr->getNext();
			// Disconnect indicated node from chain by connecting the 
			 // prior node with the one after 
			prevPtr->setNext(curPtr->getNext());
		} // end if 
	   // Return node to system 
		curPtr->setNext(nullptr);
		delete curPtr;
		curPtr = nullptr;
		itemCount--; // Decrease count of entries 
	} // end if 
	return ableToRemove;
} // end remove


template< class ItemType>
void LinkedList<ItemType>::clear()
{
	while (!isEmpty())
		remove(0);
} // end clear

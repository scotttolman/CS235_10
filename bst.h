/***********************************************************************
 * Component:
 *    Assignment 09, Binary Search Tree (BST)
 *    Brother Brother Kirby, CS 235
 * Author:
 *    Scott Tolman, Mark Baldwin
 * Summary:
 *    Create a binary search tree
 ************************************************************************/

#ifndef BST_H
#define BST_H

#include <iostream>
#include <cassert>

namespace custom
{
	/************************************************
	* CLASS BST
	* contains all methods for Binary Seach Tree
	*************************************************/
	template <class T>
	class BST
	{
	public:
		class BNode;
		class iterator;
		
		
		BST() : root(NULL), numElements(0), right(true) {}
		BST(const BST<T> & rhs) throw (const char*);
		~BST() { clear(); }
		BST<T>& operator =(const BST<T> & rhs);
		
		
		int size() { return numElements; }
		bool empty() { return (root == NULL); }
		void clear() { deleteBinaryTree(this->root); }
		void insert(const T & t) throw (const char*);
		void erase(iterator it);
		
		
		iterator find(const T & t);
		iterator begin();
		iterator end() { return NULL; }
		iterator rbegin();
		iterator rend() { return NULL; }
		
		
		BNode* getRoot() const { return root; }
	
	private:
		BNode* root;
		int numElements;
		bool right;


		void deleteNode(BNode* del, bool right);				//called from erase()
		void deleteBinaryTree(BNode*& pNode);
		void copyBinaryTree(BNode* src, BNode*& dest);
	};

	/************************************************
	* CLASS BST<T>::BNODE
	* contains members and constructors for BNode
	*************************************************/
	template <class T>
	class BST<T>::BNode
	{
	public:
		T data;
		BNode* pLeft;
		BNode* pRight;
		BNode* pParent;
		bool isRed;
		BNode() : pLeft(NULL), pRight(NULL), pParent(NULL), isRed(true) {}
		BNode(const T & t) : data(t), pLeft(NULL), pRight(NULL), pParent(NULL), isRed(true) {}
	private:
		void verifyRB(int depth) {} //recursive, useful for verifying property d) of a red-black tree
		void verifyBST() {} //Ensures that the subtree under a given node is a valid BST
		void balance(); //Contains all the red-black balancing logic
	};

	/************************************************
	* CLASS BST<T>::ITERATOR
	* contains all methods for iterator
	*************************************************/
	template <class T>
	class BST<T>::iterator
	{
	public:
		iterator() : pNode(NULL) {}
		iterator(BNode* p) : pNode(p) {}
		iterator(const iterator& rhs) { this->pNode = rhs.pNode; }
		iterator& operator =(const iterator& rhs);
		bool operator ==(const iterator& it) { return (this->pNode == it.pNode); }
		bool operator !=(const iterator& it) { return (this->pNode != it.pNode); }
		iterator operator ++();
		iterator operator ++(int postfix) { return *this; }
		iterator & operator -- ();
		iterator operator --(int postfix) { return *this; }
		T& operator *() { return pNode->data; }
		typename BST<T>::BNode* getPNode() const { return pNode; }
	private:
		typename BST<T>::BNode* pNode;
	};

	/************************************************
	* ITERATOR ASSIGNMENT OPERATOR
	* assigns rhs.pNode to this->pNode
	*************************************************/
	template <class T>
	typename BST<T>::iterator & BST<T>::iterator::operator=(const iterator & rhs)
	{
		pNode = rhs.pNode;
		return *this;
	}

	/************************************************
	* INCREMENT OPERATOR
	* contains logic to increment the iterator
	*************************************************/
	template <class T>
	typename BST<T>::iterator BST<T>::iterator::operator++()
	{
		// do nothing if we have nothing
		if (!pNode)
			return *this;

		// if there is a right node, take it
		if (pNode->pRight)
		{
			// go right
			pNode = pNode->pRight;

			// jig left - there might be more right-most children
			while (pNode->pLeft)
				pNode = pNode->pLeft;
			return *this;
		}

		// there are no right children, the left are done
		assert(NULL == pNode->pRight);
		BNode * pSave = pNode;

		// go up
		pNode = pNode->pParent;

		// if the parent is the NULL, we are done!
		if (NULL == pNode)
			return *this;

		// if we are the left-child, go to the parent.
		if (pSave == pNode->pLeft)
			return *this;

		// we are the right-child, go up as long as we are the left child!
		while (NULL != pNode && pSave == pNode->pRight)
		{
			pSave = pNode;
			pNode = pNode->pParent;
		}

		return *this;
	}

	/**************************************************
	 * BST ITERATOR :: DECREMENT PREFIX
	 *     advance by one. Note that this implementation uses
	 *     a stack of nodes to remember where we are. This stack
	 *     is called "nodes".
	 * Author:      Br. Helfrich
	 * Performance: O(log n) though O(1) in the common case
	 *************************************************/
	template <class T>
	typename BST <T> ::iterator & BST <T> ::iterator :: operator -- ()
	{
		// do nothing if we have nothing
		if (NULL == pNode)
			return *this;

		// if there is a left node, take it
		if (NULL != pNode->pLeft)
		{
			// go left
			pNode = pNode->pLeft;

			// jig right - there might be more right-most children
			while (pNode->pRight)
				pNode = pNode->pRight;
			return *this;
		}

		// there are no left children, the right are done
		assert(NULL == pNode->pLeft);
		BNode * pSave = pNode;

		// go up
		pNode = pNode->pParent;

		// if the parent is the NULL, we are done!
		if (NULL == pNode)
			return *this;

		// if we are the right-child, got to the parent.
		if (pSave == pNode->pRight)
			return *this;

		// we are the left-child, go up as long as we are the left child!
		while (NULL != pNode && pSave == pNode->pLeft)
		{
			pSave = pNode;
			pNode = pNode->pParent;
		}

		return *this;
	}

	/************************************************
	* BST COPY CONSTRUCTOR
	* initializes values and calls assignment operator
	*************************************************/
	template<class T>
	BST<T>::BST(const BST<T>& rhs) throw(const char *)
	{
		root = NULL;
		numElements = 0;
		try
		{
			*this = rhs;
		}
		catch (std::bad_alloc)
		{
			throw "ERROR: Unable to allocate a node";
		}
	}

	/************************************************
	* ASSIGNMENT OPERATOR
	* calls copyBinaryTree() to perfor operation
	*************************************************/
	template<class T>
	BST<T>& BST<T>::operator=(const BST<T>& rhs)
	{
		copyBinaryTree(rhs.root, this->root);
		this->numElements = rhs.numElements;
		return *this;
	}

	/*****************************************************
	 * INSERT
	 * creates new node with data value of t.  finds location
	 * to insert node.  balances red/black tree.
	 *****************************************************/
	template<class T>
	void BST<T>::insert(const T & t) throw(const char *)
	{
		BNode* newb = new BNode(t);
		numElements++;

		if (root)
		{
			BNode* tmp;
			tmp = root;
			while (newb->pParent == NULL)
			{
				if (newb->data < tmp->data)
				{
					if (tmp->pLeft)
						tmp = tmp->pLeft;
					else
					{
						tmp->pLeft = newb;
						newb->pParent = tmp;

					}
				}
				else
				{
					if (tmp->pRight)
						tmp = tmp->pRight;
					else
					{
						tmp->pRight = newb;
						newb->pParent = tmp;
					}
				}
			}
		}
		else if (!root)
			root = newb;		
	}

	/*************************************************
	* ERASE
	* erases a single node and rearranges pointers
	**************************************************/
	template<class T>
	void BST<T>::erase(iterator it)
	{
		deleteNode(it.getPNode(), true);
	}

	/************************************************
	* FIND
	* searches BST for a node and returns an iterator
	*************************************************/
	template<class T>
	typename BST<T>::iterator BST<T>::find(const T & t)
	{
		BNode * pNode = root;

		while (pNode != NULL && pNode->data != t )
		{
			if (t > pNode->data)
			{
				pNode = pNode->pRight;
			}
			else if (t < pNode->data)
			{
				pNode = pNode->pLeft;
			}

		}

		return iterator(pNode);
	}

	/************************************************
	* BEGIN
	* returns iterator to beginning location
	*************************************************/
	template<class T>
	typename BST<T>::iterator BST<T>::begin()
	{
		if (!root)
			return NULL;
		BNode* it = root;
		while (it->pLeft)
			it = it->pLeft;
		return BST<T>::iterator(it);
	}

	/************************************************
	* RBEGIN
	* returns iterator to reverse beginning location 
	*************************************************/
	template<class T>
	typename BST<T>::iterator BST<T>::rbegin()
	{
		if (!root)
			return NULL;
		BNode* it = root;
		while (it->pRight)
			it = it->pRight;
		return BST<T>::iterator(it);
	}

	/************************************************
	* DELETE NODE
	* contains logic to delete a single BNode
	*************************************************/
	template<class T>
	void BST<T>::deleteNode(BNode * del, bool right)
	{
		if (del->pRight == NULL && del->pLeft == NULL) //not finished need other conditions
		{
			if (del->pParent != NULL && del->pParent->pRight == del)
			{
				del->pParent->pRight = NULL;
			}
			else if (del->pParent != NULL && del->pParent->pLeft == del)
			{
				del->pParent->pLeft = NULL;
			}
			delete del;
		}
		else if (del->pRight == NULL && del->pLeft != NULL)
		{
			del->pLeft->pParent = del->pParent;
			if (del->pParent != NULL && del->pParent->pRight == del)
				del->pParent->pRight = del->pLeft;
			if (del->pParent != NULL && del->pParent->pLeft == del)
				del->pParent->pLeft = del->pLeft;
			delete del;
		}
		else if (del->pLeft == NULL && del->pRight != NULL)
		{
			del->pRight->pParent = del->pParent;
			if (del->pParent != NULL && del->pParent->pRight == del)
				del->pParent->pRight = del->pRight;
			if (del->pParent != NULL && del->pParent->pLeft == del)
				del->pParent->pLeft = del->pRight;
			delete del;
		}
		else
		{
			BNode* tmp = del->pRight;
			while (tmp->pLeft)
				tmp = tmp->pLeft;
			if (tmp->pRight)
				tmp->pRight->pParent = tmp->pParent;
			tmp->pParent->pLeft = tmp->pRight;
			tmp->pLeft = del->pLeft;
			tmp->pLeft->pParent = tmp;
			tmp->pRight = del->pRight;
			tmp->pRight->pParent = tmp;
			tmp->pParent = del->pParent;
			if (del->pParent)
			{
				if (del->pParent->pRight == del)
					del->pParent->pRight = tmp;
				else
					del->pParent->pLeft = tmp;
			}
			else
				root = tmp;
			delete del;
			del = NULL;
		}


	}

	/************************************************
	* DELETE BINARY TREE
	* deletes all nodes in Binary Tree
	*************************************************/
	template<class T>
	void BST<T>::deleteBinaryTree(BNode*& pNode)
	{
		if (!(pNode))
			return;
		deleteBinaryTree(pNode->pLeft);
		deleteBinaryTree(pNode->pRight);
		delete pNode;
		pNode = NULL;
		numElements = 0;
	}

	/************************************************
	* COPY BINARY TREE
	* contains logic to copy rhs binary tree to lhs
	*************************************************/
	template<class T>
	void BST<T>::copyBinaryTree(BNode * src, BNode*& dest)
	{
		if (!src)
			return;
		dest =  new BNode();
		dest->data = src->data;

		if (src->pLeft)
		{
			copyBinaryTree(src->pLeft, dest->pLeft);
			if (dest->pLeft)
				dest->pLeft->pParent = dest;
		}
		if (src->pRight)
		{
			copyBinaryTree(src->pRight, dest->pRight);
			if (dest->pRight)
				dest->pRight->pParent = dest;
		}
	}

	/************************************************
	* BALANCE
	* contains logic to balance red/black tree
	*************************************************/
	template <class T>
	void BST<T>::BNode::balance()
	{
		BNode* newb;
		//still need to change colors for parts D
		// RED - BLACK balancing this logic may all need to move to BNode::balance()
		if (newb->pParent == NULL) //case 1
		{
			newb->isRed = false;
			root = newb;
		}

		if (newb->pParent->isRed == true)								// parent is red -- case 3 and 4 
		{
			if (newb->pParent->pLeft == newb)								//child left of parent
			{
				if (newb->pParent->pParent->pLeft == newb->pParent)			//parent is left of grandparent		/ shape
				{
					if (newb->pParent->pParent->pRight->isRed == true
						&& newb->pParent->pParent->pRight)	//case 3
					{
						newb->pParent->pParent->pRight->isRed = false;
						newb->pParent->pParent->isRed = true;
						newb->pParent->isRed = false;
					}
					else if (newb->pParent->pParent->pRight->isRed == false
						&& newb->pParent->pParent->pRight)					//case 4a
					{
						BNode* P = newb->pParent;
						BNode* G = P->pParent;
						BNode* S = P->pRight;
						P->pParent = G->pParent;
						if (P < P->pParent)
							P->pParent->pLeft = P;
						else
							P->pParent->pRight = P;
						G->pParent = P;
						P->pRight = G;
						G->pLeft = S;
						S->pParent = G;
					}
				}
				else															//parent is right of grandparent	> shape
				{
					if (newb->pParent->pParent->pLeft->isRed == true
						&& newb->pParent->pParent->pLeft)		//case 3
					{
						newb->pParent->pParent->pLeft->isRed = false;
						newb->pParent->pParent->isRed = true;
						newb->pParent->isRed = false;
					}
					else if (newb->pParent->pParent->pLeft->isRed == false
						&& newb->pParent->pParent->pLeft)						//case 4d
					{
						BNode* L = newb->pLeft;
						BNode* R = newb->pRight;
						BNode* P = newb->pParent;
						BNode* S = P->pRight;
						BNode* G = P->pParent;
						BNode* A = G->pRight;
						newb->pParent = G->pParent;
						if (newb < newb->pParent)
							newb->pParent->pLeft = newb;
						else
							newb->pParent->pRight = newb;
						newb->pLeft = G;
						newb->pRight = P;
						G->pLeft = A;
						G->pRight = L;
						G->pParent = newb;
						P->pLeft = R;
						P->pRight = S;
						P->pParent = newb;
						A->pParent = G;
						L->pParent = G;
						R->pParent = P;
						S->pParent = P;
					}
				}
			}
			else															//child right of parent
			{
				if (newb->pParent->pParent->pLeft == newb->pParent)			//parent is left of grandparent		< shape
				{
					if (newb->pParent->pParent->pRight->isRed == true
						&& newb->pParent->pParent->pRight)//case 3
					{
						newb->pParent->pParent->pRight->isRed = false;
						newb->pParent->pParent->isRed = true;
						newb->pParent->isRed = false;
					}
					else if (newb->pParent->pParent->pRight->isRed == false
						&& newb->pParent->pParent->pRight)					//case 4c
					{
						BNode* L = newb->pLeft;
						BNode* R = newb->pRight;
						BNode* P = newb->pParent;
						BNode* S = P->pLeft;
						BNode* G = P->pParent;
						BNode* A = G->pRight;
						newb->pParent = G->pParent;
						if (newb < newb->pParent)
							newb->pParent->pLeft = newb;
						else
							newb->pParent->pRight = newb;
						newb->pLeft = P;
						newb->pRight = G;
						P->pParent = newb;
						P->pLeft = S;
						P->pRight = L;
						G->pParent = newb;
						G->pLeft = R;
						G->pRight = A;
						S->pParent = P;
						L->pParent = P;
						R->pParent = G;
						A->pParent = G;
					}
				}
				else															//parent is right of grandparent	\ shape
				{
					if (newb->pParent->pParent->pLeft->isRed == true
						&& newb->pParent->pParent->pLeft)//case 3
					{
						newb->pParent->pParent->pLeft->isRed = false;
						newb->pParent->pParent->isRed = true;
						newb->pParent->isRed = false;
					}
					else if (newb->pParent->pParent->pLeft->isRed == false
						&& newb->pParent->pParent->pLeft)						//case 4b
					{
						BNode* P = newb->pParent;
						BNode* G = P->pParent;
						BNode* S = P->pRight;
						P->pParent = G->pParent;
						if (P < P->pParent)
							P->pParent->pLeft = P;
						else
							P->pParent->pRight = P;
						G->pParent = P;
						P->pLeft = G;
						G->pRight = S;
						S->pParent = G;
					}
				}
			}	
		}
	}
};
#endif // !BST_H

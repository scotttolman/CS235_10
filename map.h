#ifndef MAP_H
#define MAP_H

#include "bst.h"
#include "pair.h"
#include <iostream>

namespace custom
{
	/********************************************
	* MAP CLASS
	* contains all methods for map class
	*********************************************/
	template <class K, class V>
	class map
	{
	public:
		class iterator;
		map() { bst = BST<Pair<K, V> >(); }
		map(const map <K, V> & rhs) throw (const char *) { bst = rhs.bst; }
		~map() { bst.clear(); }
		map<K, V>& operator =(const map <K, V> & rhs) throw (const char *);
		int size() { return bst.size(); }
		bool empty() { return bst.empty(); }
		void clear() { bst.clear(); }
		V& operator [] (const K& k) throw (const char *);
		V operator [] (const K & k) const throw (const char *);
		typename map<K, V>::iterator find(const K& k);
		void insert(const Pair<K, V>& input) throw (const char *);
		void insert(const K& k, const V& v) throw (const char *);
		typename map<K, V>::iterator begin() { return iterator(bst.begin()); }
		typename map<K, V>::iterator end() { return iterator(bst.end()); }
	private:
		BST<Pair<K, V> > bst;
	};

	/***********************************************
	* ITERATOR CLASS
	* contains all methods for map<K, V>::iterator
	************************************************/
	template <class K, class V>
	class map<K, V>::iterator
	{
	public:
		iterator();
		iterator(const typename BST<Pair<K, V> >::iterator& rhs);
		iterator(const typename map<K, V>::iterator& rhs);
		map<K, V>::iterator& operator =(const typename map<K, V>::iterator& rhs);
		bool operator == (const typename map<K, V>::iterator rhs) { return (it == rhs.it); }
		bool operator != (const typename map<K, V>::iterator rhs) { return (it != rhs.it); }
		map<K, V>::iterator& operator ++();
		map<K, V>::iterator& operator ++(int postfix);
		map<K, V>::iterator& operator --();
		map<K, V>::iterator& operator --(int postfix);
		Pair<K, V>& operator *() { return *it; }

	private:
		typename BST<Pair<K, V> >::iterator it;
	};

	/**********************************************
	* ASSIGNMENT OPERATOR
	* copies members from rhs, returns *this
	************************************************/
	template<class K, class V>
	map<K, V>& map<K, V>::operator=(const map<K, V>& rhs) throw(const char *)
	{
		bst = rhs.bst;
		return *this;
	}

	/*************************************************
	* ACCESS OPERATOR (BY-REFERENCE)
	* returns value of given key
	***************************************************/
	template<class K, class V>
	V& map<K, V>::operator[](const K & k) throw(const char *)
	{
		Pair<K, V> input = Pair<K, V>(k, V());
		iterator it = bst.find(input);
		if (it != end())
			return (*it).second;
		else
			bst.insert(input);
		return (*it).second;
	}

	/*************************************************
	* ACCESS OPERATOR (BY-VALUE)
	* returns value of given key
	***************************************************/
	template<class K, class V>
	V map<K, V>::operator[](const K & k) const throw(const char *)
	{
		Pair<K, V> input = Pair<K, V>(k, V());
		iterator it = bst.find(input);
		if (it)
			return it.second;
		else
			bst.insert(input);
		return bst.find(input).second;
	}

	template<class K, class V>
	typename map<K, V>::iterator map<K, V>::find(const K & k)
	{
		return map<K, V>::iterator(bst.find(Pair<K, V>(k, V())));
	}

	/************************************************
	* INSERT (PAIR)
	* updates value of key, or creates new node
	**************************************************/
	template<class K, class V>
	void map<K, V>::insert(const Pair<K, V>& input) throw(const char *)
	{
		iterator it = bst.find(input);
		if (it)
			*it = input;
		else
			bst.insert(input);
	}

	/************************************************
	* INSERT (KEY, VALUE)
	* updates value of key, or creates new node
	**************************************************/
	template<class K, class V>
	void map<K, V>::insert(const K & k, const V & v) throw(const char *)
	{
		Pair<K, V> input = Pair<K, V>(k, v);
		iterator it = bst.find(input);
		if (it)
			*it = input;
		else
			bst.insert(input);
	}

	template<class K, class V>
	typename map<K, V>::iterator& map<K, V>::iterator::operator ++()
	{
		++it;
		return *this;
	}

	template<class K, class V>
	typename map<K, V>::iterator& map<K, V>::iterator::operator ++(int postfix)
	{
		map<K, V>::iterator tmp = *this;
		++it;
		return tmp;
	}

	template<class K, class V>
	typename map<K, V>::iterator& map<K, V>::iterator::operator --()
	{
		--it;
		return *this;
	}

	template<class K, class V>
	typename map<K, V>::iterator& map<K, V>::iterator::operator --(int postfix)
	{
		map<K, V>::iterator tmp = *this;
		--it;
		return tmp;
	}
}

#endif // !MAP_H

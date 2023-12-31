#ifndef UTIL_H
#define UTIL_H

#include "Graph.h"
#include "QISeqEntry.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
using namespace QuickSI;

namespace std {

template <>
class hash<pair<Tag, int>> {
public:
	size_t operator()(const pair<Tag, int> &key) const;
};

template <class T>
class hash<pair<T, T>> {
public:
	size_t operator()(const pair<T, T> &key) const;
};

template <class T>
class hash<vector<T>> {
	const size_t seed;

public:
	hash(size_t seed);

	size_t operator()(const vector<T> &key) const;
};

template <class T>
class hash<unordered_multiset<T>> {
	const size_t seed;

public:
	hash(size_t seed);

	size_t operator()(const unordered_multiset<T> &key) const;
};

template <>
class hash<QISeqEntry> {
public:
	size_t operator()(const QISeqEntry &key) const;
};

template <>
class hash<Tree> {
public:
	size_t operator()(const Tree &key) const;
};

} // namespace std

template <class ContainerType, class Pred>
bool find_if(const ContainerType &v, Pred f) {
	return std::find_if(v.begin(), v.end(), f) != v.end();
}

bool connected(const Node &v, int u);

template <class InputIterator1, class InputIterator2, class OutputIterator, class CompareLess, class CompareGreater>
OutputIterator Set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, CompareLess less, CompareGreater greater) {
	while (first1 != last1 && first2 != last2) {
		if (less(*first1, *first2)) ++first1;
		else if (greater(*first1, *first2)) ++first2;
		else {
			*result = *first2;
			++result;
			++first1;
			++first2;
		}
	}
	return result;
}

int getfa(std::vector<int> &fa, int x);

#endif
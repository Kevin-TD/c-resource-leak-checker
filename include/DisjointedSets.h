#ifndef DISJOINTED_SETS_H
#define DISJOINTED_SETS_H

#include "DisjointedSetWrapper.h"

template <typename T> class DisjointedSets {
private:
  std::list<DisjointedSetWrapper<T>> sets;

  // finds the original DisjoinedSetWrapper element belongs to as an iterator
  typename std::list<DisjointedSetWrapper<T>>::iterator findIter(T element);

  // puts the elements of set2 into set1 in constant time. it also deletes set2.
  void merge(typename std::list<DisjointedSetWrapper<T>>::iterator set1,
             typename std::list<DisjointedSetWrapper<T>>::iterator set2);

public:
  // returns true iff element is in any of the disjointed sets
  bool elementIsInAnySet(T element);

  // returns a copy of every disjointed set
  std::list<DisjointedSetWrapper<T>> getSets();

  // returns the set belonging to element. an empty set is returned if it's not
  // found.
  DisjointedSetWrapper<T> getSet(T element);

  // unions sets A and B. if A == B, or elementA or elementB are not actually
  // elements of any set, nothing happens.
  void unionSets(T elementA, T elementB);

  // creates a new disjoint set if it's already not an element in any of the
  // existing sets.
  void makeSet(T element);

  // creates new disjoint set consisting of element1 and element2 if both
  // element1 and element2 aren't elements in any of the existing sets.
  void makeSetOfTwo(T element1, T element2);
};

#endif
#include "DisjointedSets.h"
#include "Debug.h"

template <typename T>
typename std::list<DisjointedSetWrapper<T>>::iterator
DisjointedSets<T>::findIter(T element) {
  for (auto it = sets.begin(); it != sets.end(); ++it) {
    if (it->contains(element)) {
      return it;
    }
  }

  return sets.end();
}

template <typename T>
void DisjointedSets<T>::merge(
    typename std::list<DisjointedSetWrapper<T>>::iterator set1,
    typename std::list<DisjointedSetWrapper<T>>::iterator set2) {
  if (set1 != sets.end() && set2 != sets.end()) {
    set1->elements.splice(set1->elements.end(), set2->elements);
    sets.erase(set2);
  }
}

template <typename T> bool DisjointedSets<T>::elementIsInAnySet(T element) {
  return findIter(element) != sets.end();
}

template <typename T>
std::list<DisjointedSetWrapper<T>> DisjointedSets<T>::getSets() {
  return sets;
}

template <typename T>
DisjointedSetWrapper<T> DisjointedSets<T>::getSet(T element) {
  for (DisjointedSetWrapper<T> set : sets) {
    if (set.contains(element)) {
      return set;
    }
  }

  return DisjointedSetWrapper<T>();
}

template <typename T>
void DisjointedSets<T>::unionSets(T elementA, T elementB) {
  auto set1 = findIter(elementA);
  auto set2 = findIter(elementB);

  if (set1 == set2 || set1 == sets.end() || set2 == sets.end()) {
    return;
  }

  merge(set1, set2);
}

template <typename T> void DisjointedSets<T>::makeSet(T element) {
  if (elementIsInAnySet(element)) {
    return;
  }

  DisjointedSetWrapper<T> newSet;
  newSet.add(element);
  sets.push_back(newSet);
}

template <typename T>
void DisjointedSets<T>::makeSetOfTwo(T element1, T element2) {
  if (elementIsInAnySet(element1) || elementIsInAnySet(element2)) {
    return;
  }

  DisjointedSetWrapper<T> newSet;
  newSet.add(element1);
  newSet.add(element2);
  sets.push_back(newSet);
}
#ifndef DISJOINTED_SET_WRAPPER_H
#define DISJOINTED_SET_WRAPPER_H

#include "Utils.h"

template <typename T> class DisjointedSetWrapper {
private:
  std::list<T> elements;

public:
  DisjointedSetWrapper();

  // returns true iff element is in the set
  bool contains(T element);

  // adds element to elements. this method does not prevent duplicates from
  // being added.
  void add(T element);

  std::list<T> getElements();

  template <typename U> friend class DisjointedSets;
};

#endif
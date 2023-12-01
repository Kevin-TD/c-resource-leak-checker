#include "DisjointedSetWrapper.h"

template <typename T> DisjointedSetWrapper<T>::DisjointedSetWrapper() {}

template <typename T> bool DisjointedSetWrapper<T>::contains(T element) {
  for (T el : elements) {
    if (el == element) {
      return true;
    }
  }

  return false;
}

template <typename T> void DisjointedSetWrapper<T>::add(T element) {
  elements.push_back(element);
}

template <typename T> std::list<T> DisjointedSetWrapper<T>::getElements() {
  return elements;
}
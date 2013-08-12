#ifndef ORG_KNORBA_COMMON_ARRAY_H
#define ORG_KNORBA_COMMON_ARRAY_H

#include "ManagedPointer.h"

#include <vector>

using namespace std;

template<class T>
class MutableArray {
private:
  vector<ManagedPointer<T>*> _items;

public:
  MutableArray() {
    // Nothing
  }

  ~MutableArray() {
    clear();
  }

  void push(ManagedPointer<T>* obj) {
    _items.push_back(obj->retain());
  }

  ManagedPointer<T>* pop() { 
    return _items.pop_back()->release();
  }

  void insert(int index, ManagedPointer<T>* obj) {
    _items.insert(_items.begin() + index, obj->retain());
  }

  void remove(int index) {
    _items[index]->release();
    _items.erase(_items.begin() + index);
  }

  ManagedPointer<T>* get(int index) {
    return _items[index];
  }

  void set(int index, ManagedPointer<T>* obj) {
    _items[index]->release();
    _items[index] = obj->retain();
  }

  void clear() {
    for(int i = _items.size() - 1; i >= 0; i--) {
      _items[i]->release();
    }
    _items->clear();
  }
};
#endif

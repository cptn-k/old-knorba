#ifndef ORG_KNORBA_COMMON_MANAGEDPOINTER_H
#define ORG_KNORBA_COMMON_MANAGEDPOINTER_H

#ifdef K_NO_MANAGED_POINTERS
#  define _X_ ->
#  define K_NEW(X, Y) (new X##Y)
#else
#  define K_NEW(X, Y) (new ManagedPointer<X>(new X##Y))
#  ifdef K_FAST_MANAGED_POINTERS
#    define _X_ ->getPtr()->
#  else
#    define _X_ ->getPtr(__FILE__, __LINE__)->
#  endif
#endif

template<class T>
class ManagedPointer {
private:

#ifndef K_FAST_MANAGED_POINTERS
  static vector<ManagedPointer*> _allManagedPointers;
#endif

  int _retainCount;
  T*  _obj;

public:
  ManagedPointer(T* obj) {
    _retainCount = 0;
    _obj = obj;
#ifndef K_FAST_MANAGED_POINTERS
    _allManagedPointers.push_back(this);
#endif
  }

  ~ManagedPointer() {
    if(_obj != 0)
      delete _obj;
  }

  ManagedPointer<T>* retain() {
    _retainCount++;
    return this;
  }

  ManagedPointer<T>* release() {
    _retainCount--;
    if(_retainCount < 0) {
      delete _obj;
      _obj = 0;
    }
    return this;
  }

  ManagedPointer<T>* release(char fileName[], int lineNumber) {
    if(_obj == 0) {
      cerr << '[' << fileName << ":" << lineNumber 
           << "] object being released is already deleted." << endl;
      return;
    }
    release();
    return this;
  }

  int getRetainCount() {
    return _retainCount;
  }

  bool isAlive() {
    return _obj != 0;
  }

  T* getPtr() {
    if(_obj == 0) {
      cerr << "Attempt to access a deleted object." << endl;
      exit(EXIT_FAILURE);
    }
    return _obj;
  }

  T* getPtr(char fileName[], int lineNumber) {
    if(_obj == 0) {
      cerr << '[' << fileName << ":" << lineNumber 
           << "] attempt to access a deleted object" << endl;
      exit(EXIT_FAILURE);
    }
    return _obj;
  }

  T* operator->() {
    return _obj;
  }

  static void clearGarbage();
  static void clearAllManagedPointers();
};

#endif

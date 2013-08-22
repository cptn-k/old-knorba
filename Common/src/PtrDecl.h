#ifndef ORG_KNORBA_COMMON_MANAGEDPOINTER_H
#define ORG_KNORBA_COMMON_MANAGEDPOINTER_H

#include <string>

#include "ObjectPool.h"
#include "ManagedObject.h"
#include "Integer.h"
#include "NullPointerException.h"
#include "InvalidPointerException.h"

#ifdef K_FAST_POINTERS
#  define _X_ ->
#  define _X(A) (*A)
#else
#  define _X(A) A.obj(__FILE__, __LINE__)
#  define _X_ .obj(__FILE__, __LINE__).
#endif

using namespace std;

namespace org {
namespace knorba {
namespace common {

class ManagedObject;

template<typename T>
class Ptr {
private:
  T*  _obj;
  
public:
  Ptr(T* obj);
  ~Ptr();

  void retain() const;
  void release() const;

  int  retainCount() const;
  bool isAlive() const;

  T& obj() const throw(MemoryException);
  T& obj(string fileName, int lineNumber) const throw(MemoryException);
  T& operator*() const;
  T* toPurePtr() const;
  T* operator->() const;
};

} // namespace common
} // namespace knorba
} // namespace org

#endif



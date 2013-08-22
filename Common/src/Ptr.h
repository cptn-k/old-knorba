#ifndef ORG_KNORBA_COMMON_MANAGEDPOINTER_DECL_H
#define ORG_KNORBA_COMMON_MANAGEDPOINTER_DECL_H

#include "PtrDecl.h"

namespace org {
namespace knorba {
namespace common {


template<typename T>
inline Ptr<T>::Ptr(T* obj) {
  _obj = obj;
}

template<typename T>
inline Ptr<T>::~Ptr() {
  // Nothing
}

template<typename T>
inline void Ptr<T>::retain() const {
  if(!_obj)
    throw NullPointerException("");

  ::org::knorba::common::retain(_obj);
}

template<typename T>
inline void Ptr<T>::release() const {
  if(!_obj)
    throw NullPointerException("");

  ::org::knorba::common::release(_obj);
}
 
template<typename T>
int Ptr<T>::retainCount() const {
  if(!_obj)
    throw NullPointerException("");

  return ObjectPool::getRootObjectPool()->getRetainCountFor(_obj);
}

template<typename T>
inline bool Ptr<T>::isAlive() const {
  if(!_obj)
    throw NullPointerException("");

  return (retainCount() >= 0);
}

template<typename T>
inline T& Ptr<T>::obj() const throw(MemoryException) {
  if(!_obj)
    throw NullPointerException("");

  if(!isAlive()) {
    throw InvalidPointerException("Error attempting to access an unmanaged or "
            "deleted object.");
  }

  return *_obj;
}

template<typename T>
inline T& Ptr<T>::obj(string fileName, int lineNumber) const
    throw(MemoryException)
{
  if(!_obj)
    throw NullPointerException("");

  if(!isAlive()) {
    throw InvalidPointerException(string("At [") + fileName + ", " 
            + Integer(lineNumber) + "] error attempting to access an "
            "unmanaged or deleted object.");
  }

  return *_obj;
}

template<typename T>
inline T& Ptr<T>::operator*() const {
  return obj();
}

template<typename T>
inline T* Ptr<T>::toPurePtr() const {
  if(!_obj)
    throw NullPointerException("");

  if(!isAlive()) {
    throw InvalidPointerException("Error attempting access to an unmanaged or "
            "deleted object.");
  }

  return _obj;
}

template<typename T>
inline T* Ptr<T>::operator->() const {
  if(!_obj)
    throw NullPointerException("");

  if(!(isAlive()))
    throw InvalidPointerException("Error attempting access to an unmanaged or "
            "deleted object.");

  return _obj;
}

} // namespace common
} // namespace knorba
} // namespace org

#endif



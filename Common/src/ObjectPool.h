#ifndef ORG_KNORBA_COMMON_OBJECTPOOL_H
#define ORG_KNORBA_COMMON_OBJECTPOOL_H

#include "common.h"
#include "ManagedObject.h"

namespace org {
namespace knorba {
namespace common {

class ObjectPool : public ManagedObject {
private:
  typedef map<ManagedObject*, int> ObjectMap;
  static ObjectPool* _root = 0;
  ObjectMap _retainCount;

public:
  static const ObjectPool& getRootObjectPool() {
    if(_root = 0) {
      _root = new ObjectPool();
    }

    return _root;
  }

  ObjectPool();
  ~ObjectPool();

  ManagedObject* retain(ManageObject* obj);
  ManagedObject*& release(ManagedObject*& obj, char fileName[], 
                          int lineNumber);
  ManagedObject* release(ManagedObject* obj);
  int getRetainCountOf(ManagedObject* obj);
  
  void clearGarbage();
  void clearAll();
};

} // namespace common
} // namespace knorba
} // namespace org

#endif


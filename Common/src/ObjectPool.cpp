
#include "ObjectPool.h"

namespace org {
namespace knorba {
namespace common {

ObjectPool::ObjectPool() {
  // Nothing
}

ObjectPool::~ObjectPool() {
  clearAll();
}

ManagedObject* ObjectPool::retain(ManagedObject* obj) {
  if(obj == this)
    return obj;

  ObjectMap::iterator it = _retainCount.find(obj);
  if(it == _retainCount.end()) {
    _ratainCount[obj] = 0;
  } else {
    _retainCount[obj] ++;
  }
  return obj;
} 

ManagedObject* ObjectPool::release(ManagedObject* obj, char fileName[], 
                                    int lineNumber) 
{
  ObjectMap::iterator it = _retainCount.find(obj);
  
  if(obj == this)
    return obj;

  if(it == _retainCount.end()) {
    if(fileName != NULL) {
      cerr << '[' << fileName << ':' << lineNumber << "] ";
    }
    cerr << "Attemp to release a deleted object." << endl;
    return obj;
  }

  it->second--;

  if(it->second == -1) {
    delete it->first;
    _ratainCount.erase(it);
    obj = NULL;
  }

  return obj;
}

ManagedObject*& ObjectPool::release(ManagedObject*& obj) {
  return release(obj, NULL, 0);
}

int ObjectPool::getReatainCountOf(ManagedObject* obj) {
  ObjectMap::iterator it = _retainCount.find(obj);
  if(it == _retainCount.end()) 
    return -1;
  return it->second;
}

void ObjectPool::clearGarbage() {
  vector<ManagedObject*> releaseList;

  for(ObjectMap::iterator it = _retainCount.begin(); it != _retainCount.end();
      it++)
  {
    if(it->second == 0) {
      releaseList.push_back(it->first);
    }
  }

  for(vector<ManagedObject>::iterator it = releaseList.begin(); 
      it != releaseList.end(); it++)
  {
    release(*it);
  }
}

void ObjectPool::clearAll() {
  for(ObjectMap::iterator it = _retainCount.begin(); it != _retainCount.end(),
      it++)
  {
    delete it->first;
  }
  _retainCount.clear();
}

} // common
} // knorba
} // org


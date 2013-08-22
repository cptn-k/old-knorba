
#include <iostream>

#include "org/knorba/common/ManagedObject.h"
#include "org/knorba/common/ObjectDumpBuilder.h"
#include "org/knorba/common/Ptr.h"

using namespace std;
using namespace org::knorba::common;

void build(ObjectDumpBuilder* builder) {
  retain(builder);
  gc();
  builder->object("TestObject")
    ->attribute("TEST_ATTRIBUTE")
  ->endObject();
  release(builder);
}

int main(int argc, char** argv) {
  try {
    Ptr<ObjectDumpBuilder> ptr(0);
    cout << ptr.toPurePtr() << endl;
  } catch(KSystemException& e) {
    LOG << e << EL;
  }
}

#ifndef ORG_KNORBA_COMMON_SERIALIZINGSTREAMER_H
#define ORG_KNORBA_COMMON_SERIALIZINGSTREAMER_H

#include <ostream>

#include "Streamer.h"
#include "PtrDecl.h"
#include "ObjectDumpBuilder.h"

using namespace std;

namespace org {
namespace knorba {
namespace common {

class SerializingStreamer : public Streamer {
public:
  virtual ~SerializingStreamer() {
    // Nothing
  }
  virtual void serialize(Ptr<ObjectDumpBuilder> builder) const = 0;
  virtual void printToStream(ostream& os) const {
    Ptr<ObjectDumpBuilder> builder(
        new ObjectDumpBuilder(os, ObjectDumpBuilder::DUMP, 4));
    builder.retain();
    serialize(builder); 
    builder.release();
  }
};

} // common
} // knorba
} // org

#endif

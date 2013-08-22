
#include "ManagedObject.h"

namespace org {
namespace knorba {
namespace common {

ManagedObject::ManagedObject() {
  retain(this);
}

ManagedObject::~ManagedObject() {
  // Nothing
}

} // common
} // knorba
} // org

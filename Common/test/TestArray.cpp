
#include <string>
#include <iostream>

#include "org/knorba/common/common.h"
#include "org/knorba/common/ManagedObject.h"
#include "org/knorba/common/Array.h"
#include "org/knorba/common/Ptr.h"

#define S(X) new ManagedString(X)

using namespace std;
using namespace org::knorba::common;

class ManagedString : public ManagedObject, public Streamer {
private:
  string _value;
public:
  ManagedString(const string& str) {
    _value = str;
  }

  virtual void printToStream(ostream& os) const {
    os << _value;
  }

  virtual string toString() const {
    return _value;
  }
};

int main(int argc, char** argv) {
  try {
    Ptr< MutableArray<ManagedString> >
                                      array(new MutableArray<ManagedString>());
    array _X_ push(S("Hello"));
    array _X_ push(S("World"));
    array _X_ push(S("This shouldn't be in the array"));
    cout << _X(array _X_ pop()) << endl;
    cout << *array << endl;
  } catch(KSystemException& e) {
    LOG << e << EL;
  }
}

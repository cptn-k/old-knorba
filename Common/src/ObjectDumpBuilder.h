#ifndef ORG_KNORBA_COMMON_OBJECTDUMPBUILDER_H
#define ORG_KNORBA_COMMON_OBJECTDUMPBUILDER_H

#include "KSystemException.h"

#include <string>
#include <vector>

using namespace std;

namespace org {
namespace knorba {
namespace common {

class ObjectDumpBuilderException : public KSystemException {
  // Nothing
}

class ObjectDumpBuilder {
public:
  typedef enum {
    DUMP,
    XML,
    JSON
  } output_type_t;

private:
  typedef enum {
    ROOT,
    MEMBER,
    OBJECT,
    OBJECT_BODY,
    PROPERTY,
    VALUE,
    COLLETION
  } state_t;

  typedef enum {
    NUMBER,
    CHAR,
    STRING,
    NONE
  } value_type_t;

  class StackItem {
  public:
    state_t _state;
    string  _name;
    bool    _isLead;
    StackItem(state_t state, string name, bool isLead)
      : _state(state), _name(name), _isLead(isLead)
    { 
      // Nothing
    }
  };

private:
  output_type_t _outputType;
  state_t       _state;
  ostream&      _stream;
  bool          _isLead;
  vector<StackItem> _stack;

  ObjectDumpBuilder* attribute(const string& name, const string& value, 
                                                            value_type_t type);

public:
  ObjectDumpBuilder(ostream& stream, output_type_t outputType)
    : _stream(stream), _state(ROOT), _outputType(outputType)
  {
    // Nothing
  }
  
  ObjectDumpBuilder* member(const string& name);
  ObjectDumpBuilder* member(const string& name, const string& text);
  ObjectDumpBuilder* object(string ClassName);
  ObjectDumpBuilder* endObject();
  ObjectDumpBuilder* text(const string& value);

  ObjectDumpBuilder* attribute(const string& name, const string& value) {
    return attribute(name, value, STRING);
  }

  ObjectDumpBuilder* attribute(const string& name, char value) {
    return attribute(name, string(&value, 1), CHAR);
  }

  ObjectDumpBuilder* attribute(const string& name, int value) {
    return attribute(name, to_string(value), NUMBER);
  }

  ObjectDumpBuilder* attribute(const string& name, long int value) {
    return attribute(name, to_string(value), NUMBER);
  }

  ObjectDumpBuilder* attribute(const string& name, double value) {
    return attribute(name, to_string(value), NUMBER);
  }

  ObjectDumpBuilder* collection(string name);
  ObjectDumpBuilder* endCollection();
};

} // common
} // knorba
} // org

#endif

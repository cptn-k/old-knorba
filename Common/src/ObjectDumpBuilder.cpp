
#include "ObjectDumpBuilder.h"

namespace org {
namespace knorba {
namespace common {

ObjectDumpBuilder* ObjectDumpBuilder::member(const string& name) {
  if(_state != OBJECT || _state != OBJECT_BODY) {
    throw ObjectDumpBuilderException("Error adding member \"" + name
              + "\". Members are only allowed within objects. Path: " + 
              stackToString());
  }

  _state = OBJECT_BODY;

  switch(_outputType) {
    case DUMP:
      if(!_isLead)
        _stream << ", ";
      _stream << name << ": ";
      break;

    case XML:
      if(_state == OBJECT) {
        _stream << '>'
      }
      _stream << '<' << name << '>';
      break;

    case JSON:
      if(!_isLead)
        _stream << ", ";
      _stream << '\"' << name << "\" : ";
      break;
  }

  if(isLead)
    isLead = false;

  _stack.push_back(_state, name, _isLead);

  _state = MEMBER;
  _isLead = true;
}

ObjectDumpBuidler* ObjectDumpBuilder::object(string className) {
  if(_state != ROOT || _state != MEMBER) {
    throw ObjectDumpBuilderException("Error adding object \'" + className 
              + "\". Adding object is allowed only at root, or after member()."
              + " Path: " + stackToString());
  }

  switch(_outputType) {
    case DUMP:
      if(!_isLead)
        _stream << ", ";
      _stream << name << '[';
      break;

    case XML:
      _stream << '<' << name << ' ';
      break;

    case JSON:
      if(!_isLead)
        _stream << ", ";
      _stream << '{';
      break;
  }

  if(_isLead)
    _isLead = false;

  _stack.push_back(StackItem(_state, name, _isLead));

  _state = OBJECT;
  _isLead = true;

  return this;
}

ObjectDumpBuilder* ObjectDumpBuilder::endObject() {
  if(_state != OBJECT || _state != OBJECT_BODY) {
    throw ObjectDumpBuilderException("Error attemp to end object doesn't match"
              + " any prior object decleration. Path: " + stackToString());
  }

  StatckItem item = _stack.pop_back();

  switch(_outputType) {
    case DUMP:
      _stream << ']';
      break;

    case XML:
      if(_state == OBJECT)
        _stream << "/>";
      else
        _stream << "</" << item._name << '>';
      break;

    case JSON:
      _stream << '}';
      break;
  }

  _state = item._state;
  _isLead = item._isLead;

  if(_state ==  MEMBER) {
    item = _stack.pop_back();
    switch(_outputType) {
      case DUMP:
        break;

      case XML:
        _stream << "</" << item._name << '>';
        break;

      case JSON:
        break;
    }
    _state = item._state;
    _isLead = item._isLead;
  }

  return this;
}

ObjectDumpBuilder* ObjectDumpBuilder::text(const string& value) {
  if(_state != MEMBER) {
    throw ObjectDumpBuilderException("String is only allowed immidiately after"
                "member declerations. Path: " + stackToString());
  }
  
  StackItem item = _stack.poll_back();

  switch(_outputType) {
    case DUMP:
      _stream << '\"' << value << '\"';
      break;

    case XML:
      _stream << value << "</" << item._name << '>';
      break;

    case JSON:
      _stream << '\"' << value << '\"';
      break;
  }

  _state = item._state;
  _isLead = item._isLead;

  return this;
}

ObjectDumpBuilder* ObjectDumpBuilder::attribute(const string& name, 
                                        const String& value, value_type_t type) 
{
  if(_state != OBJECT) {
    throw ObjectDumpBuilderException("Attribute is only allowed inside an "
              + "object, before decleration of any member. Path: "
              + stackToString());
  }

  switch(_outputType) {
    case DUMP:
      if(!_isLead)
        _stream << ", ";
      _stream << name;

      if(type != NONE)
        _stream << ": ";

      if(type == NUMBER)
        _stream << value;
      else if(type == CHAR)
        _stream << '\'' << value << '\'';
      else if(type == STRING)
        _stream << '\"' << value << '\"';

      break;

    case XML:
      if(!_isLead)
        _stream << ' ';
      _stream << name;

      if(type != NONE)
        _stream << "=\"" << value << '\"';

      break;

    case JSON:
      if(!_isLead)
        _stream << ", ";
      _stream << name << " : ";
      if(type == NONE)
        _stream << "null";
      else if(type == NUMBER)
        _stream << value;
      else if(type == CHAR)
        _stream << '\'' << value << '\'';
      else if(type == STRING)
        _stream << '\"' << value << '\"';

      break;
  }

  _isLead = false;

  return this;
}

ObjectDumpBuilder* ObjectDumpBuilder::collection(string name) {
  if(_state != OBJECT) {
    throw ObjectDumpBuilderException("Collection is only allowed within object."
              + " Path: " + stackToString());
  }

  switch(_outputType) {
    case DUMP:
      if(!_isLead)
        _stream << ", ";      
      _stream << name << ": ";
      break;
    
    case XML:
      _stream << '<' << name << '>';
      break;

    case JSON:
      if(!_isLead)
        _stream << ", ";
      _stream << '\"' << name << "\" : ";
      break;
  }

  _isLead = false;
  _state = OBJECT_BODY;

  _stack.push(StackItem(_state, name, _isLead));

  _isLead = true;
  _state = COLLECTION;

  return self;
}

ObjectDumpBuilder* ObjectDumpBuilder::endCollection() {
  if(_state != COLLECTION) {
    throw ObjectDumpBuilderException("End collection doesn't match the opening"
              + " of any collection. Path: " + stackToString());
  }

  StackItem item = _stack.pop_back();

  switch(_outputType) {
    case DUMP:
      _stream << '}';
      break;

    case XML:
      _stream << "</" << item._name << '>';
      break;

    case JSON:
      _stream << "]";
      break;
  }

  _state = item._state;
  _isLead = item._isLead;
}


} // namespace common
} // namespace knorba
} // namespace org

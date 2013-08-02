#include "PragmaParticle.h"

namespace org {
namespace knorba {
namespace cxxpreprocessor {

int PragmaParticle::_counter = 1;

PragmaParticle::PragmaParticle(SourceManager* sm) 
: SourceRangeEx(sm)
{
  dimensions = new vector<string>();
  _syncBlocks = new vector<SynchronizedBlock*>();
  _id = _counter;
  _counter++;
  stringstream stream;
  stream << "__k_particle_" << _id << "_thread";
  _threadName = stream.str();
}

PragmaParticle::~PragmaParticle() {
  delete dimensions;
}

void PragmaParticle::setType(particle::type t) {
  this->type = t;
}

particle::type PragmaParticle::getType() {
  return type;
}


int PragmaParticle::getNumberOfDimensions() {
  return dimensions->size();
}

string PragmaParticle::getDimensionName(int index) {
  return dimensions->at(index);
}

void PragmaParticle::addDimension(string label) {
  dimensions->insert(dimensions->end(), label);
}

void PragmaParticle::addSynchronizedBlock(SynchronizedBlock* syncBlock) {
  _syncBlocks->insert(_syncBlocks->end(), syncBlock);
}

int PragmaParticle::getNumberOfSynchronizedBlocks() {
  return _syncBlocks->size();
}

SynchronizedBlock* PragmaParticle::getSynchronizedBlock(int index) {
  return (*_syncBlocks)[index];
}

string PragmaParticle::getThreadName() {
  return _threadName;
}

void PragmaParticle::printToStringStream(stringstream& stream) {
  stream << "PragmaParticle dimentions: [";

  int l = getNumberOfDimensions();
  for(int i = 0; i < l; i++) {
    stream << getDimensionName(i);
    if(i < l - 1)
      stream << ", ";
  }

  stream << "] synchronizedBlocks: [";

  l = getNumberOfSynchronizedBlocks();
  for(int i = 0; i < l; i++) {
    getSynchronizedBlock(i)->printToStringStream(stream);
    if(i < l - 1)
      stream << ", ";
  }

  stream << "], range: ";
  SourceRangeEx::printToStringStream(stream);
}

} // cxxpreprocessor
} // knorba
} // org

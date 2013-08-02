/*---[Preprocessor.cpp]---------------------------------------m(._.)m--------*\
|
|  KnoRBA CXX Preporcessor agent.
|
|  Copyright (c) 2013, RIKEN (The Institute of Physical and Chemial Research)
|  All rights reserved.
|
|  Author: Hamed KHANDAN (hkhandan@ieee.org)
|
|  This file is distributed under the KnoRBA Free Public License. See
|  LICENSE.TXT for details.
|
*//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "clang/Frontend/FrontendAction.h"

#include "ClangProxy.h"


using namespace std;
using namespace clang;
using namespace org::knorba::cxxpreprocessor;


#pragma knorba package org.knorba.core
#pragma knorba agent CXX-Preprocessor

#pragma knorba using service org.knorba.core.io.StreamSource \
                         and com.company.something \
                         and info.site.someotherstuff

//----------------------------------------------------------------------
#pragma knorba service org.knorba.lang.cxx

#pragma knorba relation process(in id[StreamSource] src, \
          out id[StreamSource] result)
/*
truth expand(VRecord* q, TruthCaseList* a) {
  // do stuff
  return T;
}
*/
#pragma knorba end service
//----------------------------------------------------------------------



int main(int argc, char** argv) {
  cout << "This is kprep." << endl;
  if(argc <= 1) {
    cout << "Insufficient arguments." << endl;
    return -1;
  }

  ParserProxy* parserProxy = new ParserProxy(string(argv[1]));
  delete parserProxy;
}


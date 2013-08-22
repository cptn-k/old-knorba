
#include <iostream>
#include "org/knorba/common/ObjectDumpBuilder.h"
#include "org/knorba/common/Ptr.h"
#include "org/knorba/common/Logger.h"


using namespace std;
using namespace org::knorba::common;


void buildDump(ObjectDumpBuilder* builder) {
  builder->object("Glossary")
    ->attribute("title", "Example Glossary")
    ->member("glossDiv")->object("GlossDiv")
      ->attribute("title", 'S')
      ->collection("glossList")
        ->object("GlossEntry")
          ->attribute("id", "SGML")
          ->attribute("sortAs", "SGML")
          ->member("glossTerm")->text("Standard Generalized Markup Language")
          ->member("acronym")->text("SGML")
          ->member("abbrev")->text("ISO 8879:1986")
          ->member("glossDef")->object("GlossDef")
            ->member("para")->text("A meta-markup language, used to create"
                                   " markup languages such as DockBook.")
            ->collection("seeAlso")
              ->object("GlossSeeAlso")
                ->attribute("otherTerm", "GML")
              ->endObject()
              ->object("GlossSeeAlso")
                ->attribute("otherTerm", "XML")
              ->endObject()
            ->endCollection()
          ->endObject()
        ->endObject()
      ->endCollection()
    ->endObject()
  ->endObject();
}

int main(int argc, char** argv) {
  try {
    buildDump(new ObjectDumpBuilder(cout, ObjectDumpBuilder::XML, 4));
    cout << endl;
    buildDump(new ObjectDumpBuilder(cout, ObjectDumpBuilder::DUMP, 4));
    cout << endl;
    buildDump(new ObjectDumpBuilder(cout, ObjectDumpBuilder::JSON, 4));
  } catch(KSystemException& e) {
    LOG_ERR << e << EL;
  }
}

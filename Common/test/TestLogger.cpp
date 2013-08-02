
#include "org/knorba/common/Logger.h"

using namespace org::knorba::common;

int main(int argc, char** argv) {
  Logger::getDefaultLogger().addChannel("logfile.log");
  Logger::getDefaultChannel().setLevel(Logger::L2);

  LOG_L2 << "Logger can log int: " << 1234 << ", char: " << 'c'
         << " and double: " << 3.14 << EL;

  LOG_L3 << "This should go to file, but not on screen" << EL;
}


#include "org/knorba/common/common.h"

#ifdef K_UNIX
#  include <unistd.h>
#else
#  error "Only Unix/Linux is supported"
#endif

#include "org/knorba/common/Logger.h"
#include "org/knorba/common/Timer.h"

using namespace org::knorba::common;

int main(int argc, char** argv) {
  Logger::getDefaultLogger()->addChannel("logfile.log");
  Logger::getDefaultChannel()->setLevel(Logger::L2);

  LOG_L2 << "Logger can log int: " << 1234 << ", char: " << 'c'
         << " and double: " << 3.14 << EL;

  LOG_L3 << "This should go to file, but not on screen" << EL;

  Timer t("Test");
  LOG_L2 << t << EL;
  t.start();
  LOG_L2 << "Waiting for 1.1 seconds ..." << EL;
  usleep(1100000);
  LOG_L2 << t << EL;
}

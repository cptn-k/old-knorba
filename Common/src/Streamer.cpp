/*---[Streamer.cpp]-------------------------------------------m(._.)m--------*\
|
|  KnoRBA Common / Streamer
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

/**
 *
 * @file Implementation of `org::knorba::common::Streamer` class.
 *
 */

#include "Streamer.h"

namespace org {
namespace knorba {
namespace common {

string Streamer::toString() {
  stringstream stream;
  this->printToStream(stream);
  return stream.str();
}

} // namespace common
} // namespace knorba
} // namespace org

#ifndef ORG_KNORBA_COMMON_COMMON_H
#define ORG_KNORBA_COMMON_COMMON_H

#ifdef sun
  #undef sun
  #ifndef __sun
    #define __sun
  #endif
#endif

#if defined(__linux__) || defined(__APPLE__) || defined(__sun) || defined(__AIX)
  #define K_UNIX
#endif

#ifdef K_UNIX
  #define K_PATH_SEPARATOR_CH '/'
#endif

#ifndef NULL
  #define NULL 0
#endif

#endif

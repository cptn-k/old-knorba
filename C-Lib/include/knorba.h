/*---[knorba.h]-----------------------------------------------m(._.)m--------*\
|
|  The main C header file for KnoRBA programs.
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

#ifndef KNORBA_H
#define KNORBA_H

#include <stdlib.h>
#include <string.h> // defines memset()

#if defined(__linux__) || defined(__APPLE__) || defined(__sun) \
    || defined(sun) || defined(_AIX)

  #include <unistd.h>
  #define K_STD_UNIX

#else
  #error "The current platform is not supported"
#endif

#include <pthread.h>
#include <sched.h>

#ifdef __APPLE__
  #include <mach/mach_init.h>
  #include <mach/thread_policy.h>
#endif

#ifndef BOOL
  #define BOOL char
#endif

#ifndef TRUE
  #define TRUE 1
#endif

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef NULL
  #define NULL 0
#endif

#define VERY_LONG_UINT unsigned long long int

#define __shared __attribute__((annotate("knorba-shared")))
#define __synchronized
#define K_INDEX(x) 0
#define K_SUM(x, y)

typedef enum truth {
  F = 0,
  T = 1,
  X = -1
} truth_t;

typedef struct __k_GridParticleParams {
  void* shared;
  VERY_LONG_UINT begin;
  VERY_LONG_UINT end;
} __k_GridParticleParams_t;


// TODO limit access within compilatin unit
struct {
  int   numCpus;
  void* (*activeParticle)(void*);
  void* activeParticleArgs;
} __k_system_global;

BOOL __k_system_global_initialized = FALSE;

inline void 
__k_system_init() {
  if(__k_system_global_initialized)
    return;
  __k_system_global.activeParticle = 0;
  __k_system_global.activeParticleArgs = 0;

  #ifdef K_STD_UNIX
    __k_system_global.numCpus = sysconf(_SC_NPROCESSORS_ONLN);
  #else
    __k_system_global.numCpus = 1;
  #endif

  __k_system_global_initialized = TRUE;
}

// Accessor function
void 
__k_system_setActiveParticle(void* (*p)(void*), void* args) {
  __k_system_init();
  __k_system_global.activeParticle = p;
  __k_system_global.activeParticleArgs = args;
}

inline void
__k_GridParticleParams_delete(__k_GridParticleParams_t* t) {
  free(t->shared);
  free(t);
}

void 
__k_fireParticlesInGrid(void* (*p)(void*), void* args, 
                        VERY_LONG_UINT size[], int nDims) 
{
  __k_system_init();

  VERY_LONG_UINT particlesPerCpu = size[0] / __k_system_global.numCpus;
  unsigned int remainderParticles = size[0] % __k_system_global.numCpus;


  unsigned int i;
  int res;
  VERY_LONG_UINT offset = 0;
  VERY_LONG_UINT numParticles;

  for(i = 0; i < __k_system_global.numCpus; i++) {
    numParticles = particlesPerCpu;
    if(i < remainderParticles)
      numParticles++;

    __k_GridParticleParams_t* params = 
      (__k_GridParticleParams_t*) malloc(sizeof(__k_GridParticleParams_t));
    
    params->shared = args;
    params->begin = offset;
    
    offset += numParticles;
    params->end = offset - 1;

    pthread_t thread;
    res = pthread_create(&thread, NULL, p, params);
    // TODO error if res != 0

    
    #ifdef __APPLE__
      thread_affinity_policy_data_t policy;
      memset(&policy, 0, sizeof(thread_affinity_policy_data_t));
      policy.affinity_tag = i;
      thread_port_t threadPort = pthread_mach_thread_np(thread);
 //     kern_return_t ret = thread_policy_set(threadPort, 
 //                                           THREAD_AFFINITY_POLICY, 
 //                                           (thread_policy_t)&policy, 
 //                                           THREAD_AFFINITY_POLICY_COUNT);
      // TODO error if ret
    #else
      cpu_set_t cpuset;
      CPU_ZERO(&cpuset);
      CPU_SET(i, &cpuset);
      res = pthread_setaffinity_np(current_thread, sizeof(cpuset), 
        (sched_param*)&cpuset);
      // TODO error if res != 0
    #endif
  }
}

void 
fireParticlesInGrid(int size) {
  __k_system_init();

  int nDims = 1;
  VERY_LONG_UINT sizeArray[1];
  sizeArray[0] = size;

  if(__k_system_global.activeParticle == 0)
    // TODO return error
    return;

  __k_fireParticlesInGrid(__k_system_global.activeParticle, 
                          __k_system_global.activeParticleArgs, sizeArray, 
                          nDims);
}

#endif


#include <knorba.h>

//using namespace std;

/*
typedef struct {
  double step;
  double something;
  double sum;
} __k_main_shared;

void* __k_particle_1_thread(void* arg) {
  __k_GridParticleParams_t* params = (__k_GridParticleParams_t*)arg;

  __k_main_shared* __k_shared = (__k_main_shared*)params->shared;

  #define step (__k_shared->step)
  #define something (__k_shared->something)
  #define sum (__k_shared->sum)

  while(true) {
    double __k_sum = sum;
    for(long int __k_counter_1 = params->begin; __k_counter_1 <= params->end;
        __k_counter_1++)
    {
      int i = __k_counter_1;
      double x = (i + 0.5)*step;
      double t = 4.0 / (1.0 + x*x);
      __k_sum += t;
    }
    // mutex lock
    sum += __k_sum;
    // mutex unlock

    //singal end
    //pause thread
  }

  #undef step
  #undef something
  #undef sum

  __k_GridParticleParams_delete(params);

}
  
int _t_main() {
  int nBeans = 100000;
  
  __k_main_shared* __k_shared = malloc(sizeof(__k_main_shared));
  #define step (__k_shared->step)
  #define something (__k_shared->something)
  #define sum (__k_shared->sum)

  step = 1.0/nBeans;
  something = 12;
  sum = 0;
 
  __k_system_setActiveParticle(&__k_particle_1_thread, __k_shared);

  fireParticlesInGrid(nBeans);
  
  return 0;

  #undef step
  #undef something
  #undef sum
}
*/















int main() {
  int nBeans = 100000;

  __shared double step = 1.0/nBeans, something=12;
  __shared double sum = 0;

  double local_sum = 0;
    
  #pragma knorba particle grid(X, Y, Z)
  {
    int i = __k_counter_X;
    double x = (i + 0.5)*step;
    double t = 4.0 / (1.0 + x*x);
    
    K_SUM(sum, t);
    local_sum += t;
  }
    
  __synhronize sum += local_sum;

  fireParticlesInGrid(nBeans, 13123123, 123123123);

  //cout << "Pi = " << sum*step << endl;
  return 0;
}










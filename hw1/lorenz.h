/*
 * from https://en.wikipedia.org/wiki/Lorenz_system
 * Good Preset Values:
 *  p = 15, s = 10, b = 8/3
 *  p = 28, s = 10, b = 8/3
 */
#define LORENZ_P1_RHO   15.00
#define LORENZ_P1_SIGMA 10.00
#define LORENZ_P1_BETA   2.67

#define LORENZ_P2_RHO   28.00
#define LORENZ_P2_SIGMA 10.00
#define LORENZ_P2_BETA   2.67

struct lorenz_s
{
  double r; //rho
  double s; //sigma
  double b; //beta
};

typedef struct lorenz_s lorenz_t;

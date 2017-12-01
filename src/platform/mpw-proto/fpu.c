#include <rumboot/printf.h>

#include <math.h>

static int s1, s2, s3;

static void prandom_reset()
{
  s1 = 100;
  s2 = 200;
  s3 = 300;
}

static uint32_t prandom32()
{
  #define TAUSWORTHE(s,a,b,c,d) ((s&c)<<d) ^ (((s <<a) ^ s)>>b)

  s1 = TAUSWORTHE(s1, 13, 19, 4294967294UL, 12);
  s2 = TAUSWORTHE(s2, 2, 25, 4294967288UL, 4);
  s3 = TAUSWORTHE(s3, 3, 11, 4294967280UL, 17);

  return (s1 ^ s2 ^ s3);
}

static void fill_buff_with_double_random_seq(double* buf, const uint32_t size) {
  prandom_reset();

  union Data {
    uint32_t a[2];
    double b;
  } data;

  size_t i;
  for(i=0; i < size; i++) {

//    rumboot_printf("X0\n");

    double v =  ( (double) (prandom32() % 10000)) / ((double) (prandom32()  % 500000));
    v *= (prandom32() % 2) ? 1 : -1;
    data.b = v;

    buf[i]    = data.b;
    //rumboot_printf("buf[%d] = %f\n", i, data.b);
  }
  rumboot_printf("\n");
}

//FixMe: Ugly hack.
//FHERE MUST BE fabs(double, double)
bool fequal(double a, double b, double epsilon)
{
  if((a>0 && b<0) || (a<0 && b>0))
    return false;

  a *= (a > 0) ? 1 : (-1);
  b *= (b > 0) ? 1 : (-1);

  double res = a-b;
  res *= (res > 0) ? 1 : (-1);
  return (res < epsilon) ? true : false;
}

bool test_fpu()
{
  //enable_fpu();

  const size_t size = 100;
  double buf[size];
  double a[size];
  double b[size];

  //rumboot_printf("SIZE OF DOUBLE %d\n", sizeof(double));

  fill_buff_with_double_random_seq(buf, size);

  size_t i;
  for(i=0; i<size; i++) {

    a[i] = cos(acosf(buf[i]));
    b[i] = sin(asinf(buf[i]));
  }

  //Check arrays
  for(i=0; i<size; i++) {

    if(  !fequal(a[i], buf[i], 0.0000001)  /* || ( b[i] != buf[i])*/ ) {

      rumboot_printf("Test FPU ERROR. buf[%d] = %.18f \n", i, buf[i]);
      rumboot_printf("arcsines[%d] = %.18f\n",i, a[i]);
      rumboot_printf("arccoses[%d] = %.18f\n",i, b[i]);
      //rumboot_printf("cos(arccoses[%d]) = %f \n", cos(arccoses[i]), i);
      return false;
    }
  }

  rumboot_printf("Test FPU OK!\n");

  return true;
}

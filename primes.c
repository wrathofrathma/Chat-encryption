#include <stdio.h>
#include <libnsqrand.h>

unsigned long sq_mod_m (unsigned long n, unsigned long m)
{
     unsigned long res = n*n % m;
     
     if (n != 1 && n != m - 1 && res == 1)
          return 0;
     return res;
}

unsigned long expmod_mr (unsigned long base, unsigned long power, unsigned long modulo)
{
     if (power == 0)
          return 1;
     else if (power % 2 == 0)
          return sq_mod_m (expmod_mr (base, power / 2, modulo), modulo);
     else
          return (base * expmod_mr (base, power - 1, modulo)) % modulo;
}

int prime_p (int n, int times)
{
     while (times > 0) {
          int a = nsqrandr (1, n);
          if (expmod_mr (a, n - 1, n) != 1)
               return 0;
          times--;
     }
     return 1;
}

int random_prime (int low, int high, int times)
{
     int num = nsqrandr (low, high);
     while (!prime_p (num, times))
          num = nsqrandr (low, high);
     
     return num;
}
int nth_prime (int low, int times)

{

     int count = 0;

     int n = low;

     

     if (n % 2 == 0)

          ++n;

     else if (n < 2)

          n = 2;

     

     while (count < times)

          if (prime_p (++n, 10))

               count++;

     

     return n;

}

int main34()
{
     int i;
     
     nsqseed (time (NULL));
     for (i = 0; i < 10000; ++i)
          printf ("%d\n", random_prime (0, 9000, 100));
     return 0;
}

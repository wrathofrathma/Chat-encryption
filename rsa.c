unsigned long sq (unsigned long x)
{
     return x * x;
}


unsigned long Rsa (unsigned long base, unsigned long power, unsigned long mod)
{
     if (power == 0)
          return 1;
     else if (power % 2 == 0)
          return sq (Rsa (base, power/2, mod)) % mod;
     else
          return (base * Rsa (base, power - 1, mod)) % mod;
}

int egcd (int a, int b)
{
     int a1 = a;
     int b1 = b;
     int t1 = 0;
     int t = 1;
     int q = b1 / a1;
     int r = b1 - q*a1;
     int tmp;

     while (r > 0) {
          tmp = t1 - q*t;
          if (tmp >= 0)
               tmp = tmp % b;
          else
               tmp = b - ((-tmp) % b);
          t1 = t;
          t = tmp;
          b1 = a1;
          a1 = r;
          q = b1/a1;
          r = b1 - q*a1;
     }
     return t % b;
}


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <libnsqrand.h>
#include "primes.c"
#include "rsa.c"
//Size of all the arrays
#define SIZE 2048 
//private encryption key
#define private 3769
#define private2 263
#define private3 7553
//tracking the size of the integer array
int tracker=0;
int p,q,e,d,n;
unsigned long gcd (unsigned long a, unsigned long b);
//XOR encryption key, changable, size doesn't matter, but the more random, the better
char XKey[1024]="klajfdkalsfoi8uj8oij23;lkm;lfaflkjai94";
void encrypt(char *x, int *out, int size);
void decrypt(int *x, char *out, int size);

int main2()
{
    nsqseed (time (NULL));
    char straw[SIZE];
    int encrypted[SIZE];
    char decrypted[SIZE];
    printf("Enter a string in which you'd like to play with: ");
    fgets(straw,99,stdin);
    printf("Choosen string...: %s", straw);
    //dostuff(straw);
    encrypt(straw, encrypted, strlen (straw));
    decrypt(encrypted, decrypted, strlen (straw));
    printf("Hello %s\n", decrypted);
}

int expGen(int x, int y)
{
    int test=3;
    int tot=((x-1)*(y-1));  
    while(gcd(test,tot)!=1)
    {
        test++;
    }
    return test;
}
int keyGen(int pub)
{
    int test=private-(pub*3);
    p=nth_prime(test/4,test/5);
    q=nth_prime(test/5,test/3);
    e=expGen(p,q);
    d=egcd(e,(p-1)*(q-1));
    n=p*q;
    printf("Public key %d , test %d, P: %d, Q: %d, E: %d, D: %d, n: %d \n",pub,test,p,q,e,d,n);
}


unsigned long gcd (unsigned long a, unsigned long b)

{
     unsigned long u, v, t;
     if (a >= b) {
          u = a;
          v = b;
     }
     else {
          v = a;
          u = b;
     }
     while (v > 0) {
          t = u;
          u = v;
          v = t % v;
     }
     return u;
}

void encrypt(char *x, int *out, int size)
{
    int buf;
    int ind=0;
    int inc=0;
    int pub=nsqrandr(100,1000);
    keyGen(pub);
    int dex=(pub*private2)-(7*private2);
    out[0]=dex^private2;
    while(ind < size && x[ind] != '\0')
    {
        buf=alg(x[ind],pub,inc);
        out[ind+1]=buf;
        ind++;
        inc++;
        if(inc>=strlen(XKey))
            inc=0;
    }
//    print(out, size);

}


int alg(int x, int public,int inc)
{   
    x=(x-5)*3;
    x=Rsa(x,e,n);
    x=public+private-x;
    x=x^XKey[inc];
    return x;
}
int undoalg(int x, int public,int inc)
{
    x=x^XKey[inc];
    x=public+private-x;
    x=Rsa(x,d,n);
    x=(x/3)+5;
    return x;
}
void decrypt(int *x, char *out, int size)
{
    int inc=0;
    int ind=0;
    int buf;
    int pub = x[0];
    pub=pub^private2;
    pub=((private2*7)+pub)/private2;
    keyGen(pub);
    while(inc < size)
    {
        buf=undoalg(x[inc+1],pub,ind);
        out[inc]=buf;
        inc++;
        ind++;
        if(ind>=strlen(XKey))
            ind=0;
    }
}

int print(int *buflist, int size)
{
   int ind=0; 
   int buf=0;
    while(buf<size)
    {
        printf("Hidden index value %d : %d \n", ind, buflist[ind]);
        ind++;
        buf++;
    }
}

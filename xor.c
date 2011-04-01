#include <stdio.h>

int main(int argc, char **argv)
{
    int ksize=strlen(argv[2]);
    int msize=strlen(argv[1]);
    char message[msize];
    char key[ksize];
    strcpy(&message, argv[1]);
    strcpy(&key,argv[2]);
    printf("Message %s \n Key %s\n",message,key);
    XOR(&message,&key,msize,ksize);
    printf("%d message after first xor \n",message);
    XOR(&message,&key,msize,ksize);
    printf("%s message after second xor \n",message);
    return 0;
}

int XOR(int *in, char *key,int msize,int ksize)
{
    int buf[msize+1];
    int kdex=0;
    int mdex=0;
    int test=1;
    while(test)
    {
        if(kdex>=ksize-1)
            kdex=0;
        in[mdex]=(in[mdex])^(key[kdex]);
        if(mdex>=msize-1)
            break;
        mdex++;
        kdex++;
    }
//    in=buf;
    return 0;

}

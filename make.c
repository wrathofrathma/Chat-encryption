#include <stdio.h>
char serv[100]="gcc -lm -lpthread -lnsqrand -o linxserv server.c cryptosockets.c |grep error";
char cli[100]="gcc -lm -lpthread -lnsqrand -o linx client.c cryptosockets.c |grep error";
int main()
{
    printf("Compiling server now. \n");
    system(serv);
    printf("Compiling client now. \n");
    system(cli);
    system("ls -l |grep linx");
    return 0;
}

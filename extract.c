#include <stdio.h>
#include <string.h>
char extract[100]="7z e client.7z";
char uextract[100]="7zr e client.7z";
char make[100]="gcc make.c -o compile";
char compile[100]="./compile";
char nsq[100]="make && sudo make install";
int main()
{
    char buf;
    printf("Do you use ubuntu?(y/n)(ubuntu uses 7zr instead of 7z) \n");
    buf=getchar();
    if(buf=='y')
    {
        printf("Extracting archive...\n");        
        system(uextract);
    }
    else if(buf=='n')
    {
        printf("Extracting archive...\n");
        system(extract);
    }
    else
    {
        printf("you've entered an invalid character \n");
        return 1;
    }
    printf("Extracted \n");
    printf("Going to install libnsqrand(Custom RNG) \n");
    system(nsq);
    system(make);
    system(compile);
    system("sudo mv linx /usr/bin");
    printf("To use this applications RNG(Random number generator), add the line \n include /usr/local/lib to /etc/ld.so.conf\n  Then run sudo ldconfig to use the libnsqrand library \n");
    printf("To run this application, run the command: linx \n");
    return 0;

}

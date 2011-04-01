#include <string.h>
#include <stdio.h>
#include <stdlib.h>


struct usertrack
{
    char username[100];
    int socket;
    struct usertrack *new;
};

void kaboom (struct usertrack *p);

struct userInfo
{
    char username[2048];
    int fat_person;
};
struct usertrack *begin=NULL;
int append(struct usertrack **p, int socket, char username[200])
{
     struct usertrack *cur, *prev, *new;

     new = (struct usertrack *) malloc (sizeof (struct usertrack));
     if (new == NULL) {
          printf ("Out of memory!\n");
          return -1;
     }
     new->socket = socket;
     strcpy (new->username, username);
     new->new = NULL;

     prev = NULL;
     for (cur = *p; cur; cur = cur->new)
          prev = cur;
     if (prev)
          prev->new = new;
     else
          *p = new;
     printf("%s at %d successfully appended! \n", username, socket);
     return 0;
}

int delete_item (struct usertrack **p, char username[100])
{
     struct usertrack *cur, *prev;
     
     prev = NULL;
     cur = *p;
     while (cur) {
          if (!strcmp (cur->username, username)) {
               if (prev) {
                    prev->new = cur->new;
               }
               else {
                    *p = cur->new;
               }
               free (cur);
               return 1;
          }
          cur = cur->new;
     }
     return 0;
}
     
int findItem(struct usertrack *p, char username[100])
{
    struct usertrack *temp = p;
    printf("In the find item function, %s \n", username);
    while (temp) {
        if (!strcmp(temp->username, username)) {
            printf("Username found: %s, with socket: %d\n", temp->username, temp->socket);
            return temp->socket;
        }
        temp = temp->new;
    }
    printf("Username not found!\n");
    return -1;
}
int findSock(struct usertrack *p, int socket, char *tempname)
{
    struct usertrack *temp = p;
    printf("In the find socket function %d \n",socket);
    while (temp) {
        printf("In the findsock loop \n");
        if (temp->socket==socket) {
            printf("Username found: %s, with socket: %d\n", temp->username, temp->socket);
            tempname=temp->username;
            return 0;
        }
        temp = temp->new;
    }
    printf("User not found!\n");
    return 1;
}

void kaboom (struct usertrack *p)
{
     if (p == NULL)
          return;
     kaboom (p->new);
     free (p);
}

#include <stdio.h>
#include <string.h>

int commandParser (char *cmdStr, char cmd[20], char param[20])
{
     char *cp;                  /* Parser pointer */
     int pos = 0;               /* So we don't overflow */
     
     cp = cmdStr;
     if (*cp != '/') {
          printf ("This doesn't look like a command string...\n");
          printf ("It starts with a '%c'!\n", *cp);
          return -1;
     }
     
     /* Advance the pointer to the char after the / */
     ++cp;
     while (*cp != '\0') {
          if (pos >= 19) {
               printf ("Command too long!\n");
               return -1;
          }
          else if (*cp == ' ')
               break;           /* Done */
          cmd[pos] = *cp++;
          pos++;
     }
    
     cmd[pos] = '\0';           /* Ensure null termination */

     /* Go after the param - assume it is to the end of string */
     /* Advance the parsing pointer */
     ++cp;
     pos = 0;
     while (*cp) {
          if (pos >= 19) {
               printf ("Param too long!\n");
               return -1;
          }
          param[pos] = *cp++;
          pos++;
     }
     param[pos] = '\0';
     /* It's safe to use cmd and param to run whatever command you
      * want from here .*/
     printf ("Got cmd: %s\nGot param: %s\n", cmd, param);

     return 1;
}



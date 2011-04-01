/* Alright, here goes.
 * The protocol will send messages in the following format:
 * The first integer in the message to be sent will be a "header" integer.
 * Its value will be the number of chunks in the rest of the message.
 * If it's negative, the string will be in encrypted form.  If it's positive,
 * the string will be an ordinary string.
 * We will not deal with bytes on the high level functions - everything will
 * be considered as "chunks".  Even though an encrypted char will take
 * 4 times as much space as an unencrypted one, we'll just think of them
 * both as being "one chunk".
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "cryptosockets.h"
#include "encrypt.c"

#define MAGIC 0x2222

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* Macros to find the length of a string that's encrypted, //bogus
 * and to determine if a header is encrypted.  See above definition. */
#define ENCRYP_HEADER(h) ((h) < 0)
#define GOOD_MAGIC(h) ((h) == MAGIC)

typedef enum
{
     BAD_MAGIC=-2
} CRYP_ERRORS;

/* These functions will read a number from a socket */
unsigned char read_byte (int sockfd)
{
     unsigned char output = 0;
     
     read (sockfd, &output, 1, 0);
     return output;
}

short int read_int2 (int sockfd)
{
     short int output = 0;
     int bytes_read = 0;
     int n = 0;
     
     while (bytes_read < 2) {
          n = read (sockfd, &output, 2 - bytes_read, 0);
          bytes_read += n;
     }
     return output;
}

int read_int4 (int sockfd)
{
     int output = 0;
     int bytes_read = 0;
     int n = 0;
     
     while (bytes_read < 4) {
          n = read (sockfd, &output, 4 - bytes_read, 0);
          bytes_read += n;
     }
     return output;
}

int send_byte (int sockfd, unsigned char b)
{
     if (!(send (sockfd, &b, 1, 0) > 0))
          return 0;
     return 1;
}

int send_int2 (int sockfd, short int s)
{
     if (!(send (sockfd, &s, 2, 0) > 0))
          return 0;
     return 1;
}

int send_int4 (int sockfd, int i)
{
     if (!(send (sockfd, &i, 4, 0) > 0))
          return 0;
     return 1;
}


/* Sending and receiving functions */
int read_header (int sockfd)
{
     return read_int4 (sockfd);
}

int read_encrypted (int sockfd, int *output, int max_chunks, int total)
{
     int chunks_read = 0;
     int pos = 0;
     int junk;
     int to_read = MIN (max_chunks, total);

     while (chunks_read < to_read) {
          output[pos] = read_int4 (sockfd);
          pos++;
          chunks_read++;
     }
     junk = chunks_read;
     while (junk < total) {
          printf ("%d chars left to dispose of\n", total - junk);
          read_int4 (sockfd);
          junk++;
     }
     
     return chunks_read;
}

int read_unencrypted (int sockfd, char *output, int max_chunks, int total)
{
     int chunks_read = 0;
     int pos = 0;
     int junk;
     int to_read = MIN (max_chunks, total);
     
     while (chunks_read < to_read) {
          output[pos] = read_byte (sockfd);
          pos++;
          chunks_read++;
     }
     junk = chunks_read;
     while (junk < total) {
          printf ("%d chars left to dispose of\n", total - junk);
          read_byte (sockfd);
          junk++;
     }
     
     return chunks_read;
}

int read_cryptomsg (int sockfd, char *output, int max_len) 
{
     int header;
     int *encrypted_output;
     int esize;
     int chunks_read;

     if (!GOOD_MAGIC (read_int2 (sockfd)))
          return -1;
     header = read_header (sockfd);
     esize = ABS (header);
     if (ENCRYP_HEADER (header))
          puts ("Got encrypted message");
     else
          puts ("Got unencrypted message");
     if (!ENCRYP_HEADER (header))
          return read_unencrypted (sockfd, output, max_len - 1, esize);
     encrypted_output = malloc (esize*sizeof (int));
     chunks_read = read_encrypted (sockfd, encrypted_output,
                                   max_len, esize);
//     printf ("%s CHUNSK: %d\n", output, chunks_read);
     decrypt (encrypted_output, output, chunks_read);
     output[chunks_read-1] = '\0';
     free (encrypted_output);
     return chunks_read;
}

/* Writing */
int send_header (int sockfd, int size, int encrypted) 
{
     int h = size;

     send_int2 (sockfd, MAGIC);
     if (encrypted)
          return send_int4 (sockfd, -h);
     return send_int4 (sockfd, h);
}

int send_encrypted (int sockfd, int *sequence, int seq_len)
{
     int chunks_sent = 0;
     int pos = 0;

     send_header (sockfd, seq_len, 1);
     while (chunks_sent < seq_len) {
          if (!send_int4 (sockfd, sequence[pos])) {
               fprintf (stderr, "Error writing to socket!");
               return -1;
          }
          chunks_sent++;
          pos++;
     }
     return chunks_sent;
}

int send_unencrypted (int sockfd, const char *str, int str_len)
{
     int chunks_sent = 0;
     int pos = 0;
     
     send_header (sockfd, str_len, 0);
     while (chunks_sent < str_len) {
          if (!send_byte (sockfd, str[pos])) {
               fprintf (stderr, "Error writing to socket!");
               return -1;
          }
          chunks_sent++;
          pos++;
     }
     return chunks_sent;
}

int send_cryptomsg (int sockfd, const char *str, int e)
{
     if (!e)
          return send_unencrypted (sockfd, str, strlen (str));

     int len;
     int *encrypted_buffer;
     int chunks_sent;
     
     len = strlen (str) + 1;
     encrypted_buffer = malloc (len*sizeof (int));
     encrypt (str, encrypted_buffer, len);
     chunks_sent = send_encrypted (sockfd, encrypted_buffer, len);
     free (encrypted_buffer);
     return chunks_sent;
}

               
          

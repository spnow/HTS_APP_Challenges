#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct license_header
{
  char magic_1[4];              /* LIC\0 */
  char version[4];              /* 1.8\0 */
  char num_1[10];               /* 8char num + \0 */
  char num_2[10];               /* 8char num + \0 */
};


int
lic_gen (char *dest, char *user)
{
  int offset, len;
  char *len_1 = "1aaaaaaaa";
  char *len_2 = "1bbbbbbbb";
  char *s1 = "----";

  len = atoi (len_1);
  if (len < 4)
    len = 4;
  assert (len == strlen (s1));

  /* 0-3 header */
  strcpy (dest, "LIC");

  /* 4-7 version */
  strcpy (dest + 4, "1.8");

  /* 8-17 */
  strcpy (dest + 8, len_1);

  /* 18-27 */
  strcpy (dest + 18, len_2);

  /* 28-31 footer */
  strcpy (dest + 28, "HTS");

  offset = 0x20;
  /* s1 */
  memcpy (dest + offset, s1, strlen (s1) + 1);
  offset += strlen (s1) + 1;

  /* s2 */
  len = keygen (dest + offset, s1);
  offset += len;
  len = sprintf (dest + 18, "%d", len - 1);
  dest[18 + len] = 'b';

  /* 50-56 */
  memset (dest + offset, 0, 0x0a);
  offset += 0x0a;

  strcpy (dest + offset, "HTS_NQ");
  offset += strlen ("HTS_NQ") + 1;

  memset (dest + offset, 0, 0x0a);
  offset += 0x0a;

  return offset;
}


int
main (int argc, char **argv)
{
  int fd, key_len;
  char buf[1000];

  memset (buf, 0, 1000);

  key_len = lic_gen (buf, argv[1]);

  fd = open ("app18win.lic", O_RDWR | O_CREAT | O_TRUNC);
  if (fd < 0)
    {
      perror ("Error writing file");
      exit (-1);
    }

  write (fd, buf, key_len);

  close (fd);

  return 0;
}

int
keygen (char * dest, char *src)
{
  int i, j, key, count;
  key = 0;

  strcpy (dest, "");

  for (i = 0; i < 0x32; i++)
    {
      for (j = 0; j < strlen (src); j++)
        {
          key -= 238;
          key |= src[j];
          key = (((unsigned)key >> i) | j) ^ key;
          key = key * key;
        }

      if (i > 0 && key != 0)
        {
          count = sprintf (dest, "%s%X", dest, key);
          printf ("%2d %X\n", i, key);
        }
    }

  return count + 1;
}

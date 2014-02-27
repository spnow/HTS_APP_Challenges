#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main (int argc, char *argv[])
{
  int i, key = 0;
  char *name, passwd[100] = "HTS";

  if (argc != 2)
    {
      fprintf (stderr, "Usage: %s <user_name>\n", argv[0]);
      exit (-1);
    }
  name = argv[1];

  for (i = 0; i < strlen (name); i++)
    sprintf (passwd, "%s%s%02x", passwd, i % 2 ? "" : "-",
             key = ~(name[i] << key) & ((name[i] - key) >> 1));

  printf ("%s\n", passwd);

  return 0;
}

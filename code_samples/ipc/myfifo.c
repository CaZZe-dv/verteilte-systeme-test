/* Test program for named pipes (FIFO) */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/* Note: After starting, run a second terminal window and type
         
         cat foo

         in order to open the reading end of the fifo and output its contents. Delete the fifo via

         rm foo

         after running the sample.
*/
int main(int argc, char *argv[])
{
   FILE *fp;

   if (mkfifo("foo", 0660) == -1)
   {
      fprintf(stderr, "myfifo: Error creating fifo foo\n");
      return EXIT_FAILURE;
   }

   if ((fp = fopen("foo", "w")) != NULL)
   {
      fprintf(fp, "This is a FIFO test message\n");
   }
   fclose(fp);

   return EXIT_SUCCESS;
}

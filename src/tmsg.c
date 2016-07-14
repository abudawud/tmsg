/* COPYRIGHT
 * Copyright (C) 2016  Ahmad Waris Al H
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * AUTHOR
 * Amad Waris Al H (warisafidz@gmail.com)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include "tmsg.h"

int main(){

   FILE *file;
   int buff,status;
   char *mpath;
   pid_t child;

   struct conf fconf;
   
   mpath = malloc(sizeof(MASTER) + 3);	/*define the size*/

   (void)strcpy(mpath,MASTER);

   /*open the configuration file*/
   file = fopen("conf", "r");
   if (file == NULL){
      (void)printf("Cannot open 'conf' file, %s\n",strerror(errno));
      exit (EXIT_FAILURE);
   }

   /*read current master conf*/
   fconf.master = rconf(file);
   /*read max master*/
   fconf.mmax = rconf(file);
   /*read current index conf*/
   fconf.index = rconf(file);
   /*read index max conf*/
   fconf.imax = rconf(file);

   /*combine index of the master with the master file*/
   (void)strcpy(mpath+strlen(MASTER),itoa(fconf.master));

   (void)fclose(file);

   file = fopen(mpath,"r"); /*open the messeges db*/
   if (file == NULL){
      (void)printf("Cannot open Messege DB '%s', %s\n",mpath,strerror(errno));
      exit(EXIT_FAILURE);
   }

   while( (buff = fgetc(file)) != EOF){
   	if(buff != GREP)
            continue;
	else if ( (buff = fgetc(file)) != itoa(fconf.index)[0])
	    continue;
	else
	    break;
	}

/*    To make colored text, we need echo shell command and we can use execlp
 *    but we need a child to make new pid for run a shell command (echo)
 */
   child = fork();
   (void)wait(&status);

   if (child == 0)
      execlp("echo","echo","-e","\\033[0;3;37m",(char *)NULL);

   while( fgetc(file) == '\n');

   while ( (buff = fgetc(file)) != EOF && buff != GREP)
      (void)putchar(buff);

/* this is contain a bug
   __fpurge(stdout);
   (void)puts("|");
   (void)printf("'--------->");

SHOW DATE
   child = fork();
   (void)wait(&status);

   if(child == 0)
      execlp("date","date",(char *)NULL);
*/
/*Reset the color*/
   child = fork();
   (void)wait(&status);
   
   if(child == 0)
      execlp("echo","echo","-e","\\033[0;0m",(char *)NULL);

   (void)fclose(file);

   file = fopen("conf", "w");
   if (file == NULL){
      (void)printf("No write permission to 'conf' file, %s\n",strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (fconf.master == fconf.mmax){
      if(++fconf.index > fconf.imax - 1){
         fconf.index = START;
         fconf.master = START;
      }
   }
   else{
      if(++fconf.index > IMAX){
         fconf.master++;
         fconf.index = START;
      }
   }


   wconf(file,&fconf);

   (void)fclose(file);
	

   return 0;
}

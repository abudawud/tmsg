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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio_ext.h>

#include "tmsg.h"

#define TPATH  "/tmp/tuxtrack0x474f53"

#define SENTC  60
#define MAX    20

#define SPACE  ' '
#define LINE   '\n'
#define TAB    '\t'

#define COR       '+'
#define SIDE      '|'
#define BRID      ':'

extern char tmp[5];
FILE *file,*mfile;

void chout (char kar);
void container (void);
void header (void);
void indexing(void);

int main (int arg, char *args[]){
   int buff;
   struct conf conft;

   char *mpath;
   
   file = fopen("conf","r");
   
   if (file == NULL){
      (void)printf("Cannot open './conf' file, %s\n",strerror(errno));
      exit(EXIT_FAILURE);
   }

   conft.master = rconf(file);
   conft.mmax = rconf(file);
   conft.index = rconf(file);
   conft.imax = rconf(file);
       
   (void)printf("-----------------\nMaster: %d\tIndex: #%d\n",conft.mmax,conft.imax);

   mpath = malloc(sizeof(MASTER)+strlen(itoa(conft.mmax)));
   (void)strcpy(mpath,MASTER);
   (void)strcpy(mpath+strlen(MASTER),itoa(conft.mmax));
   
   (void)fclose(file);
   
   file = fopen(TPATH,"w+");

   if (file == NULL){
      (void)printf("Cannot open temp file on '%s', %s\n",TPATH,strerror(errno));
      exit(EXIT_FAILURE);
   }

   chout(GREP);
   chout(itoa(conft.imax)[0]);
   chout(LINE);
   
   (void)printf("------\nHeader :");
   header();

   chout(LINE);
   (void)puts("Type your messege! [press ENTER then ctrl-D when completed]");
   container();
   
   rewind(file);
   (void)puts("\nCHECK YOUR MESSEGE BEFORE PERMANENTLY ADDED TO DATABASE!\n");
   while((buff = fgetc(file)) != EOF)
      (void)putchar(buff);

   (void)printf("\nis it correct ? (Y/y):");
   buff = getchar();
   
   if(buff == 'Y' || buff == 'y'){
      rewind(file);

      mfile = fopen(mpath,"a");

      if (mfile == NULL){
         (void)printf("Cannot open '%s' file, %s\nYour last messege has been deleted",
               mpath,strerror(errno));
         exit(EXIT_FAILURE);
      }

      while((buff = fgetc(file)) != EOF)
         (void)fputc(buff,mfile);

      (void)fclose(mfile);
      
      mfile = fopen("conf","w");
     // puts("ERR");
      if (mfile == NULL){
         (void)printf("No write permission to 'conf' file, %s\n"
               "Duplicate index!, you need to delete some message in '%s' file\n",
               strerror(errno),mpath);
         exit(EXIT_FAILURE);
      }

      if(++conft.imax > IMAX){
         ++conft.mmax;
         conft.imax = START;
      }

      wconf(mfile,&conft);

      (void)fclose(mfile);
   }
   (void)fclose(file);

   return 0;
}

void chout (char kar){
   (void)fputc(kar,file);
}

void indexing(void){
   int buff;
   chout(GREP);
   __fpurge(stdin);
   while((buff = getchar()) != LINE)
      chout(buff);
   chout(LINE);
}
void header(){
   int
   buff,
   kar = 0,
   i,
   pos,
   counter = 0;
	
   char word[40];
	
   while ((buff = getchar()) != LINE)
      word[kar++] = buff;

   pos = (SENTC / 2) - ((kar+1) / 2);
   chout(COR);
   
   for (i = 0;i < pos; i++)
      chout(BRID);
	
   counter = i;
   
   for (i = 0;i < kar;i++)
      chout(word[i]);
   counter += i;
	
   while (counter++ < SENTC)
      chout(BRID);

   chout(BRID);
   chout(COR);

}

void container (void){
   int
   buff,
   kar = 0,
   pos = 0,
   lpos = 0;

   char word[MAX];

   chout(SIDE);
   chout(SPACE);

   while ( (buff = getchar()) != EOF){
      kar++;

      if (buff != SPACE && buff != LINE)
         word[pos++] = buff;
      else {
         lpos = pos;

         if (kar > SENTC){
	    kar = kar-pos-1;
	
            while (kar++ < SENTC)
	       chout(SPACE);
         
            chout(SIDE);
	    chout(LINE);
	    chout(SIDE);
	    chout(SPACE);
	    
            kar = pos+1;
	}
	for (pos = 0; pos < lpos; pos++)
	    chout(word[pos]);

	if (buff != LINE){
	    //printf("%d",pos);
	    chout(SPACE);
	}

	pos = 0;
      }
   }
   while(kar++ < SENTC)
      chout(SPACE);

   chout(SPACE);
   chout(SIDE);
   chout(LINE);
   chout(COR);
   
   kar = 0;
   while (kar++ < SENTC){
      if (kar == SENTC/2)
         chout('~');
      else
         chout(BRID);
   }
   
   chout(BRID);
   chout(COR);
   chout(LINE);

}

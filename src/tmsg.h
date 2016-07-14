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

#define IMAX      9 
#define START	  0
#define GREP	  '#'
#define MASTER	  "msg"

char tmp[5];	/*buffer to hold return address from any function*/
struct conf{
	int master,mmax,index,imax;
};

int rconf(FILE *stream);	/*read configuration*/
void wconf(FILE *stream,struct conf *sconf);
char *itoa(int num);

int rconf(FILE *stream){
	int pc,i = 0;
	char str[3];
	
	(void)memset(str,'\0',sizeof(str));
	
	while((pc = fgetc(stream)) != '=');
	
	while ((pc = fgetc(stream)) != '\n')
		str[i++]=pc;
	
	return atoi(str);
}

char *itoa(int num){
	(void)memset(tmp,'\0',sizeof(tmp));

	(void)sprintf(tmp,"%d",num);
	return tmp;
}

void wconf(FILE *stream,struct conf *sconf){
   (void)fprintf(stream,"#This configuration generated manually by mpad and bmsg.\n"
               "#Never change manually if you didn't know what you do\n");
   (void)fprintf(stream,"masterx=%d\n",sconf->master);
   (void)fprintf(stream,"mmax=%d\n",sconf->mmax);
   (void)fprintf(stream,"index=%d\n",sconf->index);
   (void)fprintf(stream,"imax=%d\n",sconf->imax);
}

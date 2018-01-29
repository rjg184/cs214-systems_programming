#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "sorter_server.h"
#include "mergesort.c"
#include <pthread.h>
#include <arpa/inet.h>

#define MAX_FILES 10000
 
record *Globalarray;
int colnum;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int Globalarraysize;
char *sortpath;	//Path leading to the file being sorted
char *category;
char *output_dir;
int hasoutput;
int hasinput;

int loadArray(char * data, record * array)
{

    int size = 128;
    char *line = strsep(&data, "\n");// remove column headings
    //printf("token: %s\n", token);
    char * token=0;

    line = strsep(&data, "\n");
    int i=0;


    while(line != NULL){
        //printf("line %i: %s\n", i, token);
       
        //char *token;
       
        // 1) STRING - Color
 
        if(strlen(line)==0)
        {
            //printf("End of input, breaking\n");
            break;
        }
	
	if(strlen(line)<27)
	{
		break;
	}
	
 	//,,,,,,,,,,,,,,,,,,,,,,,,,,\n\0
            token = strsep(&line, ",");
        //printf("*****%s*****\n",token);
            trim(token); //remove trailing and leading spaces
            array[i].color = malloc(strlen(token) + 1);
            strcpy(array[i].color, token);
        //printf("color: %s\n", array[i].color);
 
            // 2) STRING - Director Name
            token = strsep(&line, ",");
            array[i].directorName=malloc(strlen(token) + 1);
            strcpy(array[i].directorName, token);
        //printf("director name: %s\n", array[i].directorName);
       
            // 3) INT - Number of critic review
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].numCriticReviews = -1;
        } else {
            array[i].numCriticReviews = atoi(token);
        }
           
        // 4) INT - Duration
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].duration = -1;
        } else {
            array[i].duration = atoi(token);
        }
           
        // 5) INT - Director facebook likes
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].directorFBlikes = -1;
        } else {
            array[i].directorFBlikes = atoi(token);
        }
           
        // 6) INT - Actor 3 facebook likes
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].actor3FBlikes = -1;
        } else {
            array[i].actor3FBlikes = atoi(token);
        }
           
        // 7) STRING - Actor 2 name
        token = strsep(&line, ",");
        array[i].actor2name = malloc(strlen(token) + 1);
        strcpy(array[i].actor2name, token);
           
        // 8) INT - Actor 1 facebook likes
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].actor1FBlikes = -1;
        } else {
            array[i].actor1FBlikes = atoi(token);
        }
           
        // 9) INT - Gross
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].gross = -1;
        } else {
            array[i].gross = atoi(token);
        }
           
        // 10) STRING - Genres
        token = strsep(&line, ",");
        array[i].genres=malloc(strlen(token) + 1);
        strcpy(array[i].genres, token);
           
        // 11) STRING - Actor 1 name
        token = strsep(&line, ",");
        array[i].actor1name = malloc(strlen(token) + 1);
        strcpy(array[i].actor1name, token);
           
        // 12) STRING - Movie title
        if(line[0]=='\"'){
            line = line + 1;
            token = strsep(&line, "\"");
            trim(token); //remove trailing and leading spaces
            array[i].movieTitle = malloc(strlen(token));
            strcpy(array[i].movieTitle, token);
            strsep(&line, ",");  
        } else {
            token = strsep(&line, ",");
            trim(token); //remove trailing and leading spaces
            array[i].movieTitle = malloc(strlen(token) + 1);
            strcpy(array[i].movieTitle, token);
        }
           
        // 13) INT - Num_voted_users
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].numVotedUsers = -1;
        } else {
            array[i].numVotedUsers = atoi(token);
        }
           
        // 14) INT - Cast total facebook likes
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].castTotalFBlikes = -1;
        } else {
            array[i].castTotalFBlikes = atoi(token);
        }
           
        // 15) STRING - Actor 3 name
        token = strsep(&line, ",");
        array[i].actor3name = malloc(strlen(token) + 1);
        strcpy(array[i].actor3name,token);
           
        // 16) INT - Facenumber_in_poster
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].faceNumberPoster = -1;
        } else {
            array[i].faceNumberPoster = atoi(token);
        }
           
        // 17) STRING - plot keywords
        token = strsep(&line, ",");
        array[i].plotKeywords = malloc(strlen(token) + 1);
        strcpy(array[i].plotKeywords,token);
           
        // 18) STRING - Movie imdb link
        token = strsep(&line, ",");
        array[i].link = malloc(strlen(token) + 1);
        strcpy(array[i].link,token);
           
        // 19) INT - Num user for reviews
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].numUserReviews = -1;
        } else {
            array[i].numUserReviews = atoi(token);
        }
           
        // 20) STRING - Language
        token = strsep(&line, ",");
        array[i].language = malloc(strlen(token) + 1);
        strcpy(array[i].language, token);
           
        // 21) STRING - Country
        token = strsep(&line, ",");
        array[i].country = malloc(strlen(token) + 1);
        strcpy(array[i].country, token);
           
        // 22) STRING - Content Rating
        token = strsep(&line, ",");
        array[i].contentRating = malloc(strlen(token) + 1);
        strcpy(array[i].contentRating,token);
           
        // 23) INT - Budget
        token = strsep(&line, ",");
	//printf("%s\n", token);
	//printf("%i\n",atoi(token));
        if(token[0] == '\0'){
            array[i].budget = -1;
        } else {
            array[i].budget = atoi(token);
        }
           
        // 24) INT - Title Year
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].titleYear = -1;
        } else {
            array[i].titleYear = atoi(token);
        }
           
        //25) INT - Actor 2 Facebook Likes
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].actor2FBlikes = -1;
        } else {
            array[i].actor2FBlikes = atoi(token);
        }
           
        // 26) DOUBLE - IMDB Score Num
        char* temp;
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].imdbScore = -1;
        } else {
            array[i].imdbScore = strtod(token, &temp);
        }
           
        // 27) DOUBLE - Aspect Ratio
        token = strsep(&line, ",");
        if(token[0] == '\0'){
            array[i].aspectRatio = -1;
        } else {
            array[i].aspectRatio = strtod(token, &temp);
        }
           
        // 28) INT - Movie Facebook Likes
        token = strsep(&line, ",");
	//printf("token: %s\n", token);
        if(token[0] == '\0'){
            array[i].movieFBlikes = -1;
        } else {
            array[i].movieFBlikes = atoi(token);
        }
 
        line = strsep(&data, "\n");
        i++;
    }	
	free(data);
 	return i;
	
}
 
void *clientHandler(void *fd) {
	int comm_fd = *(int*) fd;
	int rc;
	char buff[20];
	char coln[20];
	int buffSize = 0;
	int colnum = 0;
	char signal[20];
	
	
	recv(comm_fd,signal,20,0);	
	if(strcmp(signal,"Sending_File")==0)
	{	
		//printf("Received SF signal, now receiving buffsize...\n");
		rc = recv(comm_fd, buff, 20, 0); 	//gets size of incoming csv file **(in the form of a CHAR ARRAY)
		buffSize = atoi(buff); 		//puts size of incoming csv file into an integer
		//printf("buffSize = %d\n", buffSize);

		char * buffer = malloc(sizeof(char) * buffSize); //inializes the buffer in which we will put all of the new csv files
		char * data = calloc(sizeof(char), buffSize); //initializes the 
		record * array = malloc(buffSize * 1.5);
		int bytesrec=0;
		while(1){
			bzero(buffer, buffSize);  //this nulls out "buffer" so theres no leftover characters each time it reads from client
				
			rc = recv(comm_fd, buffer, buffSize, 0);  //reads from client and puts it into str
			bytesrec+=rc;
			//printf("rc: %d\n", rc);
			strcat(data, buffer);

			if(bytesrec >= buffSize)
			{
				break;
			}

			  	//*** Puts all of the data received from the buffer into a fixed char array called data 
						//WITH multi-threads, you need to adjust the size of data.  RIGHT NOW, the data array isn't big enough
		}
		//printf("bytesrec =%d",bytesrec);
		//printf("%d",data);	
		int i = loadArray(data, array);
		//mergesort(array, ,i);
	
		pthread_mutex_lock(&m);
		//     (Load stuff into the global array)
			int currsize = Globalarraysize;
		    	Globalarraysize+=i;
			//printf("Globalarraysize:%d\n",Globalarraysize);
		    	Globalarray=realloc(Globalarray,Globalarraysize*sizeof(record));
			int k;
			int l = 0;	
			for(k=currsize; k<Globalarraysize; k++)
			{
			Globalarray[k] = array[l];
			l++;
			}
			free(array);
		pthread_mutex_unlock(&m);

		send(comm_fd, "FUF", 20, 0);
	}
	else if(strcmp(signal,"Dump_Request")==0)
	{
		char colchar[20];
		int colnum;
		//printf("Recieved dump request\n");
		recv(comm_fd,colchar,20,0);
		colnum = atoi(colchar);
		//printf("Column number is: %d\n",colnum);
		//printf("Global array size is: %d\n",Globalarraysize);
		
		pthread_mutex_lock(&m);
		mergesort(Globalarray,colnum,Globalarraysize);
		pthread_mutex_unlock(&m);
		record * array = Globalarray;
		char finalline[3000];
		
		char num[20];
		snprintf(num, 20, "%d", Globalarraysize);

		send(comm_fd, num,20,0);
				
		int k;		
		for(k=0; k<Globalarraysize; k++){
					//this if statement takes each title with a "," and puts quotations around it
			if(strchr(array[k].movieTitle,',')){ //detection works fine

			    	char qm1[100] = "";
			    	char qm2[100] = "";
			    	qm1[0] = '\"';
			    	qm2[0] = '\"';
			    	strcat(qm1,array[k].movieTitle);
			    	strcat(qm1,qm2);
			    	array[k].movieTitle = qm1;
			}
			//printf("%s,%s,%d,%d,%d,%d,%s,%d,%f,%s,%s,%s,%d,%d,%s,%d,%s,%s,%d,%s,%s,%s,%d,%d,%d,%f,%f,%d\n", array[k].color,array[k].directorName,array[k].numCriticReviews,array[k].duration, array[k].directorFBlikes, array[k].actor3FBlikes, array[k].actor2name,array[k].actor1FBlikes,array[k].gross, array[k].genres, array[k].actor1name, array[k].movieTitle, array[k].numVotedUsers,array[k].castTotalFBlikes, array[k].actor3name,array[k].faceNumberPoster, array[k].plotKeywords, array[k].link, array[k].numUserReviews,array[k].language,array[k].country,array[k].contentRating,array[k].budget,array[k].titleYear,array[k].actor2FBlikes,array[k].imdbScore,array[k].aspectRatio,array[k].movieFBlikes);			
			
			snprintf(finalline, 3000, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,%s,%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[k].color,array[k].directorName,array[k].numCriticReviews,array[k].duration, array[k].directorFBlikes, array[k].actor3FBlikes, array[k].actor2name,array[k].actor1FBlikes,array[k].gross, array[k].genres, array[k].actor1name, array[k].movieTitle, array[k].numVotedUsers,array[k].castTotalFBlikes, array[k].actor3name,array[k].faceNumberPoster, array[k].plotKeywords, array[k].link, array[k].numUserReviews,array[k].language,array[k].country,array[k].contentRating,array[k].budget,array[k].titleYear,array[k].actor2FBlikes,array[k].imdbScore,array[k].aspectRatio,array[k].movieFBlikes);
			//printf("%d\n", array[k].movieFBlikes);
			send(comm_fd,finalline,3000,0);
			bzero(finalline,3000);
		}
		

	}
		
	else
	{
		printf("Received unknown input, breaking.\n");
		return;
	}
	//send FINISHED signal back to client.


}

int main(int argc, char ** argv){
	

	  long port = -1;
   
    if(argc != 3){
        printf("ERROR: Incorrect number of arguments.\n");
        return -1;
    }
   
    if (strcmp(argv[1], "-p") != 0){
        printf("ERROR: Expected '-p' as first argument.\n");
        return -1;
    }
   
    if(atol(argv[2]) < 0 || atol(argv[2]) > 65565){
        printf("ERROR: Port is out of range.\n");
        return -1;
    } else {
        port = atol(argv[2]);
    }


	
	Globalarraysize = 0;
	int listen_fd, comm_fd[MAX_FILES];
	
	struct sockaddr_in servaddr;
	//int addrlen = sizeof(address);
	
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = INADDR_ANY;
    	servaddr.sin_port = htons(port);

	int len = sizeof(servaddr);
	bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listen_fd, 10);
	printf("Received connections from: ");
	int threadcount = 0;
	pthread_t tids[4096];
	while(1){
	    	comm_fd[threadcount] = accept(listen_fd, (struct sockaddr*) &servaddr, (socklen_t *) &len);
	 	//printf("Accepted a client \n");

		printf("%s, ", inet_ntoa(servaddr.sin_addr));
		
		pthread_create(&tids[threadcount], NULL, clientHandler,&comm_fd[threadcount]);
		threadcount++;
		fflush(stdout);
	}


	return 0;
}

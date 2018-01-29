#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "sorter_client.h"
#include "mergesort.c"
#include <pthread.h>

char *sortpath;	//Path leading to the file being sorted
char *category;
char *output_dir;
int hasoutput;
int hasinput;
long port = -1;
char* hostname;
char ipAddr[256];

void removeQuotes(char *input){
	
	int i;
	
	if(input[0] == '\"' || input[0] == '"'){
		
		for(i = 0; i < strlen(input); i++){
			input[i] = input[i+1];
		}
		
	}
	
	if(input[strlen(input)-1] == '"' || input[strlen(input)-1] == '\"'){
		input[strlen(input)-1] = '\0';
	}

}

int isCSV(char * string){
	int length = strlen(string);
	if(length<4){
		//printf("String is too short to be a CSV file!\n");
		return 0;
	}
	char * ending = malloc(sizeof(char)*length);
	strncpy(ending, string+length-4, 4);

	if(strcmp(ending,".csv") == 0){ //0 means that this is a CSV file.
		free(ending);
		return 1;
	}else{ 				//1 means that this is not a CSV file.
		free(ending);
		return 0;
	}

	free(ending);
	return 0;
}

void* sendFile(void * csvname){
	FILE * fp=fopen(csvname, "r");
	char num[20];

	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family=AF_INET;
    	servaddr.sin_port=htons(port);
	
	//inet_pton(AF_INET,"128.6.13.218",&(servaddr.sin_addr));
	inet_pton(AF_INET,ipAddr,&(servaddr.sin_addr));

	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
	int fd = fileno(fp);
		

	send(sockfd,"Sending_File",20,0);

	struct stat st;
	stat(csvname, &st);
	int sz = st.st_size; //gets the size of the file
	//printf("Filesize: %i\n", sz);

	snprintf(num, 20, "%d", sz); //puts the size of the file and puts it into a char array
	send(sockfd, num, 20, 0); //sends the char array of the size of file

	int err = sendfile(sockfd, fd, NULL, sz); //sends the actual file
	//printf("Bytes Sent: %i\n",err);
	if (err == -1){
		printf("ERROR: Sendfile returned incorrectly.\n");
		pthread_exit(0);		
	}
	if (err != sz){
		printf("Didn't Send the whole file. \n");
	}else{
		printf("File sent!\n");		
	}
	fclose(fp);
	char buffer[128];
	read(sockfd, buffer, 128);
	printf("Signal Received: File Finished Sorting.\n");

	close(sockfd);
	pthread_exit(0);
}

void* getFile(void * path){

	pthread_t tid[2500];
	int tcounter = 0;

	char * path_ptr = (char*) path;
	//printf("Looking through Directory: %s\n", path_ptr);

	DIR * dir;
	dir = opendir(path_ptr);
	//struct dirent * string;
	struct dirent * string2 = malloc(5000);
	//int tnum;
	while(dir){  //traverses through contents of directory
		readdir_r(dir, string2, &string2);
		if(string2 != NULL){  //checks to see if we've checked everything in the directory
			if(string2->d_type == DT_DIR){  //If directory
				if( (strcmp(string2->d_name,".") !=0) && (strcmp(string2->d_name, "..") !=0) ){ //no hidden directory
					//printf("found a DIRECTORY <%s>, <------- thread here\n", string2->d_name);
					//spawn a thread to traverse through sub directory
					//pthread_t id;
					pthread_attr_t attr;
					pthread_attr_init(&attr);
					
					int newsize = (strlen(path_ptr) + strlen(string2->d_name) + 1);
					char * fullpath = malloc(sizeof(char)*(newsize + 1));
	    				strcpy(fullpath, path_ptr);
	    				strcat(fullpath, "/");
					strcat(fullpath, string2->d_name);
					
            				pthread_create(&tid[tcounter], NULL, getFile, fullpath); //fullpath should be a pointer
					//threadcount++;
					tcounter++;
				}
			}else{  //Not a directory, but could be a csv file
				int c = isCSV(string2->d_name);
				if(c==1){
					//printf("Found CSV FILE:  %s\n", string2->d_name);
					//spawn a thread to sort the csv file
					pthread_attr_t attr;
					pthread_attr_init(&attr);
					
					int newsize = (strlen(path_ptr) + strlen(string2->d_name) + 1);
					char * fullpath = malloc(sizeof(char)*(newsize + 1));
	    				strcpy(fullpath, path_ptr);
	    				strcat(fullpath, "/");
					strcat(fullpath, string2->d_name);

					pthread_create(&tid[tcounter], NULL, sendFile, fullpath);
					//threadcount++;
					tcounter++;
				}
			}
		}else{ //if theres nothing else to see in the directory, stop looping through directory
			break;
		}
	}
	//sleep(1);
	int j;
	for(j=0; j<tcounter; j++){
		pthread_join(tid[j], NULL);
		//tid[j] = pthread_self();
		//printf("%lu, ", tid[j]);
	}
	//tid[tnum] = pthread_self();
	//printf("tid is: %lu\n", tid[tnum]);
	//pthread_join(tid[tnum],NULL);

	pthread_exit(0);
}

int main(int argc, char ** argv){
	
	// Argument 1 -> ./sorter_client
	
	// Argument 2 -> category (e.g. "movie_title")
	if(argv[2] == '\0' || argv[2] == NULL){
		printf("ERROR: Invalid input, one or more arguments are null.\n");
		return -1;
	}
	
	// Argument 2 (cont) -> check validity of category
	if(strcmp(argv[2], "color") == 0 || strcmp(argv[2], "director_name") == 0 || strcmp(argv[2], "actor_2_name") == 0 || strcmp(argv[2], "genres") == 0 || strcmp(argv[2], "actor_1_name") == 0 || strcmp(argv[2], "movie_title") == 0 || strcmp(argv[2], "actor_3_name") == 0 || strcmp(argv[2], "plot_keywords") == 0 || strcmp(argv[2], "movie_imdb_link") == 0 || strcmp(argv[2], "language") == 0 || strcmp(argv[2], "country") == 0 || strcmp(argv[2], "content_rating") == 0 || strcmp(argv[2], "num_critic_for_reviews") == 0 || strcmp(argv[2], "duration") == 0 || strcmp(argv[2], "director_facebook_likes") == 0 || strcmp(argv[2], "actor_3_facebook_likes") == 0 || strcmp(argv[2], "actor_1_facebook_likes") == 0 || strcmp(argv[2], "gross") == 0 || strcmp(argv[2], "num_voted_users") == 0 || strcmp(argv[2], "cast_total_facebook_likes") == 0 || strcmp(argv[2], "facenumber_in_poster") == 0 || strcmp(argv[2], "num_user_for_reviews") == 0 || strcmp(argv[2], "budget") == 0 || strcmp(argv[2], "title_year") == 0 || strcmp(argv[2], "actor_2_facebook_likes") == 0 || strcmp(argv[2], "movie_facebook_likes") == 0 || strcmp(argv[2], "imdb_score") == 0 || strcmp(argv[2], "aspect_ratio") == 0){
		category = argv[2];
		//printf("category is: %s\n", category);
	} else {
		printf("ERROR: Invalid input, argument two must contain a valid movie category. \n");
		return -1;
	}
	
	// Argument 3 -> "-h"
	if (strcmp(argv[3], "-h") != 0){
		printf("ERROR: Expected '-h' as third argument.\n");
		return -1;
	}
	
	// Argument 5 -> "-p"
	if (strcmp(argv[5], "-p") != 0){
		printf("ERROR: Expected '-p' as fifth argument.\n");
		return -1;
	}
	
	// Argument 4 -> Host Name
	if(argv[4] == NULL || argv[4] == '\0'){
		printf("ERROR: HOST NAME IS NULL.\n");
		return -1;
	} else {
		hostname = argv[4];
		
		struct sockaddr_in serv_addr;
		struct addrinfo hints, *servinfo, *p;
		int rv;
		
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		
		if((rv = getaddrinfo(hostname, argv[6], &hints, &servinfo)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			return(-1);
		}
		
		memset(&serv_addr, '0', sizeof(serv_addr));
		
		for(p = servinfo; p != NULL; p = p->ai_next) {
			getnameinfo(p->ai_addr, p->ai_addrlen, ipAddr, sizeof(ipAddr), NULL, 0, NI_NUMERICHOST);
		}
		
		//printf("ip address is: %s\n", ipAddr);
		
		freeaddrinfo(servinfo);
	}
	
	// Argument 6 -> Port Number
	port = -1;
	port = atol(argv[6]);
	
	if(port>65565 || port < 0)
	{
		printf("ERROR: Port [%i] is out of range [1-65565]\n.",port);
		return -1;	
	}


	char *input_dir = malloc(4096); 
	char * output_dir = malloc(4096);
	DIR *dir;
	char path[4096];
	hasoutput = 0;
	hasinput = 0;
	
	// Case 1 - " ./sorter_client -c category -h grep.cs.rutgers.edu -p 12345 "
	if(argc == 7){
		
		// Argument 1 -> "-c"
		if (strcmp(argv[1], "-c") != 0){
			printf("ERROR: Expected '-c' as first argument.\n");
			return -1;
		}
		
		// gets cwd, you'll need to change this so that it give you the correct starting directory with different inputs
		if(getcwd(path, sizeof(path)) != NULL){
			//printf("Current path (case 1): %s\n",path);
		} else {
			printf("ERROR: Error with getcwd (getting the current working directory).");
			return -1;
		}
		

		strcpy(input_dir, path);
		//sortpath = path;
		if((dir = opendir(input_dir)) == NULL){
			printf("ERROR: Input directory is invalid or incorrect\n");
			return -1;
        }
		
	
	// Case 2 & 3 - " ./sorter_client -c category -h grep.cs.rutgers.edu -p 12345 -d [full directory / local directory] "
	//				" ./sorter_client -c category -h grep.cs.rutgers.edu -p 12345 -o [full directory / local directory] "
	} else if(argc == 9){
	
		// Argument 1 -> "-c"
		if(strcmp(argv[1], "-c") != 0){
			printf("ERROR: Expected '-c' as first argument.\n");
			return -1;
		}
		
		// Argument 7 -> "-d"
		if(strcmp(argv[7],"-d") == 0){
			hasinput = 1;
		}
		
		//Argument 7 -> "-o"
		else if(strcmp(argv[7],"-o") == 0){
			hasoutput = 1;
		} else {
			printf("ERROR: Expected '-o' or '-d' as seventh argument.\n");
			return -1;
		}
		
		//printf("check: %s\n", input_dir);
		
		// argument 7 is -d [directory]
		if(hasinput){
			
			strcpy(input_dir, argv[8]);
			removeQuotes(input_dir);
			//printf("input directory (case 2 &3): %s\n", input_dir);
			
			if((dir = opendir(input_dir)) == NULL){
				printf("ERROR: Input directory is invalid or incorrect\n");
				return -1;
			}
		}
		
		// argument 7 is -o [directory]
		if(hasoutput){
			
			strcpy(output_dir, argv[8]);
			removeQuotes(output_dir);
			//printf("output directory (case 2 & 3): %s\n", output_dir);
			
			if(getcwd(path, sizeof(path)) != NULL){
				//printf("%s\n",path);
			} else {
				printf("ERROR: Error with getcwd (getting the current working directory).");
				return -1;
			}
			
			strcpy(input_dir, path);
			dir = opendir(input_dir);
		}
		
		//printf("directory: %s\n", path);

	}
	
	// Case 4 - " ./sorter_client -c category -h grep.cs.rutgers.edu -p 12345 -d [full directory / local directory] -o [full directory / local directory]"
	else if(argc == 11){
			
		// Argument 7 -> "-d"
		if (strcmp(argv[7], "-d") == 0){
			strcpy(input_dir,argv[8]);
			removeQuotes(input_dir);
			//printf("input directory (case 4): %s\n", input_dir);
			hasinput = 1;
		}
		
		//Argument 9 -> "-o"
		if (strcmp(argv[9], "-o")==0){
			hasoutput = 1;
			strcpy(output_dir, argv[10]);
			removeQuotes(output_dir);
			//printf("output directory (case 4): %s\n", output_dir);
		} else {
			printf("ERROR: Incorrect argument entered.\n");
			return -1;
		}
		
		//printf("check: %s\n", input_dir);
			
			if((dir = opendir(input_dir)) == NULL){
				printf("ERROR: Input directory is invalid or incorrect\n");
				return -1;
			}		
	
	// Case 5 - Incorrect Arguments
	} else {
		printf("ERROR: There is an error in the given arguments.\n");
		return -1;
	}

	
	int colnum;
	if(strcmp(category,"color")==0){
		colnum = 0;
	}else if(strcmp(category,"director_name")==0){
		colnum = 1;
	}else if(strcmp(category,"num_critic_for_reviews")==0){
		colnum = 2;
	}else if(strcmp(category,"duration")==0){
		colnum = 3;
	}else if(strcmp(category,"director_facebook_likes")==0){
		colnum = 4;
	}else if(strcmp(category,"actor_3_facebook_likes")==0){
		colnum = 5;
	}else if(strcmp(category,"actor_2_name")==0){
		colnum = 6;
	}else if(strcmp(category,"actor_1_facebook_likes")==0){
		colnum = 7;
	}else if(strcmp(category,"gross")==0){
		colnum = 8;
	}else if(strcmp(category,"genres")==0){
		colnum = 9;
	}else if(strcmp(category,"actor_1_name")==0){
		colnum = 10;
	}else if(strcmp(category,"movie_title")==0){
		colnum = 11;
	}else if(strcmp(category,"num_voted_users")==0){
		colnum = 12;
	}else if(strcmp(category,"cast_total_facebook_likes")==0){
		colnum = 13;
	}else if(strcmp(category,"actor_3_name")==0){
		colnum = 14;
	}else if(strcmp(category,"facenumber_in_poster")==0){
		colnum = 15;
	}else if(strcmp(category,"plot_keywords")==0){
		colnum = 16;
	}else if(strcmp(category,"movie_imdb_link")==0){
		colnum = 17;
	}else if(strcmp(category,"num_user_for_reviews")==0){
		colnum = 18;
	}else if(strcmp(category,"language")==0){
		colnum = 19;
	}else if(strcmp(category,"country")==0){
		colnum = 20;
	}else if(strcmp(category,"content_rating")==0){
		colnum = 21;
	}else if(strcmp(category,"budget")==0){
		colnum = 22;
	}else if(strcmp(category,"title_year")==0){
		colnum = 23;
	}else if(strcmp(category,"actor_2_facebook_likes")==0){
		colnum = 24;
	}else if(strcmp(category,"imdb_score")==0){
		colnum = 25;
	}else if(strcmp(category,"aspect_ratio")==0){
		colnum = 26;
	}else if(strcmp(category,"movie_facebook_likes")==0){
		colnum = 27;
	}


	//printf("input: %s\n", input_dir);
	
	pthread_t tid;
	pthread_create(&tid, NULL, getFile, (void*) input_dir);

	pthread_join(tid, NULL);

	printf("All signals received, sending dump request...\n");

	//send DMP request, also send Colnum to server, (server should then mergesort on data)

	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
    	servaddr.sin_port=htons(port);
	inet_pton(AF_INET,ipAddr,&(servaddr.sin_addr));


	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	send(sockfd,"Dump_Request",20,0);
	
	char num[20];
	snprintf(num, 20, "%d", colnum);
	send(sockfd, num, 20, 0);
	char numstr[20];	
	recv(sockfd,numstr,20,0);
	int numlines = atoi(numstr);
	//printf("Numlines: %d\n",numlines);
	printf("Dump request confirmed, receiving data now...\n");
	char readin[2000];
	int k;	
	
	FILE* fp;

	if(hasoutput)
    	{
        mkdir(output_dir,0777);
        strcat(output_dir,"/AllFiles-sorted-");
        strcat(output_dir,category);
        strcat(output_dir,".csv");
    	fp = fopen(output_dir, "w+");
    	}
   	else
	{
        strcat(output_dir,"AllFiles-sorted-");
        strcat(output_dir,category);
        strcat(output_dir,".csv");
	}    	
	
	fp = fopen(output_dir, "w+");

	if(fp==NULL)
	{
		printf("output_Dir: %s\n", output_dir);
		printf("Shits broke my dude\n");
		return 0;
	}
	printf("output_Dir: %s\n", output_dir);
	 fprintf(fp,"color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");

	for(k=0; k<numlines; k++){
					//this if statement takes each title with a "," and puts quotations around it
		recv(sockfd,readin,3000,0);
		//printf("%s",readin);		
		fprintf(fp,"%s",readin);
		bzero(readin,3000);
	}
	fclose(fp);
	printf("Sorted filed recieved.\n");
	return 0;
}

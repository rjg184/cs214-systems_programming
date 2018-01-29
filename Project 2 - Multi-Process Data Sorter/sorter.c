/*
 * sorter.c
 * 
 * Project 1: Multi-Process Sorting
 * 
 * Authors: 
 * Ronak Gandhi
 * Maxwell Mucha
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mergesort.c"
#include "Sorter.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int numchild;
char *sortname;	//Name of the file being sorted
char *sortpath;	//Path leading to the file being sorted
char *outputname;
char *fullname;
char *category;
char *output_dir;
//int num_processes;
int hasoutput;
int hasinput;

int main(int argc, char **argv) {
	
	int originalPID = getpid();
	// Argument 2 -> category (e.g. "movie_title")
	if (argv[2] == '\0' || argv[2] == NULL){
		printf("ERROR: Invalid input, one or more arguments are null.\n");
		return -1;
	}
	
	if(strcmp(argv[2], "color") == 0 || strcmp(argv[2], "director_name") == 0 || strcmp(argv[2], "actor_2_name") == 0 || strcmp(argv[2], "genres") == 0 || strcmp(argv[2], "actor_1_name") == 0 || strcmp(argv[2], "movie_title") == 0 || strcmp(argv[2], "actor_3_name") == 0 || strcmp(argv[2], "plot_keywords") == 0 || strcmp(argv[2], "movie_imdb_link") == 0 || strcmp(argv[2], "language") == 0 || strcmp(argv[2], "country") == 0 || strcmp(argv[2], "content_rating") == 0 || strcmp(argv[2], "num_critic_for_reviews") == 0 || strcmp(argv[2], "duration") == 0 || strcmp(argv[2], "director_facebook_likes") == 0 || strcmp(argv[2], "actor_3_facebook_likes") == 0 || strcmp(argv[2], "actor_1_facebook_likes") == 0 || strcmp(argv[2], "gross") == 0 || strcmp(argv[2], "num_voted_users") == 0 || strcmp(argv[2], "cast_total_facebook_likes") == 0 || strcmp(argv[2], "facenumber_in_poster") == 0 || strcmp(argv[2], "num_user_for_reviews") == 0 || strcmp(argv[2], "budget") == 0 || strcmp(argv[2], "title_year") == 0 || strcmp(argv[2], "actor_2_facebook_likes") == 0 || strcmp(argv[2], "movie_facebook_likes") == 0 || strcmp(argv[2], "imdb_score") == 0 || strcmp(argv[2], "aspect_ratio") == 0){
		//valid input
	} else {
		printf("ERROR: Invalid input, argument two must contain a valid movie category. \n");
		return -1;
	}
	
	char *input_dir=malloc(4096); 
	output_dir = malloc(4096);
	outputname = malloc(4096);
	DIR *dir;
	char path[4096];
	//num_processes = 0;
	hasoutput=0;
	hasinput=0;
	
	// Case 1 - "./sorter -c movie_title"
	if(argc == 3){
		
		// Argument 1 -> "-c"
		if (strcmp(argv[1], "-c") != 0){
			printf("ERROR: Expected '-c' as first argument.\n");
			return -1;
		}
		
		// gets cwd, you'll need to change this so that it give you the correct starting directory with different inputs
		if(getcwd(path, sizeof(path)) != NULL){
			//printf("%s\n",path);
		} else {
			printf("ERROR: Error with getcwd (getting the current working directory).");
			return -1;
		}
		
		sortpath = path;
		if((dir = opendir(path)) == NULL){
			printf("ERROR: Input directory is invalid or incorrect\n");
			return -1;
        }
		
	
	// Case 2 & 3 - "./sorter -c movie_title -d [full directory]" or "./sorter -c movie_title -d [local directory]"
	// 				"./sorter -c movie_title -d [full directory]" -o [full directory]"
	} 
	else if(argc == 5)
	{
	
		// Argument 1 -> "-c"
		if (strcmp(argv[1], "-c") != 0)
		{
			printf("ERROR: Expected '-c' as first argument.\n");
			return -1;
		}
		
		// Argument 3 -> "-d"
		if (strcmp(argv[3],"-d") == 0)
		{

			hasinput=1;
		}
		//Argument 3 -> "-o"
		else if (strcmp(argv[3],"-o")==0)
		{
			hasoutput = 1;
			
		}
		else
		{
			printf("ERROR: Expected '-o' or '-d' as third argument.\n");
			return -1;
		}
		
		//printf("check: %s\n", input_dir);
		if (hasinput)
		{
			strcpy(input_dir, argv[4]);
			removeQuotes(input_dir);
			
			sortpath = input_dir;
			int p;
			for(p = 0; p < strlen(input_dir); p++)
			{ //copy input directory to path
				path[p] = input_dir[p];
			}
			if((dir = opendir(path)) == NULL){
				printf("ERROR: Input directory is invalid or incorrect\n");
				return -1;
			}
		}
		
		if(hasoutput)
		{
			
			strcpy(output_dir, argv[4]);
			removeQuotes(output_dir);
			
			if(getcwd(path, sizeof(path)) != NULL)
			{
			//printf("%s\n",path);
			} 
			else 
			{
			printf("ERROR: Error with getcwd (getting the current working directory).");
			return -1;
			}
			
			sortpath = path;
			dir = opendir(path);
		}
		
		//printf("directory: %s\n", path);

	}
	 
	else if(argc == 7)
	{	
		// Argument 5 -> "-d"
		if (strcmp(argv[3],"-d") == 0)
		{
			strcpy(input_dir,argv[4]);
			removeQuotes(input_dir);
			hasinput=1;
		}
		//Argument 7 -> "-o"
		if (strcmp(argv[5],"-o")==0)
		{
			hasoutput = 1;
			strcpy(output_dir,argv[6]);
			removeQuotes(output_dir);
		}
		else
		{
			printf("ERROR: Incorrect argument entered.\n");
			return -1;
		}
		
		//printf("check: %s\n", input_dir);
			
			sortpath = input_dir;
			int p;
			for(p = 0; p < strlen(input_dir); p++)
			{ //copy input directory to path
				path[p] = input_dir[p];
			}
			
			if((dir = opendir(path)) == NULL){
				printf("ERROR: Input directory is invalid or incorrect\n");
				return -1;
			}		
	}
	else {
		printf("ERROR: There is an error in the given arguments.\n");
		return -1;
	}
	
	printf("Initial PID: %d\n", getpid());
	printf("PIDS of all child processes: ");
	fflush(stdout);
	
	numchild = 0; 	// Number of children a process spawns
	//Directory and directory structure
    struct dirent *str;
    
    
    if ( dir != NULL) // null check for directory
    {
		str = readdir(dir); // reads the first thing in the directory
		for(;str != NULL; str = readdir(dir)) //for loop that goes through everything in the loop.
		{
			if((strcmp(str->d_name,"..") != 0) && (strcmp(str->d_name,".") != 0)) //checks for .  and .. directories
			{
				
				//num_processes++;
				
				if(str->d_type == DT_DIR) // if its a directory
				{
					
					int pid = fork(); // fork here
					if(pid == 0) //if its a child
					{
						
						printf("%d, ", getpid());
						fflush(stdout);
						
						closedir(dir); //close the original directory
						strcat(path,"/"); //append the folder to the path
						strcat(path, str->d_name);
						//printf("%s\n",nextdir);
						dir = opendir(path);	//open the new directory
						numchild = 0;	//reset number of children			
					}
					else
					{
						numchild++; //increment number of children in parent
					}
					
				}
				else
				{
					char* extension = strrchr(str->d_name,'.'); // gets the extension of the file
					char* sorted = strstr(str->d_name,"-sorted-");
					if (sorted != NULL)
					{
							//printf("\n %s is already sorted, skipping. \n",str->d_name);
					}
					
					
					if(extension!=NULL && sorted==NULL)
					{
						if(strcmp(extension,".csv")==0)			// ALL CSV FILES GO HERE
						{
							if(checkvalid(path,str->d_name))
							{

								int pid = fork(); // fork on here
								if(pid == 0) // if its  a child
								{
								
									printf("%d, ", getpid());
									fflush(stdout);
									numchild = 0; //reset number of children
									sortname = str->d_name; //set the name of the file
									sortpath = path; //set the path of the file
									break; //exit look
								}
								else
								{		
									numchild++; //increment number of children in parent
								}
							
							}
							else
							{
									//printf("\nImproperly formatted file, skipping: %s\n", str->d_name);
							}
										
						}
					}
					
				}		
			}	
		}	
	}
	closedir(dir); //close whatever directory is open.
	
	//at this point every file and every directory has its own process

	int i;
	int exitnum;
	if(sortpath != NULL && sortname == NULL) // goes in here if its a directory fork.
	{
			for(i = numchild;i>0;i--) // wait for all the children that the directory spawned. 
									  //returns if a directory has no children, or finishes waiting
			{
				//printf("[%i] waiting on child #%i\n", getpid(),i);
				int status;
				wait(&status);
				//printf("Child %i returns: %i \n",i,WEXITSTATUS(status));
				exitnum+=WEXITSTATUS(status);
				//printf("[%i] wait [%i] complete\n",getpid(),i);
			}
		if(originalPID == getpid())
		{
				printf("\nTotal number of processes: %i\n",exitnum+numchild+1);
		}
		
		return numchild+exitnum;

	}	
	
	//printf("[%i] %s/%s\n",getpid(),sortpath,sortname);
	fullname = malloc(4096); //for some reason sortpath and sortname get corrupted
																//so if you want to use them for something else
																//you'll need to malloc them into a variable.
		outputname = strcpy(outputname,sortname);											
		fullname = strcpy(fullname, sortpath);
		fullname = strcat(fullname, "/");
		fullname = strcat(fullname, sortname); // Sets the full path name 
	
	
	//printf("[%i] %s\n",getpid() ,fullname);
	
	//printf("\n According to this, the value of hasoutput is: %i\n",hasoutput);
	
	sort(argv);
	
	free(outputname);
	free(input_dir);
	free(output_dir);
	
	return 0;
	
} //end of 'main' function

void sort(char **argv){
	
	char* buffer = malloc(1000);
	char* hold = buffer;
	int size = 128;
	int i = 0;
	FILE* fp = fopen(fullname,"r");
	
	fgets(buffer, 1000, fp);
	
	char *token = strsep(&buffer, ",");
	int isin = 0;
	
	while(token != NULL){
		//printf("%s\n",token);
		if(strcmp(token, argv[2]) == 0 || strcmp(argv[2], "movie_facebook_likes") == 0 || strcmp(argv[2], "actor_3_name") == 0){	
			//printf("%s\n",token);
			isin = 1;
			break;
		}
		token = strsep(&buffer, ",");
	}
	
	if(isin == 0){ //argument does not match column category
		printf("\nERROR: Argument not found in CSV header. \n");
		return;
	}
	
	category = argv[2]; //category/column that needs to be sorted
	
	struct movie* array = malloc(sizeof(struct movie) * size);
	//printf("[%i] FILEPATH IS STILL\n %s/%s\n",getpid(),sortpath,sortname);
	while(fgets(buffer, 1000, fp)){
		
		//Array size increaser
		if (i == size-1){
			//printf("Wow, you entered a lot of stuff, we have to make our array bigger!\n");
			size = size * 2;
			array = realloc(array,sizeof(struct movie) * size);
		}
		
		char* token;
		//struct movie *a = malloc(sizeof(movie));
		//array[i] = *a;
		
		// 1) STRING - Color 
		token = strsep(&buffer, ",");
		trim(token); //remove trailing and leading spaces
		array[i].color = malloc(strlen(token) + 1);
		strcpy(array[i].color, token);

		// 2) STRING - Director Name 
		token = strsep(&buffer, ",");
		array[i].director_name=malloc(strlen(token) + 1);
		strcpy(array[i].director_name, token);
		
		// 3) INT - Number of critic review 
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].num_critic_for_reviews = -1;
		} else {
			array[i].num_critic_for_reviews = atoi(token);
		}
		
		// 4) INT - Duration
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].duration = -1;
		} else {
			array[i].duration = atoi(token);
		}
		
		// 5) INT - Director facebook likes
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].director_facebook_likes = -1;
		} else {
			array[i].director_facebook_likes = atoi(token);
		}
		
		// 6) INT - Actor 3 facebook likes
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].actor_3_facebook_likes = -1;
		} else {
			array[i].actor_3_facebook_likes = atoi(token);
		}
		
		// 7) STRING - Actor 2 name
		token = strsep(&buffer, ",");
		array[i].actor_2_name = malloc(strlen(token) + 1);
		strcpy(array[i].actor_2_name, token);
		
		// 8) INT - Actor 1 facebook likes
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].actor_1_facebook_likes = -1;
		} else {
			array[i].actor_1_facebook_likes = atoi(token);
		}
		
		// 9) INT - Gross
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].gross = -1;
		} else {
			array[i].gross = atoi(token);
		}
		
		// 10) STRING - Genres 
		token = strsep(&buffer, ",");
		array[i].genres=malloc(strlen(token) + 1);
		strcpy(array[i].genres, token);
		
		// 11) STRING - Actor 1 name
		token = strsep(&buffer, ",");
		array[i].actor_1_name = malloc(strlen(token) + 1);
		strcpy(array[i].actor_1_name, token);
		
		// 12) STRING - Movie title
		if(buffer[0]=='\"'){
			buffer = buffer + 1;
			token = strsep(&buffer, "\"");
			trim(token); //remove trailing and leading spaces
			array[i].movie_title = malloc(strlen(token));
			strcpy(array[i].movie_title, token);
			strsep(&buffer, ",");	
		} else {
			token = strsep(&buffer, ",");
			trim(token); //remove trailing and leading spaces
			array[i].movie_title = malloc(strlen(token) + 1);
			strcpy(array[i].movie_title, token);
		}
		
		// 13) INT - Num_voted_users
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].num_voted_users = -1;
		} else {
			array[i].num_voted_users = atoi(token);
		}
		
		// 14) INT - Cast total facebook likes
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].cast_total_facebook_likes = -1;
		} else {
			array[i].cast_total_facebook_likes = atoi(token);
		}
		
		// 15) STRING - Actor 3 name
		token = strsep(&buffer, ",");
		array[i].actor_3_name = malloc(strlen(token) + 1);
		strcpy(array[i].actor_3_name,token);
		
		// 16) INT - Facenumber_in_poster
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].facenumber_in_poster = -1;
		} else {
			array[i].facenumber_in_poster = atoi(token);
		}
		
		// 17) STRING - plot keywords
		token = strsep(&buffer, ",");
		array[i].plot_keywords = malloc(strlen(token) + 1);
		strcpy(array[i].plot_keywords,token);
		
		// 18) STRING - Movie imdb link
		token = strsep(&buffer, ",");
		array[i].movie_imdb_link = malloc(strlen(token) + 1);
		strcpy(array[i].movie_imdb_link,token);
		
		// 19) INT - Num user for reviews
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].num_user_for_reviews = -1;
		} else {
			array[i].num_user_for_reviews = atoi(token);
		}
		
		// 20) STRING - Language
		token = strsep(&buffer, ",");
		array[i].language = malloc(strlen(token) + 1);
		strcpy(array[i].language, token);
		
		// 21) STRING - Country
		token = strsep(&buffer, ",");
		array[i].country = malloc(strlen(token) + 1);
		strcpy(array[i].country, token);
		
		// 22) STRING - Content Rating
		token = strsep(&buffer, ",");
		array[i].content_rating = malloc(strlen(token) + 1);
		strcpy(array[i].content_rating,token);
		
		// 23) INT - Budget
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].budget = -1;
		} else {
			array[i].budget = atoi(token);
		}
		
		// 24) INT - Title Year
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].title_year = -1;
		} else {
			array[i].title_year = atoi(token);
		}
		
		//25) INT - Actor 2 Facebook Likes
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].actor_2_facebook_likes = -1;
		} else {
			array[i].actor_2_facebook_likes = atoi(token);
		}
		
		// 26) DOUBLE - IMDB Score Num
		char* temp;
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].imdb_score_num = -1;
		} else {
			array[i].imdb_score_num = strtod(token, &temp);
		}
		
		// 27) DOUBLE - Aspect Ratio
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].aspect_ratio = -1;
		} else {
			array[i].aspect_ratio = strtod(token, &temp);
		}
		
		// 28) INT - Movie Facebook Likes
		token = strsep(&buffer, ",");
		if(token[0] == '\0'){
			array[i].movie_facebook_likes = -1;
		} else {
			array[i].movie_facebook_likes = atoi(token);
		}
		
		/*
		// Checks / Print Statements
		printf("------ Movie No: %i ------\n", i);
		printf("%s\n", array[i].color);
		printf("%s\n", array[i].director_name);
		printf("%i\n", array[i].num_critic_for_reviews);
		printf("%i\n", array[i].duration);
		printf("%i\n", array[i].director_facebook_likes);
		printf("%i\n", array[i].actor_3_facebook_likes);
		printf("%s\n", array[i].actor_2_name);
		printf("%i\n", array[i].actor_1_facebook_likes);
		printf("%i\n", array[i].gross);
		printf("%s\n", array[i].genres);
		printf("%s\n", array[i].actor_1_name);
		printf("%s\n", array[i].movie_title);
		printf("%i\n", array[i].num_voted_users);
		printf("%i\n", array[i].cast_total_facebook_likes);
		printf("%s\n", array[i].actor_3_name);
		printf("%i\n", array[i].facenumber_in_poster);
		printf("%s\n", array[i].plot_keywords);
		printf("%s\n", array[i].movie_imdb_link);
		printf("%i\n", array[i].num_user_for_reviews);
		printf("%s\n", array[i].language);
		printf("%s\n", array[i].country);
		printf("%s\n", array[i].content_rating);
		printf("%i\n", array[i].budget);
		printf("%i\n", array[i].title_year);
		printf("%i\n", array[i].actor_2_facebook_likes);
		printf("%f\n", array[i].imdb_score_num);
		printf("%f\n", array[i].aspect_ratio);
		printf("%i\n", array[i].movie_facebook_likes);
		*/

		//array[i] = *a;
		
		i++;
		buffer = hold;
	}
	
	int m = i;
	int n = 27;
	//printf("[%i] FILEPATH IS STILL\n %s/%s\n",getpid(),sortpath,sortname);
	mergesort(array, category, 0, (m - 1)); //MERGE SORT
	printCSV(array, m, n); //PRINT OUTPUT/SORTED ARRAY
	
	// Free malloc'd fields/variables
	int index;
	
	for(index = 0; index <= i; index++){
		free(array[index].color);
		free(array[index].director_name);
		free(array[index].actor_2_name);
		free(array[index].genres);
		free(array[index].actor_1_name);
		free(array[index].movie_title);
		free(array[index].actor_3_name);
		free(array[index].plot_keywords);
		free(array[index].movie_imdb_link);
		free(array[index].language);
		free(array[index].country);
		free(array[index].content_rating);
	}
	
	free(fullname);
	free(array);
	free(buffer);
	
	fclose(fp);
	
}

void trim(char* token) {
	
	if(token[0] == ' '){ //leading white spaces
		
		int i, j;
		for(i = 0; token[i] == ' '; i++);
		
		for(j = 0; token[i+j] != '\0'; j++){
			token[j] = token[i+j];
		}
		
		token[j] = '\0';
		
	} 
	
	if(token[strlen(token)] == ' ' || token[strlen(token)] == '\0'){ //trailing white spaces
		
		int i = strlen(token);
		
		while(token[i] == ' ' || token[i] == '\0'){
			i--;
		}
		
		token[i + 1] = '\0';
		
	}
	
} //end of 'trim' function

//fullname
int checkvalid(char* path, char* filename)
{
	char* checkpath = malloc(4096);
	strcpy(checkpath,path);
	strcat(checkpath,"/");
	strcat(checkpath,filename);
	//printf("\n%s\n",checkpath);
	FILE* check = fopen(checkpath,"r");						
	char* checkbuf=malloc(5000);
int cnt = 0;
	fgets(checkbuf,5000,check);
	//printf("\n%s\n",checkbuf);
							
							
	char *token = strsep(&checkbuf, ",");
	while(token != NULL)
	{
		token = strsep(&checkbuf,",");
		//printf("\n%s\n",token);
		cnt++;
	}

	//printf("\n%i\n",cnt);						
	free(checkbuf);
	fclose(check);
	free(checkpath);

	if (cnt != 28)
	{
		return 0;
	}
	
	return 1;
	
}

void printCSV(struct movie* array, int m, int n) {
	
	FILE *fp2;
	int i;
	
	//printf("\n According to this, the value of hasoutput is: %i\n",hasoutput);
	
	if(hasoutput)
	{
		mkdir(output_dir,0777);
		strcat(output_dir,"/");
		strcat(output_dir,outputname);
		char* file=strrchr(output_dir,'.');
		strcpy(file,"-sorted-");
		strcat(file,category);
		strcat(file,".csv");
		//printf("\n We should be writing to: %s\n",output_dir);
		
		fp2 = fopen(output_dir, "w+");
	}
	else
	{
		
		char*extension = strrchr(fullname,'.'); //properly formats the name of the output file
		strcpy(extension,"-sorted-");
		strcat(extension,category);
		strcat(extension,".csv");
		//printf("%s\n",fullname);
		//char* filename = name;
		//filename = strcat(name, ".csv");
	
		fp2 = fopen(fullname, "w+");
	}
	// Print column titles
	fprintf(fp2, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes, actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	//fprintf(stdout, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes, actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	// Print each struct (each movie is 1 row with 28 columns)
	for(i = 0; i < m; i++){
		
		if(strchr(array[i].movie_title, ',')){
			fprintf(fp2, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,\"%s\",%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);	
			//fprintf(stdout, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,\"%s\",%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);		
		} else {
			fprintf(fp2, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,%s,%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);
			//fprintf(stdout, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,%s,%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);
		}
		
		
	}
	//printf("[%i] FILEPATH IS STILL\n %s/%s\n",getpid(),sortpath,sortname);
	//fclose(fp2);
	
	
	//for(i = 0; i < m; i++){
		
		// Checks / Print Statements
		//printf("------ Movie No: %i (i = %i) ------\n", i+1, i);
		//printf("%s\n", array[i].director_name);
		//printf("%i\n", array[i].num_critic_for_reviews);
		//printf("%i\n", array[i].duration);
		//printf("%i\n", array[i].director_facebook_likes);
		//printf("%i\n", array[i].actor_3_facebook_likes);
		//printf("%s\n", array[i].actor_2_name);
		//printf("%i\n", array[i].actor_1_facebook_likes);
		//printf("%i\n", array[i].gross);
		//printf("%s\n", array[i].genres);
		//printf("%s\n", array[i].actor_1_name);
		//printf("%s\n", array[i].movie_title);
		//printf("%i\n", array[i].num_voted_users);
		//printf("%i\n", array[i].cast_total_facebook_likes);
		//printf("%s\n", array[i].actor_3_name);
		//printf("%i\n", array[i].facenumber_in_poster);
		//printf("%s\n", array[i].plot_keywords);
		//printf("%s\n", array[i].movie_imdb_link);
		//printf("%i\n", array[i].num_user_for_reviews);
		//printf("%s\n", array[i].language);
		//printf("%s\n", array[i].country);
		//printf("%s\n", array[i].content_rating);
		//printf("%i\n", array[i].budget);
		//printf("%i\n", array[i].title_year);
		//printf("%i\n", array[i].actor_2_facebook_likes);
		//printf("%f\n", array[i].imdb_score_num);
		//printf("%f\n", array[i].aspect_ratio);
		//printf("%i\n", array[i].movie_facebook_likes);
		
	//}
	
	
} //end of 'printCSV' function

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
	
	//printf("output: %s\n", input);

}

/*
 * sorter_thread.c
 *
 * Project 2: Multi-Threaded Sorting
 *
 * Authors:
 * Ronak Gandhi
 * Maxwell Mucha
 *
 */
 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mergesort.c"
#include "sorter_thread.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
 
 char* category;
 char* outputdir;
 char* inputdir;
 
 int hasoutput=0;
 int hasinput=0;
 int full = 0;
 int totalnumthreads =0;
 
 struct movie* masterarray; // MUST BE LOCKED
 int masterarraysize; // MUST BE LOCKED
 int masterarraycount; //MUST BE LOCKED TO ACCESS
 
 pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 pthread_mutex_t expo = PTHREAD_MUTEX_INITIALIZER;
 
 void* dostuff(void *asf)
{
    printf("%s\n",(char*)asf);
    return 0;
}
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
void* directoryTrav(void *args)
{
   
    printf("%lu, ",pthread_self());
   
                    pthread_mutex_lock(&expo);
                    totalnumthreads++;
                    //printf("\n%i\n",totalnumthreads);
                    pthread_mutex_unlock(&expo);
                   
    int size=5000;
    pthread_t* tids = malloc(size*sizeof(pthread_t));
    int pathsize=100000;
    char** path2 = malloc(pathsize);
    int path2cnt=0;
    int count = 0;
   
   
    char* path = malloc(5000);
   
    strcpy(path, (char*) args);
   
    //printf("***%s\n",path);
    //fflush(stdout);
   
    struct dirent *parent;
    struct dirent *str=malloc(5000000);
    parent=str;
    DIR *dir;
    //printf("here\n");
    //fflush(stdout);
    if((dir = opendir(path)) == NULL)
    {
        printf("ERROR: Input directory is invalid or incorrect\n");
        return 0;
    }
    if ( dir != NULL) // null check for directory
    {
        readdir_r(dir,str,&str); // reads the first thing in the directory

        //fflush(stdout);
        for(;str != NULL; readdir_r(dir,str,&str)) //for loop that goes through everything in the directory.
        {

            if((strcmp(str->d_name,"..") != 0) && (strcmp(str->d_name,".") != 0)) //checks for .  and .. directories
            {
                if(str->d_type == DT_DIR) //  its a directory
                {
                    path2[path2cnt] = malloc(5000);
                    strcpy(path2[path2cnt],path);
                    strcat(path2[path2cnt],"/");
                    strcat(path2[path2cnt],str->d_name);
                   // printf("%s\n",path2[path2cnt]);
                    pthread_create(&tids[count],NULL,directoryTrav,path2[path2cnt]);
                    path2cnt++;
                    count++;
                    if(count>=size-1)
                    {
                        size=size*2;
                        tids = realloc(tids, size*sizeof(pthread_t));
                    }
                    if(path2cnt>=pathsize-1)
                    {
                        pathsize=pathsize*2;
                        path2 = realloc(path2, pathsize);
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
                        if(strcmp(extension,".csv")==0)         // ALL CSV FILES GO HERE
                        {  

                            path2[path2cnt] = malloc(10000);
                            strcpy(path2[path2cnt],path);
                            strcat(path2[path2cnt],"/");
                            strcat(path2[path2cnt],str->d_name);
                           
                            //printf("%s\n",path2[path2cnt]);
                           
                           
                            pthread_create(&tids[count],NULL,sort,path2[path2cnt]);
                            //pthread_create(&tids[count],NULL,dostuff,path2[path2cnt]);
                            path2cnt++;
                            count++;

                            if(count>=size-1)
                            {
                                size=size*2;
                                tids = realloc(tids, size*sizeof(pthread_t));
                            }
                            if(path2cnt>=pathsize-1)
                            {
                                pathsize=pathsize*2;
                                path2 = realloc(path2, pathsize);
                            }

                        }
                    }
                   
                }      
            }  
        }
           
    }
 
    //printf("%i\n",count);
   
    int i=0;
    for(;i<count;i++)
    {
        pthread_join(tids[i],0);
    }
   
    i=0;
   
    for(;i<path2cnt;i++)
    {
        free(path2[i]);
    }
    free(tids);
    free(parent);
    free(path);
    free(path2);
   
    closedir(dir); //close whatever directory is open.
   
    return 0;
}
 
void *sort(void *args){
    printf("%lu, ",pthread_self());
                    pthread_mutex_lock(&expo);
                    totalnumthreads++;
                    pthread_mutex_unlock(&expo);
                   
    char* fullname = (char*) args;
    char* buffer = malloc(1000);
    char* hold = buffer;
    int size = 128;
    int i = 0;
    FILE* fp = fopen(fullname,"r");
   
    fgets(buffer, 1000, fp);
   
    //THIS NEES TO BE CHANGED TO ACCOUNT FOR
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
       
        i++;
        buffer = hold;
    }
   
    int m = i;
    //int n = 27;
    //printf("[%i] FILEPATH IS STILL\n %s/%s\n",getpid(),sortpath,sortname);
   
    char* idk = malloc(strlen(category));
    strcpy(idk,category);
    mergesort(array, idk, 0, (m - 1)); //MERGE SORT
   
    //HERE WE'RE GONNA HAVE TO:
    //MUTEX LOCK
    
    pthread_mutex_lock(&lock);
   
   
    masterarraysize+=size;
    masterarraycount+=m;
   
    //printf("ARRAY: %i,%lu,%p, %s\n",masterarraycount,masterarraysize*sizeof(movie),&masterarray, category);
    masterarray=realloc(masterarray,masterarraysize*sizeof(struct movie));
   
    //printf("MASTER: %i,%i,%i\n",sizeof(masterarray),masterarraycount,masterarraysize);
    if(full == 0)
    {
        memcpy(masterarray, array, sizeof(movie) * m);
        //int i;
        //for(i = 0; i < m; i++){
        //  masterarray[i] = array[i];
            //printf("%s\n", masterarray[i].movie_title);
        //}
        full = 1;
    }
   
    else {
    mergeArrays(masterarray,array,category,masterarraycount-m,m);
    }
   
    pthread_mutex_unlock(&lock);
    //MERGESORT GLOBAL ARRAY WITH THE SORTED ARRAY
   
   
    //MUTEX UNLOCK
    //THEN AND ONLY THEN DO WE DO PRINTCSV IN MAIN
   
    //printCSV(array, m, 27);
    //Free malloc'd fields/variables
    //int index;
   
    free(array);
    free(buffer);
   
    fclose(fp);
    
    pthread_exit(0);
}
 
int main(int argc, char **argv)
{
    totalnumthreads=0;
    outputdir=malloc(4096);
    inputdir=malloc(4096);
    int i=1;
    if(argc%2!=1)
    {
        printf("ERROR: Incorrect number of flags or arguments!\n");
        return -1;
    }
    for (;i<argc;i++)
    {
        if(strcmp(argv[i], "-o")==0)
        {
            if(hasoutput)
            {
                printf("ERROR: Too many [-o] flags!\n");
                return -1;
            }
            hasoutput=1;
            strcpy(outputdir,argv[i+1]);
            removeQuotes(outputdir);
        }
       
        if(strcmp(argv[i], "-d")==0)
        {
            if(hasinput)
            {
                printf("ERROR: Too many [-d] flags!\n");
                return -1;
            }
            hasinput=1;
            strcpy(inputdir,argv[i+1]);
            removeQuotes(inputdir);
           
            if(opendir(inputdir) == NULL){
                printf("ERROR: Input directory is invalid or incorrect\n");
                exit(0);
            }
           
        }
        else
        {
            if(strcmp(argv[i], "-c")==0)
            {
            category = argv[i+1];
            }
        }  
    }
    if (category==NULL)
    {
        printf("ERROR: No [-c] flag or no/improper category!\n");
        return -1;
    }
   
   
    char *path = malloc(5000);
   
    if(hasinput==1)
    {
        strcpy(path,inputdir);
    }
    else
    {
        getcwd(path, 4096);
    }
       
    if(hasoutput==0)
    {
        getcwd(outputdir,4096);
    }
 
       
    masterarray= malloc(1000*sizeof(struct movie));
    masterarraysize=0;
    masterarraycount=0;
    pthread_t t;
   
    //printf("%s\n",path);
    //printf("Initial TID: %lu\n",pthread_self());
   
    printf("TIDS of all child threads: ");
    //fflush(stdout);
   
    pthread_create(&t, NULL, directoryTrav, path);
    pthread_join(t,0);
   
   
   printCSV(masterarray, masterarraycount, 27);
   
    int index;
   
   
   
    for(index = 0; index < masterarraycount; index++){
        free(masterarray[index].color);
        free(masterarray[index].director_name);
        free(masterarray[index].actor_2_name);
        free(masterarray[index].genres);
        free(masterarray[index].actor_1_name);
        free(masterarray[index].movie_title);
        free(masterarray[index].actor_3_name);
        free(masterarray[index].plot_keywords);
        free(masterarray[index].movie_imdb_link);
        free(masterarray[index].language);
        free(masterarray[index].country);
        free(masterarray[index].content_rating);
    }
   
    free(masterarray);
    free(path);
    free(outputdir);
    free(inputdir);
   
   
    printf("\nTotal number of threads: %i\n",totalnumthreads);
    //printf("Everything is freed, everything waited on, good work!\n");
   
   
    return 0;
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
 
void printCSV(struct movie* array, int m, int n) {
 
    int i;
    FILE *fp2;
   
    if(hasoutput)
    {
        mkdir(outputdir,0777);
        //printf("\n We should be writing to: %s\n",output_dir);
    }
   
        strcat(outputdir,"/AllFiles-sorted-");
        strcat(outputdir,category);
        strcat(outputdir,".csv");
    fp2 = fopen(outputdir, "w+");
   
    //printf("\n According to this, the value of hasoutput is: %i\n",hasoutput);
   
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
            //printf("%f\n", array[i].imdb_score_num);
        }
       
       
    }
    //printf("[%i] FILEPATH IS STILL\n %s/%s\n",getpid(),sortpath,sortname);
    fclose(fp2);
   
   
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
    return;
   
} //end of 'printCSV' function

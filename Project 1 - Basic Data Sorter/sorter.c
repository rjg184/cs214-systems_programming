/*
 * sorter.c
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


int main(int argc, char **argv) {
	
	char* buffer = malloc(1000);
	char* hold = buffer;
	int size = 128;
	int i = 0;
	
	fgets(buffer, 1000, stdin);

	if (argv[1][0] != '-' || argv[1][1] != 'c'){ //argument 1
		printf("ERROR: Expected '-c' as first argument.\n");
		return -1;
	}
	
	if (argv[2] == '\0'){ // argument 2
		printf("ERROR: Invalid input, one or more arguments are null.\n");
		return -1;
	}
	
	char *token = strsep(&buffer, ",");
	int isin = 0;
	
	while(token != NULL){
		//printf("%s\n",token);
		if(strcmp(token, argv[2]) == 0 || strcmp(argv[2], "movie_facebook_likes")==0){	
			//printf("%s\n",token);
			isin = 1;
			break;
		}
		token = strsep(&buffer, ",");
	}
	
	if(isin == 0){ //argument does not match column category
		printf("ERROR: Malformed input.\n");
		return -1;
	}
	
	if(argv[3] != '\0'){ //extra argument
		printf("ERROR: Extra argument received.\n");
		return -1;
	}
	
	char* category = argv[2]; //category/column that needs to be sorted
	
	struct movie* array = malloc(sizeof(struct movie) * size);
	
	while(fgets(buffer, 1000, stdin)){
		
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
	
	free(array);
	free(buffer);
	
	return 0;
	
} //end of 'main' function

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
	
	//FILE *fp;
	int i;
	//char name[] = "sortedmovies";
	//char* filename = name;
	//filename = strcat(name, ".csv");
	//fp = fopen(filename, "w+");
	
	// Print column titles
	//fprintf(fp, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes, actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	fprintf(stdout, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes, actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");
	
	// Print each struct (each movie is 1 row with 28 columns)
	for(i = 0; i < m; i++){
		
		if(strchr(array[i].movie_title, ',') || strchr(array[i].movie_title, ';')){
			//fprintf(fp, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,\"%s\",%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);	
			fprintf(stdout, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,\"%s\",%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);		
		} else {
			//fprintf(fp, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,%s,%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);
			fprintf(stdout, "%s,%s,%i,%i,%i,%i,%s,%i,%i,%s,%s,%s,%i,%i,%s,%i,%s,%s,%i,%s,%s,%s,%i,%i,%i,%f,%f,%i\n", array[i].color,array[i].director_name,array[i].num_critic_for_reviews,array[i].duration,array[i].director_facebook_likes,array[i].actor_3_facebook_likes,array[i].actor_2_name,array[i].actor_1_facebook_likes,array[i].gross,array[i].genres,array[i].actor_1_name,array[i].movie_title,array[i].num_voted_users,array[i].cast_total_facebook_likes,array[i].actor_3_name,array[i].facenumber_in_poster,array[i].plot_keywords,array[i].movie_imdb_link,array[i].num_user_for_reviews,array[i].language,array[i].country,array[i].content_rating,array[i].budget,array[i].title_year,array[i].actor_2_facebook_likes,array[i].imdb_score_num,array[i].aspect_ratio,array[i].movie_facebook_likes);
		}
		
		
	}
	
	//fclose(fp);
	
	
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

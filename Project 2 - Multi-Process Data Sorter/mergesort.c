/*
 * mergesort.c
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
#include "Sorter.h"
#include <stdio.h>

void mergesort(struct movie* arr, char* category, int left, int right) {
	
	if (left < right){
		int middle = left + ((right - left) / 2);
		
		mergesort(arr, category, left, middle);
		mergesort(arr, category, (middle + 1), right);
		
		mergesortHelper(arr, category, left, right, middle);
	}
	
} //end of 'mergesort' function
 
void mergesortHelper(struct movie* arr, char* category, int left, int right, int middle) {
	
	int k = left;
	
	// Left Sub-Array
	int i;
	int sizeL = (middle - left + 1); //size of left array
	struct movie *arrLeft = malloc(sizeof(struct movie) * sizeL);
	for (i = 0; i < sizeL; i++){ //copy data to temp arrays
		arrLeft[i] = arr[i + left];
	}
	i = 0;
	
	// Right Sub-Array
	int j;
	int sizeR =  (right - middle); //size of right array
	struct movie *arrRight = malloc(sizeof(struct movie) * sizeR);
	for (j = 0; j < sizeR; j++){
		arrRight[j] = arr[j + middle + 1];
	}
	j = 0;
	
	int strVal;
	int valueL;
	int valueR;
	double dvalueL;
	double dvalueR;
		
	while (i < sizeL && j < sizeR){
		if(strcmp(category, "color") == 0){ strVal = compareStrings(arrLeft[i].color, arrRight[j].color); }
		else if(strcmp(category, "director_name") == 0){ strVal = compareStrings(arrLeft[i].director_name, arrRight[j].director_name); }
		else if(strcmp(category, "num_critic_for_reviews") == 0){ valueL = arrLeft[i].num_critic_for_reviews; valueR = arrRight[j].num_critic_for_reviews; }
		else if(strcmp(category, "duration") == 0){ valueL = arrLeft[i].duration; valueR = arrRight[j].duration; }
		else if(strcmp(category, "director_facebook_likes") == 0){ valueL = arrLeft[i].director_facebook_likes; valueR = arrRight[j].director_facebook_likes; }
		else if(strcmp(category, "actor_3_facebook_likes") == 0){ valueL = arrLeft[i].actor_3_facebook_likes; valueR = arrRight[j].actor_3_facebook_likes; }
		else if(strcmp(category, "actor_2_name") == 0){ strVal = compareStrings(arrLeft[i].actor_2_name, arrRight[j].actor_2_name); }
		else if(strcmp(category, "actor_1_facebook_likes") == 0){ valueL = arrLeft[i].actor_1_facebook_likes; valueR = arrRight[j].actor_1_facebook_likes; }
		else if(strcmp(category, "gross") == 0){ valueL = arrLeft[i].gross; valueR = arrRight[j].gross; }
		else if(strcmp(category, "genres") == 0){ strVal = compareStrings(arrLeft[i].genres, arrRight[j].genres); }
		else if(strcmp(category, "actor_1_name") == 0){ strVal = compareStrings(arrLeft[i].actor_1_name, arrRight[j].actor_1_name); }
		else if(strcmp(category, "movie_title") == 0){ strVal = compareStrings(arrLeft[i].movie_title, arrRight[j].movie_title); }
		else if(strcmp(category, "num_voted_users") == 0){ valueL = arrLeft[i].num_voted_users; valueR = arrRight[j].num_voted_users; }
		else if(strcmp(category, "cast_total_facebook_likes") == 0){ valueL = arrLeft[i].cast_total_facebook_likes; valueR = arrRight[j].cast_total_facebook_likes; }
		else if(strcmp(category, "actor_3_name") == 0){ strVal = compareStrings(arrLeft[i].actor_3_name, arrRight[j].actor_3_name); }
		else if(strcmp(category, "facenumber_in_poster") == 0){ valueL = arrLeft[i].facenumber_in_poster; valueR = arrRight[j].facenumber_in_poster; }
		else if(strcmp(category, "plot_keywords") == 0){ strVal = compareStrings(arrLeft[i].plot_keywords, arrRight[j].plot_keywords); }
		else if(strcmp(category, "movie_imdb_link") == 0){ strVal = compareStrings(arrLeft[i].movie_imdb_link, arrRight[j].movie_imdb_link); }
		else if(strcmp(category, "num_user_for_reviews") == 0){ valueL = arrLeft[i].num_user_for_reviews; valueR = arrRight[j].num_user_for_reviews; }
		else if(strcmp(category, "language") == 0){ strVal = compareStrings(arrLeft[i].language, arrRight[j].language); }
		else if(strcmp(category, "country") == 0){ strVal = compareStrings(arrLeft[i].country, arrRight[j].country); }
		else if(strcmp(category, "content_rating") == 0){ strVal = compareStrings(arrLeft[i].content_rating, arrRight[j].content_rating); }
		else if(strcmp(category, "budget") == 0){ valueL = arrLeft[i].budget; valueR = arrRight[j].budget; }
		else if(strcmp(category, "title_year") == 0){ valueL = arrLeft[i].title_year; valueR = arrRight[j].title_year; }
		else if(strcmp(category, "actor_2_facebook_likes") == 0){ valueL = arrLeft[i].actor_2_facebook_likes; valueR = arrRight[j].actor_2_facebook_likes; }
		else if(strcmp(category, "imdb_score") == 0){ dvalueL = arrLeft[i].imdb_score_num; dvalueR = arrRight[j].imdb_score_num; }
		else if(strcmp(category, "aspect_ratio") == 0){ dvalueL = arrLeft[i].aspect_ratio; dvalueR = arrRight[j].aspect_ratio; }
		else if(strcmp(category, "movie_facebook_likes") == 0){ valueL = arrLeft[i].movie_facebook_likes; valueR = arrRight[j].movie_facebook_likes; }
			
		// Strings
		if(strcmp(category, "color") == 0 || strcmp(category, "director_name") == 0 || strcmp(category, "actor_2_name") == 0 || strcmp(category, "genres") == 0 || strcmp(category, "actor_1_name") == 0 || strcmp(category, "movie_title") == 0 || strcmp(category, "actor_3_name") == 0 || strcmp(category, "plot_keywords") == 0 || strcmp(category, "movie_imdb_link") == 0 || strcmp(category, "language") == 0 || strcmp(category, "country") == 0 || strcmp(category, "content_rating") == 0){
			if(strVal == 2){
				arr[k] = arrLeft[i]; 
				i++;
			} else {
				arr[k] = arrRight[j];
				j++;
			}
			k++;
		}
			
		// Ints
		if(strcmp(category, "num_critic_for_reviews") == 0 || strcmp(category, "duration") == 0 || strcmp(category, "director_facebook_likes") == 0 || strcmp(category, "actor_3_facebook_likes") == 0 || strcmp(category, "actor_1_facebook_likes") == 0 || strcmp(category, "gross") == 0 || strcmp(category, "num_voted_users") == 0 || strcmp(category, "cast_total_facebook_likes") == 0 || strcmp(category, "facenumber_in_poster") == 0 || strcmp(category, "num_user_for_reviews") == 0 || strcmp(category, "budget") == 0 || strcmp(category, "title_year") == 0 || strcmp(category, "actor_2_facebook_likes") == 0 || strcmp(category, "movie_facebook_likes") == 0){
			if(valueL <= valueR){
				arr[k] = arrLeft[i]; 
				i++;
			} else {
				arr[k] = arrRight[j];
				j++;
			}
			k++;
		}
		
		// Doubles
		if(strcmp(category, "imdb_score") == 0 || strcmp(category, "aspect_ratio") == 0){
			if(dvalueL <= dvalueR){
				arr[k] = arrLeft[i]; 
				i++;
			} else {
				arr[k] = arrRight[j];
				j++;
			}
			k++;
		}
		
		
	}
	
	// Left Array - copy leftover elements
	while (i < sizeL){
		arr[k] = arrLeft[i];
		i++;
		k++;
	}
	
	// Right Array - copy leftover elements
	while (j < sizeR){
		arr[k] = arrRight[j];
		j++;
		k++;
	}
	
	// Free left and right sub-arrays
	free(arrLeft);
	free(arrRight);
        
} //end of 'mergesortHelper' function

int compareStrings(char* str1, char* str2) {
	
	if(strcmp(str1, str2) > 0){ //str2 is less than str1
		return 1; //goes to right
	} else {
		return 2; //goes to left
	}
    
    return 1;
    
} //end of 'compareStrings' function

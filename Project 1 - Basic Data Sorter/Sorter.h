/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

#ifndef _Sorter_h
#define _Sorter_h

//Suggestion: define a struct that mirrors a record (row) of the data set

/* Struct for each movie in CSV file. Each movie contains 28 fields that describe it according to IMDB. */
typedef struct movie {
	
	char* color;
	char* director_name;
	int num_critic_for_reviews;
	int duration;
	int director_facebook_likes;
	int actor_3_facebook_likes;
	char* actor_2_name;
	int actor_1_facebook_likes;
	int gross;
	char* genres;
	char* actor_1_name;
	char* movie_title;
	int num_voted_users;
	int cast_total_facebook_likes;
	char* actor_3_name;
	int facenumber_in_poster;
	char* plot_keywords;
	char* movie_imdb_link;
	int num_user_for_reviews;
	char* language;
	char* country;
	char* content_rating;
	int budget;
	int title_year;
	int actor_2_facebook_likes;
	double imdb_score_num;
	double aspect_ratio;
	int movie_facebook_likes;
	
} movie;


/* Trims/removes leading and trailing spaces in a string */
void trim(char* str);

/* Loops through sorted struct array and prints the fields of each movie to STDOUT (command line) 
 * (printing to a CSV file named 'sortedmovies.csv' is also available if needed, but needs to be uncommented in 'printCSV' function) */
void printCSV(struct movie* arr, int m, int n);

/* Determines middle value to split single array into two halves (calls 'mergesortHelper') */
void mergesort(struct movie* arr, char* category, int left, int right);

/* Creates and sorts sub-arrays and then merges them together to create a single sorted array */
void mergesortHelper(struct movie* arr, char* category, int left, int right, int middle);

/* Compares two strings to determine their order during the sort (lexicographical ordering) */
int compareStrings(char* str1, char* str2);

//Suggestion: prototype a mergesort function

#endif

/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

/* Struct for each movie in CSV file. Each movie contains 28 fields that describe it according to IMDB. */
typedef struct record {
	
	char *color;
	char *directorName;
	int numCriticReviews;
	int duration;
	int directorFBlikes;
	int actor3FBlikes;
	char *actor2name;
	int actor1FBlikes;
	int gross;
	char *genres;
	char *actor1name;
	char *movieTitle;
	int numVotedUsers;
	int castTotalFBlikes;
	char *actor3name;
	int faceNumberPoster;
	char *plotKeywords;
	char *link;
	int numUserReviews;
	char *language;
	char *country;
	char *contentRating;
	int budget;
	int titleYear;
	int actor2FBlikes;
	double imdbScore;
	double aspectRatio;
	int movieFBlikes;
	
} record;

/* Trims/removes leading and trailing spaces in a string */
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

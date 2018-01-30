#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int compareStrings(char *str1, char *str2){
	
	if(strcmp(str1, str2) > 0){	// string 1 is first
		return 0;	
	}else {	// string 2 is first
		return 1;
	}
	
} //end of 'compareStrings'

void merge(movie *arr, int left, int middle, int right, int category){

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
		if(category == 0){ strVal = compareStrings(arrLeft[i].color, arrRight[j].color); }
		else if(category == 1){ strVal = compareStrings(arrLeft[i].directorName, arrRight[j].directorName); }
		else if(category == 2){ valueL = arrLeft[i].numCriticReviews; valueR = arrRight[j].numCriticReviews; }
		else if(category == 3){ valueL = arrLeft[i].duration; valueR = arrRight[j].duration; }
		else if(category == 4){ valueL = arrLeft[i].directorFBlikes; valueR = arrRight[j].directorFBlikes; }
		else if(category == 5){ valueL = arrLeft[i].actor3FBlikes; valueR = arrRight[j].actor3FBlikes; }
		else if(category == 6){ strVal = compareStrings(arrLeft[i].actor2name, arrRight[j].actor2name); }
		else if(category == 7){ valueL = arrLeft[i].actor1FBlikes; valueR = arrRight[j].actor1FBlikes; }
		else if(category == 8){ valueL = arrLeft[i].gross; valueR = arrRight[j].gross; }
		else if(category == 9){ strVal = compareStrings(arrLeft[i].genres, arrRight[j].genres); }
		else if(category == 10){ strVal = compareStrings(arrLeft[i].actor1name, arrRight[j].actor1name); }
		else if(category == 11){ strVal = compareStrings(arrLeft[i].movieTitle, arrRight[j].movieTitle); }
		else if(category == 12){ valueL = arrLeft[i].numVotedUsers; valueR = arrRight[j].numVotedUsers; }
		else if(category == 13){ valueL = arrLeft[i].castTotalFBlikes; valueR = arrRight[j].castTotalFBlikes; }
		else if(category == 14){ strVal = compareStrings(arrLeft[i].actor3name, arrRight[j].actor3name); }
		else if(category == 15){ valueL = arrLeft[i].faceNumberPoster; valueR = arrRight[j].faceNumberPoster; }
		else if(category == 16){ strVal = compareStrings(arrLeft[i].plotKeywords, arrRight[j].plotKeywords); }
		else if(category == 17){ strVal = compareStrings(arrLeft[i].link, arrRight[j].link); }
		else if(category == 18){ valueL = arrLeft[i].numUserReviews; valueR = arrRight[j].numUserReviews; }
		else if(category == 19){ strVal = compareStrings(arrLeft[i].language, arrRight[j].language); }
		else if(category == 20){ strVal = compareStrings(arrLeft[i].country, arrRight[j].country); }
		else if(category == 21){ strVal = compareStrings(arrLeft[i].contentRating, arrRight[j].contentRating); }
		else if(category == 22){ valueL = arrLeft[i].budget; valueR = arrRight[j].budget; }
		else if(category == 23){ valueL = arrLeft[i].titleYear; valueR = arrRight[j].titleYear; }
		else if(category == 24){ valueL = arrLeft[i].actor2FBlikes; valueR = arrRight[j].actor2FBlikes; }
		else if(category == 25){ dvalueL = arrLeft[i].imdbScore; dvalueR = arrRight[j].imdbScore; }
		else if(category == 26){ dvalueL = arrLeft[i].aspectRatio; dvalueR = arrRight[j].aspectRatio; }
		else if(category == 27){ valueL = arrLeft[i].movieFBlikes; valueR = arrRight[j].movieFBlikes; 
	}
			
		// Strings
		if(category == 0 || category == 1 || category == 6 || category == 9 || category == 10 || category == 11 || category == 14 || category == 16 || category == 17 || category == 19 || category == 20 || category == 21){
			if(strVal == 1){
				arr[k] = arrLeft[i];
				i++;
			} else {
				arr[k] = arrRight[j];
				j++;
			}
			k++;
		}
			
		// Ints
		if(category == 2 || category == 3 || category == 4 || category == 5 || category == 7 || category == 8 || category == 12 || category == 13 || category == 15 || category == 18 || category == 22 || category == 23 || category == 24 || category == 27){
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
		if(category == 25 || category == 26){
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
	
} //end of 'merge'

void mergesortHelper(movie *arr, int left, int right, int category) {
	
		if (left < right){
			int middle = left + ((right - left) / 2);
		
			mergesortHelper(arr, left, middle, category);
			mergesortHelper(arr, (middle + 1), right, category);
		
			merge(arr, left, middle, right, category);
		}

} //end of 'mergesortHelper'

movie *mergesort(movie *arr, int category, int size) {
	
	mergesortHelper(arr, 0, (size - 1), category);
	
	return arr;
	
} //end of 'mergesort'

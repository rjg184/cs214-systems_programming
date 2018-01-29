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
	
} //end of 'compareStrings' function

void NUMmerge(record *arr, int l, int m, int r, int colnum){
	
    	int i, j, k;
    	int n1 = m - l + 1; 	//size of left array
    	int n2 =  r - m;	//size of right array
 
    	record * tempL = malloc(sizeof(record)*n1); 		//temp arrays
	record * tempR = malloc(sizeof(record)*n2);	

    	for (i = 0; i < n1; i++){	//copy data to temp arrays
        	tempL[i] = arr[l+i];
	}
    	for (j = 0; j < n2; j++){
        	tempR[j] = arr[m+1+j];
	}
	
    	/* merge back into temp array*/
    	i = 0; // first index of first subarray
    	j = 0; // first index of second subarray
    	k = l; // first index of merged subarray

	switch(colnum){
		case(0):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].color,tempR[j].color);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(1):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].directorName,tempR[j].directorName);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(2): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].numCriticReviews <= tempR[j].numCriticReviews){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(3)://INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].duration <= tempR[j].duration){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(4): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].directorFBlikes <= tempR[j].directorFBlikes){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(5): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].actor3FBlikes <= tempR[j].actor3FBlikes){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(6):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].actor2name,tempR[j].actor2name);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(7): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].actor1FBlikes <= tempR[j].actor1FBlikes){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(8): //FLOAT/DOUBLE
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].gross <= tempR[j].gross){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(9):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].genres,tempR[j].genres);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(10):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].actor1name,tempR[j].actor1name);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(11): //CHAR
			//printf("working \n");
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].movieTitle,tempR[j].movieTitle);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(12): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].numVotedUsers <= tempR[j].numVotedUsers){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;		
		case(13): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].castTotalFBlikes <= tempR[j].castTotalFBlikes){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(14):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].actor3name,tempR[j].actor3name);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;	
		case(15): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].faceNumberPoster <= tempR[j].faceNumberPoster){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(16):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].plotKeywords,tempR[j].plotKeywords);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;	
		case(17):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].link,tempR[j].link);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;		
		case(18): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].numUserReviews <= tempR[j].numUserReviews){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;	
		case(19):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].language,tempR[j].language);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;		
		case(20):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].country,tempR[j].country);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(21):
			//CHAR
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
				int result = compareStrings(tempL[i].contentRating,tempR[j].contentRating);
        			if (result == 1){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(22): //FLOAT/DOUBLE
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].budget <= tempR[j].budget){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(23): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].titleYear <= tempR[j].titleYear){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(24): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].actor2FBlikes <= tempR[j].actor2FBlikes){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(25): //FLOAT/DOUBLE
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].imdbScore <= tempR[j].imdbScore){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(26): //FLOAT/DOUBLE
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].aspectRatio <= tempR[j].aspectRatio){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
		case(27): //INT
			while (i < n1 && j < n2){ //sees if i or j is larger and puts the smallest one into temp
        			if (tempL[i].movieFBlikes <= tempR[j].movieFBlikes){
            				arr[k] = tempL[i];	
            				i++;
        			}else{
            				arr[k] = tempR[j];
            				j++;
        			}
        			k++;
    			}
			break;
	}
	
	// Left
	while (i < n1){
		arr[k] = tempL[i];
        i++;
        k++;
    }
    
 	// Right
    	while (j < n2){
        	arr[k] = tempR[j];
        	j++;
        	k++;
    	}
	free(tempL);
	free(tempR);
	
} //end of 'NUMmerge'

void NUMmergeSorter(record *arr, int left, int right, int colnum) {
	
		if (left < right){
			int middle = left + ((right - left) / 2);
		
			NUMmergeSorter(arr, left, middle, colnum);
			NUMmergeSorter(arr, (middle + 1), right, colnum);
		
			NUMmerge(arr, left, middle, right, colnum);
		}

}

record *mergesort(record *records, int colnum, int size) { //splits array into arrays of one
	
	switch(colnum){
		case(0):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(1):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(2):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(3):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(4):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(5):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(6):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(7):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(8):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(9):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(10):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(11):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(12):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;		
		case(13):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(14):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;	
		case(15):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(16):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;	
		case(17):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;		
		case(18):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;	
		case(19):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;		
		case(20):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(21):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(22):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(23):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(24):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(25):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(26):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
		case(27):
			NUMmergeSorter(records,0,size-1 ,colnum);
			break;
	}
	
	return records;
	
} //end of 'mergesort'

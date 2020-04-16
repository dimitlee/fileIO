#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


/*insert_at writes the string buf into the file with descriptor
  fd, without overwriting the bytes that are already at the 
  offset where it is going to be written. n is the length 
  of string to be written.
 */
// This ils an sfj
int insert_at(int fd, off_t offset, char* buf, int n){
  
  //get the size of the file
  int sz = 0;
  sz = lseek(fd, 0, SEEK_END);
  printf("insert_at; The size of the file: %i\n", sz);
  
  //offset the file and see if it needs a temp buffer
  offset = lseek(fd, offset, SEEK_SET);
  if(offset < 0){
    printf("insert_at; Failure to offset the file\n");
    return -1;
  }
  int needTemp = 1;
  char* temp;
  int tempSz = 0;
  if(sz <= offset)
    needTemp = 0;
  
  //only allocate memory for temp, if there is something to write into it
  if(needTemp == 1){
    temp = (char*)malloc(sizeof(char)*(sz-offset));
    printf("insert_at; %ld chars allocated for temp\n", sz-offset);
    tempSz = read(fd, temp, sz - offset);
    printf("insert_at; Temporary buffer size: %i\n", tempSz);
  }
  
  //write the string into the file
  offset = lseek(fd, offset, SEEK_SET);
  int writB = write(fd, buf, n);
  if(writB < 0){
    printf("insert_at; Failure to write the string into the file\n");
    return -1;
  }
  printf("insert_at; Bytes written: %i. The string: %s\n", writB, buf);
  
  //getting the new offset
  offset = lseek(fd, 0, SEEK_CUR);
  if(offset < 0){
    printf("insert_at; Failure to offset the file\n");
    return -1;
  }

  
  //return if temp was not created
  if(needTemp == 0)
    return offset;

  //write temp back into the file and restore the offset
  if(write(fd, temp, tempSz) < 0){
    printf("insert_at; Failure to write temp back into the file\n");
    return -1;
  }
  offset = lseek(fd, offset, SEEK_SET);
  
  return offset;
}

int main(int argc, char** argv){

  //Only accept 5 arguments
  if(argc != 5){
    printf("Incorrect number of arguments!\n");
    return 1;
  }
  
  //argv arguments
  char* srcPath = *(argv + 1);
  char* destPath = *(argv + 2);
  int offset = atoi(*(argv + 3));
  int numBytes = atoi(*(argv + 4));
  
  //open source file
  int srcFd = open(srcPath, O_RDWR);
  if(srcFd == -1){
    printf("Cannot open source file\n");
    return 1;
  }
  printf("Source file descriptor: %i\n", srcFd);
  
  //get source file size
  int srcSz = lseek(srcFd, 0, SEEK_END);
  printf("Source file size: %i\n", srcSz);
  if(srcSz < offset + numBytes){
    printf("File is too small to read.");
    printf("Either decrease the offset or the num of bytes to read.\n");
    return 1;
  }
  
  //open destination file, if it does not exist create new one
  int destFd = open(destPath, O_RDWR|O_CREAT, S_IRWXU);
  if(destFd < 0){
    printf("Failure opening the source file\n");
    return 1;
  }
  
  /*if(destFd == -1){
    printf("Creating the destination file...\n");

    destFd = open(destPath, O_CREAT, S_IRWXU);
    if(destFd == -1){
      printf("Cannot create destination file\n");
      return 1;
    }
  }*/
  printf("Destination file descriptor: %i\n", destFd);
  
  
  //offset the file first
  int srcOff = lseek(srcFd, offset, SEEK_SET);
  printf("Source file, bytes offset: %i\n", srcOff);

  //alocate memory for buffer and read string from the file
  char* srcStr = (char*)malloc(sizeof(char)*numBytes);
  int srcRead = read(srcFd, srcStr, numBytes);
  printf("Source file, %i bytes read, the string is: %s\n", srcRead, srcStr);
  
  //replace bytes in the destFile with the bytes read from srcFile
  int currOffDest = insert_at(destFd, offset, srcStr, srcRead);
  
  //close files
  close(srcFd);
  close(destFd);

  return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/*the function replace_at  writes the string s consisting
  of n bytes into the file at filedescriptor fd
  at the given offset
 */
int replace_at(int fd, off_t offset, char* s, int n){
  int initOff, finOff;
  initOff = lseek(fd, offset, 0);
  if(initOff == -1){
    printf("Failure offseting the file\n");
    return -1;
  }
  printf("Dest file, bytes offset: %i\n", initOff);
  finOff = write(fd, s, n);
  if(finOff == -1){
    printf("Failure writing into the file\n");
    return -1;
  }else{
    printf("Dest file, bytes written: %i, string written: %s\n", finOff, s);
  }
  return initOff + finOff;
}

int main(int argc, char** argv){
  //accepting argv arguments
  char* srcPath = *(argv + 1);
  char* destPath = *(argv + 2);
  int offset = atoi(*(argv + 3));
  int numBytes = atoi(*(argv + 4));

  //open files
  int srcFd = open(srcPath, O_RDWR);
  if(srcFd == -1){
    printf("Cannot open source file\n");
    exit(1);
  }
  printf("Source file descriptor: %i\n", srcFd);
  int destFd = open(destPath, O_RDWR);
  if(destFd == -1){
    printf("Creating the destination file\n");
    destFd = open(destPath, O_CREAT, S_IRWXU);
    if(destFd == -1){
      printf("Cannot create destination file\n");
      exit(1);
    }
  }
  destFd = open(destPath, O_RDWR);
  printf("Destination file descriptor: %i\n", destFd);
  
  //read from source files at the offset
  int srcOff = lseek(srcFd, offset, 0);
  printf("Source file, bytes offset: %i\n", srcOff);
  char* srcStr = (char*)malloc(sizeof(char)*numBytes);
  int srcRead = read(srcFd, srcStr, numBytes);
  printf("Source file, %i bytes read, the string is: %s\n", srcRead, srcStr);
  
  //replace bytes in the destFile with the bytes read from srcFile
  int currOffDest = replace_at(destFd, offset, srcStr, srcRead);
  
  //close files
  close(srcFd);
  close(destFd);
}

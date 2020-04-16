#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int replace_at(int fd, off_t offset, char* s, int n){
  int lseekOff, writeOff;
  lseekOff = lseek(fd, offset, 0);
  if(lseekOff == -1)
    return -1;
  writeOff = write(fd, s, n);
  if(writeOff == -1)
    return -1;
  return writeOff + lseekOff;
}

int main(int argc, char** argv){

  if(argc != 4){
    printf("Invalid arguments\n");
    exit(1);
  }

  
  int fd = open(*(argv+1), O_RDWR);
  int offset = atoi(*(argv+2));
  char* str = *(argv+3);
  int len = strlen(str);

  if(fd<0){
    printf("Path directory is wrong or the file could not be opened\n");
    exit(1);
  }
  printf("%i\n", fd);

  
  if(replace_at(fd, offset, str, len) == -1){
    printf("Error writing into the file");
    exit(1);
  }
  close(fd);
  return 0;
}

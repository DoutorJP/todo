#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct{
  bool status;
  char desc[256];
  int number;
} todo;

bool write_data(char* filename, todo* data, int total);
todo* read_data(char* filename, int* total);
void add_todo(todo*);
void line();
void list(todo*);
void check_arg(int, char**, todo*);
int total = 0;
int n_users = 0;

int main(int argc, char** argv){
  todo* p;
  p = malloc(sizeof(todo)*256);
  check_arg(argc, argv, p);
  line();
  printf("status | number | description\n");
  line();
  list(p);
  if(write_data("save.bin", p, n_users))
	printf("Write ok!\n");
  else{
	printf("error writing to file\n");
	return 1;
  }
  //  free(file_data);
  free(p);
  return 0;
}


void add_todo(todo* p){
  printf("description:");
  fflush(stdin);
  fgets(p[n_users].desc, 256, stdin);
  fflush(stdin);

  p[n_users].status = false;
  p[n_users].number = n_users;
  n_users++;
  line();
}

void line(){
  for(int i = 0; i < 32; i++)
	putchar('-');
  putchar('\n');
}

void check_arg(int argc, char** argv, todo* p){
  if(strncmp(argv[1], "-n", 2) == 0){
	if(argc < 3)
	  add_todo(p);
	else{
	  int n_count = atoi(argv[2]);
	  for(int i = 0; i < n_count; i++)
		add_todo(p);
	}
  }
}
void list(todo* p){

  char status[6];
  #ifdef __unix__
  system("clear");
  #endif
  #ifdef _WIN32
  system("cls");
  #endif
  for(int i = 0; i < n_users; i++){
	(p[i].status == false) ? strncpy(status, "TODO", 5) : strncpy(status, "DONE", 5);
	fflush(stdout);
	printf("%s  | %d  | %s\n", status, p[i].number, p[i].desc);
	fflush(stdout);
	line();
  }  
}

bool write_data(char* filename, todo* data, int total){
  FILE* file = fopen(filename, "wb");
  if(file == NULL) return false;
  if(fwrite(&total, sizeof(int), 1, file) != 1) return false;
  if(fwrite(data, sizeof(todo), total, file) != total) return false;
  if(fclose(file) == EOF) return false;
  return true;
}

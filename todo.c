#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ANSI_RED     "\x1b[31;1m"
#define ANSI_NULL    "\x1b[0m"

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

void list_data(){
  todo* file_data;
  file_data = read_data("save.bin", &n_users);
  if(file_data == NULL) {
	printf(ANSI_RED"error reading from file\n"ANSI_NULL);
	exit(1);
  }
  for(int i = 0; i < n_users; i++)
	{
	  char status[6];
	  	(file_data[i].status == false) ? strncpy(status, "TODO", 5) : strncpy(status, "DONE", 5);
			printf("%s  | %d  | %s\n", status, file_data[i].number, file_data[i].desc);
			line();
	}
}

int main(int argc, char** argv){

  //	  list_data();
  todo* p;
  p = malloc(sizeof(todo)*256);
  check_arg(argc, argv, p);

  line();
  printf("status | number | description\n");
  line();
  list(p);

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
  if(!argv[1]){
	list_data();
	printf(ANSI_RED "no operation\n" ANSI_NULL);
	exit(0);
  }
  if(strncmp(argv[1], "-n", 2) == 0){
	if(argc < 3)
	  add_todo(p);
	
	else{
	  int n_count = atoi(argv[2]);
	  for(int i = 0; i < n_count; i++)
		add_todo(p);
	}
	if(write_data("save.bin", p, n_users))
	  printf("Write ok!\n");
	else{
	  printf(ANSI_RED"error writing to file\n"ANSI_NULL);
	  exit(1);
	}
  }
  if(strncmp(argv[1], "-p", 2) == 0)
	list_data();

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
  FILE* file = fopen(filename, "ab");
  if(file == NULL) return false;
  if(fwrite(&total, sizeof(int), 1, file) != 1) return false;
  if(fwrite(data, sizeof(todo), total, file) != total) return false;
  if(fclose(file) == EOF) return false;
  return true;
}
todo* read_data(char* filename, int* total){
  FILE* file = fopen(filename, "rb");
  if(file == NULL) return NULL;
  if(fread(total, sizeof(int), 1, file) != 1) return NULL;
  todo* data = malloc(sizeof(todo)* *total);
  if(fread(data, sizeof(todo), *total, file) != *total){
	free(data);
	return NULL;
  }
  if(fclose(file) == EOF){
	return NULL;
	free(data);
  }
  return data;
}

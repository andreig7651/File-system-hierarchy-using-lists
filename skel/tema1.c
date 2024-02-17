#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void touch (Dir* parent, char* name) {
	File *file=parent->head_children_files;
	if(file==NULL){
		file=malloc(sizeof(File));//cream primul fisier in lista de fisiere daca acesta nu exista
		if(file==NULL){
			printf("Could not allocate the file\n");
			free(file);
			return;
		}
		file->name=strdup(name);//alocam campurile fisierului
		if(file->name==NULL){
			printf("Could not allocate file name\n");
			free(file->name);
			return;
		}
		file->next=NULL;
		file->parent=parent;
		parent->head_children_files=file;
	}else{
		if(strcmp(name,file->name)==0){//daca fisierul exista si este primul in lista
				printf("File already exists\n");
				return;
			}
		
		for(;file->next!=NULL;file=file->next)//iteram prin lista de fisiere si verificam daca exista
			if(strcmp(name,file->name)==0){
				printf("File already exists\n");
				return;
			}
		
		for(;file->next!=NULL;)//avansam pana la ultimul fisier din lista
			file=file->next;
		
		File *new_file=malloc(sizeof(File));//alocam noul fisier si campurile sale
		if(new_file==NULL){
			printf("Could not allocate the file\n");
			free(new_file);
			return;
		}
		new_file->name=strdup(name);
		if(new_file->name==NULL){
			printf("Could not allocate the file name\n");
			free(new_file->name);
			return;
		}
		new_file->parent=parent;
		new_file->next=NULL; 
		file->next=new_file;//legam fisierul in lista		
	
	}
}

void mkdir (Dir* parent, char* name) {
	
	Dir *director=parent->head_children_dirs;
	
	if(director==NULL){//daca lista de directoare este vida

		director=malloc(sizeof(Dir));//alocam noul director si campurile aferente
		if(director==NULL){
			printf("Could not allocate the director\n");
			free(director);
			return;
		}
		director->name=strdup(name);
		if(director->name==NULL){
			printf("Could not allocate the director name\n");
			free(director->name);
			return;
		}
		director->next=NULL;
		director->head_children_dirs=NULL;
		director->head_children_files=NULL;
		director->parent=parent;
		parent->head_children_dirs=director;
	
	}else{
		
		if(strcmp(name,director->name)==0){//daca directorul exista deja si este primul
				printf("Directory already exists\n");
			return;
		}
		
		for(;director->next!=NULL;director=director->next)//iteram prin lista si cautam directorul
			if(strcmp(name,director->name)==0){
				printf("Directory already exists\n");
			return;
		}

		for(;director->next!=NULL;)//iteram pana la finalul listei
			director=director->next;
		
		director->next=malloc(sizeof(Dir));//cream noul director si ii alocam campurile
		if(director->next==NULL){
			printf("Could not allocate the new director name\n");
			free(director->next);
			return;
		}
		director->next->name=strdup(name);
		if(director->next->name==NULL){
			printf("Could not allocate the driector name\n");
			free(director->next->name);
			return;
		}
		director->next->parent=parent;
		director->next->head_children_files=NULL;
		director->next->head_children_dirs=NULL;
		director->next->next=NULL;
	}
}

void ls (Dir* parent) {
	
	for(Dir *director=parent->head_children_dirs;director!=NULL;director=director->next)//iteram prin lista de directoare
		printf("%s\n",director->name);//afisam numele tuturor directoarelor

	for(File *file=parent->head_children_files;file!=NULL;file=file->next)//iteram prin lista de fisiere
		printf("%s\n",file->name);//afisam numele tuturor fisierelor
}

void rm (Dir* parent, char* name) {

	File *file=parent->head_children_files;
	
	if(file==NULL){//daca lista de fisiere este vida
		printf("Could not find the file\n");
		free(file);
		return;
	}

	for(File *pre=NULL;file!=NULL;pre=file,file=file->next)//iteram prin lista de fisiere
		if(strcmp(name,file->name)==0){//cautam fisierul ce trebuie sters
			if(pre==NULL){//daca trebuie sters primul din lista
				if(file->next==NULL){//daca lista are doar un element
					parent->head_children_files=NULL;
					free(file->name);
					file->next=NULL;
					return;
				}else{
					file->parent=parent;//daca lista are mai multe elemente
					parent->head_children_files=file->next;
					return;
				}
				parent->head_children_files=file->next;//actualizam legatura si eliberam campurile
				free(file->name);
				file->next=NULL;
				free(file->parent);
				file=NULL;
				return;
			}
			else{
				pre->next=file->next;//daca trebuie sters un element din interiorul listei
				free(file->name);//eliberare campuri si actualizare legaturi
				file->next=NULL;
				free(file->next);
				free(file->parent);
				return;
			}
		}
	printf("Could not find the file\n");//daca lista nu e vida,dar nu gasim fisierul	
}

void rmdir (Dir* parent, char* name) {
	
	Dir *director=parent->head_children_dirs;
	
	if(director==NULL){//daca lista de directoare este vida
		printf("Could not find the dir\n");
		free(director);
		return;
	}

	for(Dir *pre=NULL;director!=NULL;pre=director,director=director->next)//iteram prin lista
		if(strcmp(name,director->name)==0){//daca gasim directorul
			if(pre==NULL){//daca primul element e de sters
				if(director->next==NULL){//daca lista are doar un element
					for(File *file=director->head_children_files;file!=NULL;file=file->next)
						rm(director,file->name);//stergem toae fisierele din director
					parent->head_children_dirs=NULL;//eliberam directorul
					director->next=NULL;
					director->head_children_files=NULL;
					
				}else{//daca trebuie sters primul,dar lista are mai multe elemente
					for(File *file=director->head_children_files;file!=NULL;file=file->next)
						rm(director,file->name);//stergem toate fisierele din director
					director->parent=parent;//actualizam legaturile
					parent->head_children_dirs=director->next;
				}

				free(director->head_children_dirs);//eliberam directorul
				free(director->head_children_files);
				free(director->name);
				free(director->next);
				free(director->parent);
				free(director);
				return;

			}else{
				
				for(File *file=director->head_children_files;file!=NULL;file=file->next)
					rm(director,file->name);//stergem toate fisierele din director

				pre->next=director->next;//actualizam legatura,element in interior
				free(director->name);//eliberam campurile
				director->next=NULL;
				free(director->next);
				free(director->parent);
				free(director);
				return;
			}
		}

	printf("Could not find the dir\n");//parcurgem toata lista si nu gasim directorul

}

void cd(Dir** target, char *name) {
	
	if(strcmp(name,"..")==0){//daca primim ..
		if((*target)->parent==NULL){//daca ne aflam in home
			return;
		}else{
		*target=(*target)->parent;//altfel,schimbam la parinte
		return;
		}
	}
	
	for(Dir* director=(*target)->head_children_dirs;director!=NULL;director=director->next)
		if(strcmp(director->name,name)==0){//cautam directorul
			*target=director;//ne schimbam in el
			return;
		}
	
	printf("No directories found!\n");//nu am gasit directorul
}

char *pwd (Dir* target) {

	if(target->parent==NULL){//conditie de iesire din recursivitate
		printf("/%s",target->name);//afisam home
		return 0;
	}
	pwd(target->parent);//reapelam functia
		printf("/%s",target->name);//afisam mereu directorul in care ne aflam
}

void stop (Dir* target) {
	return;
}

void tree (Dir* target, int level) {
	
	if(target->head_children_dirs==NULL){//daca nu avem directoare in director
		if(target->parent->parent==NULL)//iar directorul este fiu al lui home 
			printf("%s\n",target->name);//il afisam
		if(target->parent->head_children_files!=NULL && level==0)//daca directorul nu contine fisiere
			printf("%s\n",target->parent->head_children_files->name);//afisam celelalte fisiere din home
		return;
	}
	
	int mylevel=level+1,newlevel=level+1;//salvam nivelul curent
	
	if(target->parent->parent==NULL)//daca suntem in home
		printf("%s\n",target->name);

	while(mylevel){//afisam spatierea
		printf("    ");
		mylevel=mylevel-1;//in functie de nivel
	}

	printf("%s\n",target->head_children_dirs->name);//afisam primul director din directorul curent
	tree(target->head_children_dirs,level+1);//reapelam functia

	while(newlevel){
		printf("    ");
		newlevel=newlevel-1;//afisam spatii din nou
	}

	if(target->head_children_files!=NULL)//afisam fisierele
		printf("%s\n",target->head_children_files->name);
	
}

void mv(Dir* parent, char *oldname, char *newname) {
	
	for(File *file=parent->head_children_files;file!=NULL;file=file->next){//cautam fisierul dupa numele nou

			if(strcmp(file->name,newname)==0){//daca exista deja
				printf("File/Director already exists\n");
				return;
			}
	}
	
	for(Dir *director=parent->head_children_dirs;director!=NULL;director=director->next){//cautam directorul dupa numele nou
			
			if(strcmp(director->name,newname)==0){//daca exista deja
				printf("File/Director already exists\n");
				return;
			}
			
	}

	for(Dir *director=parent->head_children_dirs;director!=NULL;director=director->next){
			
			if(strcmp(director->name,oldname)==0){//daca gasim directorul vechi in lista
				rmdir(director->parent,oldname);//il stergem din lista
				mkdir(director->parent,newname);//il readaugam dupa numele nou
				return;
			}
	}

	for(File *file=parent->head_children_files;file!=NULL;file=file->next){

			if(strcmp(file->name,oldname)==0){//daca gasim fisierul vechi in lista
				rm(file->parent,oldname);//il stergem din lista
				touch(file->parent,newname);//il readaugam in lista
				return;
			}
	}

	printf("File/Director not found\n");//daca nu am gasit nimic in liste
	return;
}

int main () {
	char *line=NULL;
	line = malloc(MAX_INPUT_LINE_SIZE * sizeof(char));//alocam linia pe care o vom citi
	if (line == NULL) {//verificam alocarea
		printf("Error reading line\n");
		free(line);
		return 0;
	}
	Dir *home=malloc(sizeof(Dir));//alocam directorul home
	if (home == NULL) {//verificam alocarea
		printf("Error allocateing home\n");
		free(home);
		return 0;
	}
	home->head_children_dirs=NULL;
	home->head_children_files=NULL;
	home->name=strdup("home");
	if (home->name == NULL) {//verificam alocarea
		printf("Error allocating home name\n");
		free(home->name);
		return 0;
	}
	home->next=NULL;
	home->parent=NULL;
	
	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		char *operatia = strtok(line, " ");//primul cuvant de pe fiecare linie din fisier
			if(operatia)
				if (operatia[strlen(operatia) - 1] == '\n')//escapare caracter final
					operatia[strlen(operatia) - 1] = '\0';
		
		char *nume = strtok(NULL, " ");//al doilea cuvant de pe fiecare linie din fisier
			if(nume)
				if (nume[strlen(nume) - 1] == '\n')//escapare caracter final
					nume[strlen(nume) - 1] = '\0';
		
		if(operatia){
			
			if (strcmp(operatia, "touch") == 0) {//cazul touch
				touch(home,nume);
				free(operatia);//eliberare memorie
				free(nume);
				free(line);	
				continue;//trecem la iteratia urmatoare
			}
			
			if (strcmp(operatia, "mkdir") == 0) {
				mkdir(home,nume);
				free(operatia);
				free(nume);
				free(line);	
				continue;
			}
			
			if (strcmp(operatia, "ls") == 0) {
				ls(home);
				free(operatia);
				free(line);	
				continue;
			}
			
			if (strcmp(operatia, "rm") == 0) {
				rm(home,nume);	
				free(operatia);
				free(nume);
				free(line);
				continue;
			}
			
			if (strcmp(operatia, "rmdir") == 0) {
				rmdir(home,nume);	
				free(operatia);
				free(nume);
				free(line);
				continue;
			}
			
			if (strcmp(operatia, "cd") == 0) {
				cd(&home,nume);	
				free(operatia);
				free(nume);
				free(line);
				continue;
			}

			if(strcmp(operatia,"stop")==0){
				stop(home);
				free(operatia);
				free(line);
				continue;
			}

			if(strcmp(operatia,"pwd")==0){
				pwd(home);
				free(operatia);
				free(line);
				continue;
			}

			if(strcmp(operatia,"tree")==0){
				for(Dir *director=home->head_children_dirs;director!=NULL;director=director->next)
					tree(director,0);//apelam tree pe fiecare director din lista
				free(operatia);
				free(line);
				continue;
			}

			if(strcmp(operatia,"mv")==0){
				char *nume1 = strtok(NULL, " ");//al treilea cuvant de pe linie
				if(nume1)
					if (nume1[strlen(nume1) - 1] == '\n')//escapare caracter final
						nume1[strlen(nume1) - 1] = '\0';
				mv(home,nume,nume1);
				free(nume1);
				free(operatia);
				free(nume);
				free(line);
				continue;
			}

		}				
	
	
	} while (/*condition*/fgets(line, MAX_INPUT_LINE_SIZE, stdin) != NULL);
	
	free(home->head_children_dirs);//eliberare memorie
	free(home->head_children_files);
	free(home->name);
	free(home->next);
	free(home->parent);
	free(home);
	
	return 0;
}

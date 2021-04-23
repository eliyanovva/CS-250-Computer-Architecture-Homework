#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node{
	char name[100];
	struct node* left;
	struct node* right;
};

struct node* set_node(char* word){
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	strcpy(new_node->name, word);
	new_node->right = NULL;
	new_node->left = NULL;

	return new_node;
}

struct node* find(struct node* root, char* name){
	struct node* found = NULL;
	if(root==NULL)
		return NULL;
	if(strcmp(root->name, name)==0)
		return root;

	found = find(root->left, name);
	if(found)
		return found;
	found = find(root->right, name);
	if(found)
		return found;
	return NULL;

}


struct node* insert(struct node* root, char* name, char* source){
	struct node* m = find(root, source);
	if(m==NULL){
		root = set_node(name);
		return root;
	}

	if(m->left == NULL)
		m->left = insert(m->left, name, source);

	else{
		int flag = strcmp(m->left->name, name);

		if(flag>0){
			struct node* temp = m->left;
			m->right = temp;
			m->left = set_node(name);
		}

		else if(flag<0)
			m->right = insert(m->right, name, source);
	}
	//free(m);

	return root;
}


void print(struct node* root){
	if(root==NULL)
		return;
	printf("%s\n", root->name);
	print(root->left);
	print(root->right);
}

void free_tree(struct node* tree){
	if(tree==NULL)
		return;
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}


int main(int argc, char* argv[]){

	FILE *fp;
	char* line = (char*)malloc(1000);
	struct node* tree = NULL;

	char* file = argv[1];

	fp = fopen(file, "r");
	while(fgets(line, 200, fp)!=NULL){
		char* ill = (char*)malloc(300);
		char* source  = (char*)malloc(300);
		int k=0;

		while(*(line+k)!='\0' && *(line+k)!=' '){
			*(ill+k) = *(line+k);
			k++;
		}
		*(ill+k) = '\0';
		//printf("%s\n",ill);
		if(ill[0]=='D' && ill[1]=='O'&&ill[2]=='N'&&ill[3]=='E'){
		//if(strcmp(ill, "DONE\0")==0){
		//	printf("bachka cboro");
			//free(line);
			free(ill);
			free(source);
			break;
		}

		k++;
		int temp=0;
		while(*(line+k)!='\0'&&*(line+k)!=' '&& *(line+k)!='\n'){
			*(source + temp) = *(line+k);
			k++;
			temp++;
		}

		*(source + temp) = '\0';

		if(tree ==NULL){
			tree = set_node(source);
			tree = insert(tree, ill, source);
		}

		else
			tree = insert(tree, ill, source);

		free(ill);
		free(source);
		//free(line);
	
	}


	print(tree);
	fclose(fp);
	free(line);
	free_tree(tree);

	return 0;
}

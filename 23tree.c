#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX(A, B) (((A)>(B)) ? (A) : (B))

struct tnode {
	int key[3];
	char name[3][16];
	struct tnode *s, *m, *l, *t; /* t for temp(largest) child before splitting */
	struct tnode *par;
};

struct tdata {
	int key;
	char name[16];
};

struct tnode *root = NULL;

void reconnect(struct tnode *cur, struct tnode *temp, int median) 
{
	/* if cur is small child */
	if(cur->par->s && cur->key == cur->par->s->key) {
		/* adopt children */
		if(median == 0) {
			temp->s = cur->s;
			temp->m = cur->m;
			temp->l = temp->t = NULL;
			cur->s = cur->l;
			cur->m = cur->t;
			cur->l = cur->t = NULL;
		} else if(median == 1 || median == 2) {
			temp->s = cur->l;
			temp->m = cur->t;
			temp->l = temp->t = NULL;
			cur->s = cur->s;
			cur->m = cur->m;
			cur->l = cur->t = NULL;
		} 

		if(cur->par->m && !cur->par->l) {
			cur->par->l = cur->par->m;
			if(median == 0) {
				cur->par->s = temp;
				cur->par->m = cur;	
			}
			else if(median == 1 || median == 2) {
				cur->par->s = cur;
				cur->par->m = temp;
			}
		} else if(cur->par->m && cur->par->l) {
			cur->par->t = cur->par->l;
			cur->par->l = cur->par->m;
			if(median == 0) {
				cur->par->s = temp;
				cur->par->m = cur;	
			}
			else if(median == 1 || median == 2) {
				cur->par->s = cur;
				cur->par->m = temp;
			}
		}
	/* if cur is median child */
	} else if(cur->par->m && cur->key == cur->par->m->key) {
		if(median == 0) {
			temp->s = cur->s;
			temp->m = cur->m;
			temp->l = temp->t = NULL;
			cur->s = cur->l;
			cur->m = cur->t;
			cur->l = cur->t = NULL;
		} else if(median == 1 || median == 2) {
			temp->s = cur->l;
			temp->m = cur->t;
			temp->l = temp->t = NULL;
			cur->s = cur->s;
			cur->m = cur->m;
			cur->l = cur->t = NULL;
		} 

		if(!cur->par->l) {
			if(median == 0) {
				cur->par->m = temp;
				cur->par->l = cur;
			}
			if(median == 1 || median == 2) {
				cur->par->m = cur;
				cur->par->l = temp;
			}
		} else {
			cur->par->t = cur->par->l;
			if(median == 0) {
				cur->par->m = temp;
				cur->par->l = cur;
			}
			else if(median == 1 || median == 2) {
				cur->par->m = cur;
				cur->par->l = temp;
			}
		}

	/* if cur is large child */
	} else if(cur->par->l && cur->key == cur->par->l->key) {
		if(median == 0) {
			temp->s = cur->s;
			temp->m = cur->m;
			temp->l = temp->t = NULL;
			cur->s = cur->l;
			cur->m = cur->t;
			cur->l = cur->t = NULL;
		} else if(median == 1 || median == 2) {
			temp->s = cur->l;
			temp->m = cur->t;
			temp->l = temp->t = NULL;
			cur->s = cur->s;
			cur->m = cur->m;
			cur->l = cur->t = NULL;
		} 		
		
		if(median == 0) {
			cur->par->l = temp;
			cur->par->t = cur;
		} else if(median == 1 || median == 2) {
			cur->par->l = cur;
			cur->par->t = temp;
		}
	}
}

void pushup_split(struct tnode *cur, int median) 
{
	int i;
	struct tnode *temp, *temp2;
	/* push up */
	if(cur->par) {
		cur->par->key[2] = cur->key[median];
		/*strcpy(&(cur->par->name[2]), &(cur->name[median]));*/

		/* split */
		if(median == 0) {
			cur->key[0] = cur->key[1];
			/*strcpy(&(cur->name[0]), &(cur->name[1]));*/
			cur->key[1] = 0;
			/*cur->name = NULL;*/
			temp = (struct tnode *)malloc(sizeof(struct tnode));
			temp->key[0] = cur->key[2];
			cur->key[2] = 0;
			/*strcpy(&(temp->name[0]), &(cur->name[2]));*/
			temp->s = temp->m = temp->l = temp->t = NULL;
			temp->par = cur->par;
			/* cur is left child of par */
			reconnect(cur, temp, median);
		} else if(median == 1) {
			cur->key[1] = 0;
			/*cur->name = NULL;*/
			temp = (struct tnode *)malloc(sizeof(struct tnode));
			temp->key[0] = cur->key[2];
			cur->key[2] = 0;
			/*strcpy(&(temp->name[0]), &(cur->name[2]));*/
			temp->s = temp->m = temp->l = temp->t = NULL;
			temp->par = cur->par;
			reconnect(cur, temp, median);
		} else /* if median == 2 */ {
			temp = (struct tnode *)malloc(sizeof(struct tnode));
			temp->par = cur->par;
			temp->key[0] = cur->key[1];
			cur->key[1] = 0;
			/*strcpy(&(temp->name[0]), &(cur->name[1]));*/
			temp->s = temp->m = temp->l = temp->t = NULL;
			temp->par = cur->par;
			reconnect(cur, temp, median);
		}	
	} else {
		temp = (struct tnode *)malloc(sizeof(struct tnode));
		cur->par = temp;
		temp->key[0] = cur->key[median];
		/*strcpy(&(temp->name[2]), &(cur->name[median]));*/
		temp2 = (struct tnode*)malloc(sizeof(struct tnode));
		if(median == 0) {
			temp2->key[0] = cur->key[2];
			temp2->key[1] = temp2->key[2] = 0;
			temp2->s = temp2->m = temp2->l = temp2->t = NULL;
			cur->key[0] = cur->key[1];
			cur->key[1] = cur->key[2] = 0;
			reconnect(cur, temp2, median);
		} else if(median == 1) {
			temp2->key[0] = cur->key[2];
			temp2->key[1] = temp2->key[2] = 0;
			temp2->s = temp2->m = temp2->l = temp2->t = NULL;
			cur->key[0] = cur->key[0];
			cur->key[1] = cur->key[2] = 0;
			reconnect(cur, temp2, median);
		} else if(median == 2) {
			temp2->key[0] = cur->key[1];
			temp2->key[1] = temp2->key[2] = 0;
			temp2->s = temp2->m = temp2->l = temp2->t = NULL;
			cur->key[0] = cur->key[0];
			cur->key[1] = cur->key[2] = 0;
			reconnect(cur, temp2, median);
		}
		root = temp;
	}
}

void xinsert(struct tnode **pn, struct tdata tn)
{
	int median, t;
	struct tnode *cur = *pn, *temp;
	/* if cur is root and null , that is cur is root */
	if(!root) {
		root = (struct tnode *)malloc(sizeof(struct tnode));
		root->key[0] = tn.key;
		root->key[1] = 0;
		root->key[2] = 0;
		/*strcpy(&temp->name[0], tn.name);*/
		root->s = root->m = root->l = root->t = NULL;
		return;
	}
	/* if leaf */
	if(!cur->s && !cur->m && !cur->l) {
		/* if cur is full */
		if(cur->key[0] && cur->key[1]) {
			cur->key[2] = tn.key;
			/*strcpy(&(cur->name[2]), tn.name);*/
			/* pick up median key and 
			   push it up to the par node
			 */
			if(tn.key < cur->key[0]) {
				median = 0;
			} else if(tn.key > cur->key[1]) {
				median = 1;
			} else median = 2;

			pushup_split(cur, median);
		} else if(cur->key[0] > tn.key) {
			cur->key[1] = cur->key[0];
			/* cur->name[2] = cur->name[0] */
			cur->key[0] = tn.key;
		} else {
			cur->key[1] = tn.key;
		}
		return;
	} else if(tn.key < cur->key[0]) {
		xinsert(&cur->s, tn);
	} else if(cur->key[1] && tn.key >= cur->key[0] && tn.key < cur->key[1]) {
		xinsert(&cur->m, tn);
	} else if(cur->key[1] && tn.key >= cur->key[1]) {
		xinsert(&cur->l, tn);
	} else if(!cur->key[1]) {
		xinsert(&cur->m, tn);
	}

	/* if there is a pushup from the previous */
	if(cur->key[2]) {
		if(cur->key[1]) {
			if(cur->key[0] > cur->key[2]) {
				median = 0;
			} else if(cur->key[1] < cur->key[2]) {
				median = 1;
			} else median = 2;

			pushup_split(cur, median); 
		} else {
			if(cur->key[0] > cur->key[2]) {
				cur->key[1] = cur->key[0];
				cur->key[0] = cur->key[2];
				cur->key[2] = 0;
			} else {
				cur->key[1] = cur->key[2];
				cur->key[2] = 0;
			}
		}
	}
}

int main()
{
	struct tdata tn;
	tn.key = 50;
	xinsert(&root, tn);
	tn.key = 60;
	xinsert(&root, tn);
	tn.key = 70;
	xinsert(&root, tn);
	tn.key = 40;
	xinsert(&root, tn);
	tn.key = 30;
	xinsert(&root, tn);
	tn.key = 20;
	xinsert(&root, tn);
	tn.key = 10;
	xinsert(&root, tn);
	tn.key = 80;
	xinsert(&root, tn);
	tn.key = 100;
	xinsert(&root, tn);
	return 0;
}

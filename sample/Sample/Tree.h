#pragma once

/*
Build hierarchical structure which is
a binary tree constructed by recursively partitioning the cloth 
evenly on the material coordinates until each node contains only a single triangle.
*/

typedef struct node{
	struct node *left;
	int data;
	struct node *right;
 }Node;

enum CUT {Wdir, Hdir};

void BuildTree(Node **root, int start, int num_particles_width, int num_particles_height, int original_width, CUT dir)
{
	(*root) = (Node*)malloc(sizeof(Node));
	(*root)->data = -1;
	(*root)->left = NULL;
	(*root)->right = NULL;

	if (num_particles_width == 2 && num_particles_height == 2)
	{
		(*root)->left = (Node*)malloc(sizeof(Node));
		(*root)->right = (Node*)malloc(sizeof(Node));
		(*root)->left->data = start;
		(*root)->right->data = start + 1;
		return;
	}

	if (num_particles_width == 2)
		dir = Wdir;
	if (num_particles_height == 2)
		dir = Hdir;

	if (dir == Wdir) //가로 분할
	{
		int divided_height = (int)(num_particles_height / 2) + 1;
		BuildTree(&(*root)->left, start, num_particles_width, divided_height, original_width, Hdir);
		BuildTree(&(*root)->right, start + 2 * (original_width - 1)*(divided_height - 1), num_particles_width, num_particles_height - divided_height + 1, original_width, Hdir);
	}

	else if (dir == Hdir) //세로 분할
	{
		int divided_width = (int)(num_particles_width / 2) + 1;
		BuildTree(&(*root)->left, start, divided_width, num_particles_height, original_width, Wdir);
		BuildTree(&(*root)->right, start + 2 * (divided_width - 1), num_particles_width - divided_width + 1, num_particles_height, original_width, Wdir);
	}
}
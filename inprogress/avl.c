#include <stdlib.h>
#include <stdio.h>

typedef struct node node;
typedef struct tree tree;

struct node {
	int key;
	unsigned height;
	int bal;
	node *l; // left child
	node *r; // right child
	node *p; // parent
};

struct tree {
	node *root;
  unsigned size;
};

void printAVL_(node *n, unsigned depth) {
	if(n) {
		printAVL_(n->r, depth + 1);
		int i;
		for(i = 0; i < depth; i++) {
			printf("     ");
		}
		printf("%d (%d)\n", n->key, n->height);
    printAVL_(n->l, depth + 1);
	}
}

void printAVL(tree *t) {
  if(t) {
    printf("Tree of size: %d\nBEGIN TREE\n", t->size);
  	printAVL_(t->root, 0);
    printf("END TREE\n");
  }
}

unsigned r_height(node *n) {
	if(!n) {
		return 0;
	}
	int l = r_height(n->l);
	int r = r_height(n->r);
	n->height = (l > r ? l : r) + 1;
  n->bal = r - l;
	return n->height;
}

int height(node *n) {
  if(n) {
	 return n->height;
  } else {
    return 0;
  }
}

int getBal(node *n) {
  if(n) {
    return height(n->r) - height(n->l);
  } else {
    return 0;
  }
}

node *newNode(int key) {
	node *n = malloc(sizeof(node));
	if(n) {
		n->l = NULL;
		n->r = NULL;
		n->p = NULL;
		n->key = key;
		n->bal = 0;
		n->height = 1;
	}
	return n;
}

node *rotateWithLeftChild(node *n) {
  if(n && n->l) {
    node *parent = n->p;
    node *left = n->l;
    node *node = n;
    node->l = left->r;
    node->p = left;
    left->p = parent;
    left->r = node;
    node->height = height(node->l) > height(node->r) ? height(node->l) + 1 : height(node->r) + 1;
    left->height = height(left->l) > height(left->r) ? height(left->l) + 1 : height(left->r) + 1;
    node->bal = getBal(node);
    left->bal = getBal(left);
    return left;
  } else {
    return n;
  }
}

node *rotateWithRightChild(node *n) {
  if(n && n->r) {
    node *parent = n->p;
    node *right = n->r;
    node *node = n;
    node->r = right->l;
    node->p = right;
    right->p = parent;
    right->l = node;
    node->height = height(node->l) > height(node->r) ? height(node->l) + 1 : height(node->r) + 1;
    right->height = height(right->l) > height(right->r) ? height(right->l) + 1 : height(right->r) + 1;
    node->bal = getBal(node);
    right->bal = getBal(right);
    return right;
  } else {
    return n;
  }
}

node *insert_(node *n, int key) {
  if(n == NULL) {
    return newNode(key);
  }

  if(key < n->key) {
    if((n->l = insert_(n->l, key))) {
      n->l->p = n;
    }
  } else {
    if((n->r = insert_(n->r, key))) {
      n->r->p = n;
    }
  }

  n->height = height(n->l) > height(n->r) ? height(n->l) + 1 : height(n->r) + 1;

  n->bal = getBal(n);

  if(n->bal > 1 && key > n->r->key) {
    return rotateWithRightChild(n);
  } else if(n->bal < -1 && key < n->l->key) {
    return rotateWithLeftChild(n);
  } else if(n->bal > 1 && key < n->r->key) {
    n->r = rotateWithLeftChild(n->r);
    return rotateWithRightChild(n);
  } else if(n->bal < -1 && key > n->l->key) {
    n->l = rotateWithRightChild(n->l);
    return rotateWithLeftChild(n);
  }

  return n;
}

node *find_(node *n, int key) {
  if(n == NULL) {
    return NULL;
  }

  if(key == n->key) {
    return n;
  } else if (key < n->key) {
    return find_(n->l, key);
  } else {
    return find_(n->r, key);
  }
}

node *find(tree *t, int key) {
  return find_(t->root, key);
}

void insert(tree *t, int key) {
  /* only insert if key is not present */
  if(t && !find(t, key)) {
    t->root = insert_(t->root, key);
    t->size++;
  }
}

int leastVal(node *n) {
  if(n->l) {
    return leastVal(n->l);
  } else {
    return n->key;
  }
}

node *delete_(node *n, int key) {
  if(n == NULL) {
    return NULL;
  }

  if(n->key == key) {
    if(n->l == NULL && n->r == NULL) {
      free(n);
      return NULL;
    } else if (n->l == NULL) {
      node *right = n->r;
      free(n);
      return right;
    } else if(n->r == NULL) {
      node *left = n->l;
      free(n);
    } else {
      /* hardest case... n->guaranteed not null */
      int successor = leastVal(n->r);
      n->r = delete_(n->r, successor);
      n->key = successor;
    }
  } else if (key < n->key) {
    n->l = delete_(n->l, key);
  } else if (key > n->key) {
    n->r = delete_(n->r, key);
  }

  n->height = height(n->l) > height(n->r) ? height(n->l) + 1 : height(n->r) + 1;

  n->bal = getBal(n);

  if(n->bal > 1 && getBal(n->r) >= 0) {
    return rotateWithRightChild(n);
  } else if(n->bal < -1 && getBal(n->l) <= 0) {
    return rotateWithLeftChild(n);
  } else if(n->bal > 1 && getBal(n->r) < 0) {
    n->r = rotateWithLeftChild(n->r);
    return rotateWithRightChild(n);
  } else if(n->bal < -1 && getBal(n->l) > 0) {
    n->l = rotateWithRightChild(n->l);
    return rotateWithLeftChild(n);
  }
  return n;
}

void delete(tree *t, int key) {
  node *n = find(t, key);
  if(n) {
    t->root = delete_(t->root, key);
    t->size--;
  }
}

tree *init_tree() {
  tree *t = malloc(sizeof(tree));
  if(t) {
    t->size = 0;
    t->root = NULL;
  }
  return t;
}

int assertTreeIsBalanced_(node *n) {
  int ret = 1;
  if(n) {
    if(n->r) {
      ret &= (n->r->key > n->key ? 1 : 0);
      ret &= assertTreeIsBalanced_(n->r);
    }
    if(n->l) {
      ret &= (n->l->key < n->key ? 1 : 0);
      ret &= assertTreeIsBalanced_(n->l);
    }
    if(n->bal <= 1 && n->bal >= -1) {
      ret &= 1;
    } else {
      ret &= 0;
    }
  }
  return ret;
}

void assertTreeIsBalanced(tree *t) {
  r_height(t->root);
  if(assertTreeIsBalanced_(t->root)) {
    printf("Tree is balanced and sorted correctly\n");
  } else {
    printf("Tree is either not balanced or not sorted correctly\n");
  }
}

int main(int argc, char *argv[]) {
  tree *t = init_tree();
  int i;
  unsigned j = 5;
  for(i = 0; i < 25; i++) {
    insert(t, j);
    j += 7;
    j = j % 31;
  }

  printf("Tree size: %u\n", t->size);

  j = 12;
  for(i = 0; i < 5; i++) {
    delete(t, j);
    j += 7;
    j = j % 31;
    assertTreeIsBalanced(t);
  }

  printf("Tree size: %u\n", t->size);
  return 0;
}



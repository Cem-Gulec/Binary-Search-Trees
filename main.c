/*Designed and programmed by - Cem Gulec - 150117828
 *
 * Constructed tree:
 *                18
 *         /              \
 *       12                36
 *      /  \              /  \
 *     8    16          24    72
 *    /    /  \        /     /
 *   4   14    17     20    54
 *  /     \             \
 * 2       15            21
 *
 * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct bst_tree {
    int data;
    struct bst_tree *left, *right;
};
typedef struct bst_tree node;

node * root = NULL;
int numArray[20], i=0, num, decision;
FILE *fptr;

/*prototypes*/
node * insert(node * tree, int val);
node * delete(node * tree, int val);
void print_lnr(node * tree);
node * getMin(node *tree);
node * getMax(node *tree);
int getMaxDepth(node *tree);
int getSizeLeft(node * tree);
int getSizeRight(node * tree);
int getSize(node * tree);
node * traceNode (node *tree, int val);
int checkTerm(int val);
int checkDepth(node *tree, int val, int level);
void deleteByTheUserEntered();
void tracePathByTheUserEntered();
void splitString(char []);

void main()
{
    char str[200];
    printf("Enter your sequence of numbers: ");
    gets(str);
    splitString(str);

    /* Printing nodes of tree in LNR fashion */
    printf("LNR display\n");
    print_lnr(root);
    printf("\nmax depth level: %d\n",getMaxDepth(root));
    printf("\nleft size of the root:%d\n",getSizeLeft(traceNode(root,36)));
    printf("\nright size of the root:%d\n",getSizeRight(traceNode(root,36)));
    printf("\nsize of the root: %d\n",getSize(traceNode(root,36)));
    printf("depth level: %d\n",checkDepth(root,72,1));

    printf("Press (1) to delete a node\nPress(2) to trace path of a nodepress\n(-1) to exit\nDecision: ");
    scanf("%d",&decision);

    while(decision != -1){
        switch (decision){
            case 1:
                deleteByTheUserEntered();
                break;
            case 2:
                tracePathByTheUserEntered();
                break;
            case -1:
                exit(-1);
        }

        printf("\nPress (1) to delete a node\nPress(2) to trace path of a nodepress\n(-1) to exit\nDecision: ");
        scanf("%d",&decision);

    }
    fclose(fptr);
}

//inserting a new node into tree
node * insert(node * tree, int val){
    //creating a node in order to place the value
    if( tree == NULL ){
        node *tmp = (node *)malloc(sizeof(node));
        tmp->data = val;
        numArray[i++]=val;
        tmp->left = tmp->right = NULL;
        tree = tmp;
        tmp = NULL;
    }
        //looking for the right place to put node, iterating through its left, middle and right branches
    else if( val < tree->data ){
        tree->left = insert(tree->left,val);
    }
    else if( val >= tree->data ){
            tree->right =  insert(tree->right,val);

    }else{}

    return tree;
}

//deleting a node
node * delete(node * tree, int val){
    // base case
    if (tree == NULL) return tree;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if (val < tree->data)
        tree->left = delete(tree->left, val);

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
    else if (val > tree->data)
        tree->right = delete(tree->right, val);

        // if key is same as root's key, then This is the node
        // to be deleted
    else
    {
        // node with only one child or no child
        if (tree->left == NULL)
        {
            node *temp = tree->right;
            free(tree);
            return temp;
        }
        else if (tree->right == NULL)
        {
            node *temp = tree->left;
            free(tree);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        node* temp = getMin(tree->right);

        // Copy the inorder successor's content to this node
        tree->data = temp->data;

        // Delete the inorder successor
        tree->right = delete(tree->right, temp->data);
    }
    return tree;
}

//print all the nodes in LNR fashion
void print_lnr(node * tree){
    if (tree){
        print_lnr(tree->left);
        printf("%d\n",tree->data);
        print_lnr(tree->right);
    }
}

//get left most node by iterating recursively
node * getMin(node *tree){
    node *currentNode = tree;
    while (currentNode->left != NULL)
        currentNode = currentNode->left;

    return currentNode;
}

//get right most node by iterating recursively
node * getMax(node *tree){
    node *currentNode = tree;
    while (currentNode->right != NULL)
        currentNode = currentNode->right;

    return currentNode;
}

//a function to find out max depth level
int getMaxDepth(node *tree){
    if (tree==NULL)
        return 0;
    else
    {
        /* compute the depth of each subtree */
        int left_depth = getMaxDepth(tree->left);
        int right_depth = getMaxDepth(tree->right);

        /* use the larger one */
        if (left_depth > right_depth)
            return(left_depth+1);
        else return(right_depth+1);
    }
}

int getSizeLeft(node * tree){
    if (tree==NULL)
        return 0;
    else
    {
        int count = 0;
        if (tree->left != NULL)
            count += 1 + getSizeLeft(tree->left);
        if (tree->right != NULL)
            count += getSizeLeft(tree->right);
        return count;
    }
}

int getSizeRight(node * tree){
    if (tree==NULL)
        return 0;
    else
    {
        int count = 0;
        if (tree->left != NULL)
            count += getSizeRight(tree->left);
        if (tree->right != NULL)
            count += 1 + getSizeRight(tree->right);
        return count;
    }
}

//a function to learn size of a node
int getSize(node * tree){
    if (tree==NULL)
        return 0;
    else
    {
        int left_size = getSize(tree->left);
        int right_size = getSize(tree->right);
        return (left_size + right_size +1);
    }
}

//trace a node and return it
node * traceNode (node *tree, int val){
    if (tree == NULL || tree->data == val)
        return tree;

    if (tree->data < val)
        return traceNode(tree->right, val);

    return traceNode(tree->left, val);
}

//this function checks if the term is exist
int checkTerm(int val){

    int i;

    for(i=0; i<20; i++){
        if(numArray[i] == val)
            return 1;
    }
    return 0;
}

//return the level of wanted node
int checkDepth(node *tree, int val, int depthLevel){
    //if not a single node exists
    if(tree == NULL)
        return 0;

    //if found the node
    if(tree->data == val)
        return depthLevel;

    //if not found recursively look the other branches
    return checkDepth(tree->left, val, depthLevel+1)
           | checkDepth(tree->right, val, depthLevel+1);
}

void deleteByTheUserEntered(){
    //get the value which is wanted to be deleted
    printf("write the number to delete: ");
    scanf("%d",&num);

    //if it does exist
    if(checkTerm(num) == 1){
        //assign the last form into root
        root = delete(root, num);
        printf("\ndeleted\n**re-construct tree by the new formation**\nlnr display\n");

        print_lnr(root);
    }
        //if does not exist
    else
        printf("does not exist\n\n");
}

void tracePathByTheUserEntered(){
    //select the node to trace path
    printf("write the number to trace path: ");
    scanf("%d",&num);

    //if selected value does exist, look through its path and depth information
    if(checkTerm(num) == 1)
        printf("found in the depth level: %d",checkDepth(root,num,1));
    else
        printf("does not exist\n\n");
}

void splitString(char string[]){

    int val;
    char * pch;
    pch = strtok (string," ,.-");
    while (pch != NULL)
    {
        val = atoi(pch);
        root = insert(root, val);
        pch = strtok (NULL, " ,.-");
    }
}
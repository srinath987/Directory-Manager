#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX_LENGTH_FOR_ADDRESS 10000
typedef struct node node;

struct node
{
    char *name;  //name of directory/file
    char *alias; //alias of directory/file if any
    int type;    //1 for directory 2 for file
    node *child; // points to children of this node
    node *next;  // points to next node at same level
};

int Flag1; //to check if there are any files or directories with their Prefix same as input Prefix-String

// this is a recursive function which traverses all the nodes in the current directory and finds all the strings with given prefix
void RecursiveFindadd(node *CurrentDirectory, char *Prefix)
{
    //check for the first child node of current directory
    if (CurrentDirectory->child != NULL)
    {
        if (!strcmp(CurrentDirectory->child->name, Prefix)) //compares name and prefix
        {
            Flag1 = 1; //implies that their is a Directory/File with the given prefix
        }

        if (!strcmp(CurrentDirectory->child->alias, Prefix)) //compares alias and prefix
        {
            Flag1 = 1; //implies that their is a Directory/File with the given prefix
        }

        //recursively call the function to check child nodes and next nodes of the first child node and so on
        RecursiveFindadd(CurrentDirectory->child, Prefix);
    }

    //check for the first next node of current directory
    if (CurrentDirectory->next != NULL)
    {
        if (!strcmp(CurrentDirectory->next->name, Prefix)) //compares name and prefix
        {
            Flag1 = 1; //implies that their is a Directory/File with the given prefix
        }

        if (!strcmp(CurrentDirectory->next->alias, Prefix)) //compares alias and prefix
        {
            Flag1 = 1; //implies that their is a Directory/File with the given prefix
        }

        //recursively call the function to check child nodes and next nodes of the first next node and so on
        RecursiveFindadd(CurrentDirectory->next, Prefix);
    }
}

int findadd(node *T, char *Prefix)
{

    Flag1 = 0; //intializes Flag to zero

    RecursiveFindadd(T, Prefix); //calls RecursiveFind function to find all the nodes with given prefix

    if (!Flag1) //if we do not find any Directory/File with given prefix
    {
        return 1;
    }
    else
        return 0;
}

//Implement addition of file/directory inside current directory.
void add(node *current)
{
    node *NewNode = (node *)malloc(sizeof(node));
    NewNode->name = (char *)malloc(sizeof(char) * 100);
    NewNode->alias = (char *)malloc(sizeof(char) * 100);
    char name[100];
    int type;
    printf("Enter the type(1->Directory  2->File) :");
    scanf("%d", &type);

    printf("Enter the name of File/Directory :");
    scanf("%s", name);

    int integer = findadd(current, name);
    if (integer == 0)
    {
        printf("Name already exist\n");
        return;
    }

    strcpy(NewNode->name, name);
    NewNode->type = type;
    strcpy(NewNode->alias, "None");
    NewNode->child = NULL;
    NewNode->next = NULL;

    if (current->child == NULL)
    {
        current->child = NewNode;
        return;
    }

    node *T = current->child;

    while (T->next != NULL)
        T = T->next;

    T->next = NewNode;

    return;
}

//Change current directory to any directory by taking input the complete path to that directory.
node *Move(node *T, char *ADDRESS, int *Flag)
{
    //Check whether the T->Word is equal to word given in ADDRESS till first / is encountered

    char *C, *D;
    node *temp;
    int n, m, i = 0;
    n = strlen(T->name);
    m = strlen(ADDRESS);

    C = (char *)malloc(sizeof(char) * m);
    D = (char *)malloc(sizeof(char) * m);

    for (i = 0; i < n; i++)
    {
        if (T->name[i] != ADDRESS[i])
        {
            printf("Error: Wrong Name of root node has been provided\n");
            *Flag = 0;
            return NULL;
        }
    }
    if (i == m)
    {
        return T;
    }
    if (ADDRESS[i] != '/')
    {
        printf("Error: Wrong Address\n");
        *Flag = 0;
        return NULL;
    }
    i++;

    for (int j = i; j < m; j++)
    {
        C[j - i] = ADDRESS[j];
    }
    int k = i;

    i = 0;
    while (C[i] != '/')
    {
        D[i] = C[i];
        if (i == (m - k - 1))
        {
            break;
        }
        i++;
    }

    //// Check whether D is the name of any of the T's childeren

    temp = T->child;

    while (temp)
    {
        if (strcmp(temp->name, D) == 0)
        {
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Error: There is no node with name %s in directory with name %s\n", D, T->name);
        *Flag = 0;
        return NULL;
    }
    else
    {
        temp = Move(temp, C, Flag);
    }

    return temp;
}

node *MOVE(node *ROOT, char *ADDRESS)
{
    int flag;
    flag = 1;
    node *Temp;
    Temp = (node *)malloc(sizeof(struct node));
    Temp = Move(ROOT, ADDRESS, &flag);
    if (flag == 1)
    {
        return Temp;
    }
    else
    {
        return NULL;
    }
}

//Change the current directory to any directory by taking input the alias of that directory.
node *teleport(node *Root, char *Alias)
{
    if (Root->child != NULL)
    {
        if (strcmp(Root->child->alias, Alias) == 0)
        {
            return Root->child;
        }

        node *Temp = teleport(Root->child, Alias);
        if (Temp)
        {
            return (Temp);
        }
    }

    if (Root->next != NULL)
    {
        if (strcmp(Root->next->alias, Alias) == 0)
        {
            return Root->next;
        }

        node *Temp1 = teleport(Root->next, Alias);
        if (Temp1)
        {
            return (Temp1);
        }

        return NULL;
    }
}

// Implement saving directory with an alias.
void alias(node *ROOT)
{
    char aliasstr[] = "None";
L2:;
    int no_of_word = 0, input = 0;
    char str[100][100], string2[100]; //path and alias strings
    int i = 0, j = 0;                 //loop variables

    printf("Enter the total number of folder/directory in the path: ");
    scanf("%d", &no_of_word); //scan number of folder in path

    printf("Enter path to the directory (seperated by spaces):\n");
    for (i = 0; i < no_of_word; i++)
    {
        scanf("%s[^\n]", str[i]); //scan path seperated by spaces
    }

    node *c = malloc(sizeof(node));
    node *temp2 = malloc(sizeof(node));
    c = ROOT->child;
    i = 0;
L3:
    j = 0;
    i++;
    while (c != NULL) //search every level
    {
        if (strcmp(c->name, str[i]) == 0) //search every level word by word
        {
            goto L1; //if found
        }
        else
        {
            c = c->next;
        }
    }
    printf("Wrong Input. Please Enter 1 to enter a Correct Path or Press 0 to exit.\n"); //wrong path
    scanf("%d", &input);
    if (input == 0)
    {
        return; //exit
    }
    else
    {
        goto L2; //begin from start
    }
L1:;
    if (c->type == 1 && strcmp(str[no_of_word - 1], c->name) == 0) //if satisfies conditions
    {
        if (no_of_word == 1)
        {
            printf("Cannot give alias to root\n");
            return;
        }
        if (strcmp(c->alias, aliasstr) != 0 && c->type == 1) //alias already exist condition
        {
            printf("Alias already exist\n");
            return;
        }
        else //if alias doesnt exist
        {
            printf("Enter the alias you want to impose to the directory: ");
            scanf("%s", string2); //scan alias
            node *Temp;
            Temp = (node *)malloc(sizeof(struct node));
            Temp = teleport(ROOT, string2);
            if (Temp)
            {
                printf("Alias with same name already exists\n");
            }
            else
            {
                strcpy(c->alias, string2);                                           //copy alias
                printf("Alias :%s implemented to directory %s\n", string2, c->name); //successfully implemented
                return;
            }
        }
    }
    else if (c->child != NULL)
    {
        c = c->child;
        goto L3; //goto child then search at that line
    }
    else
    {
        printf("Wrong Path\n");
        return;
    }
}

//Implement searching all the files/directories with given prefix inside current directory.
int Flag; //to check if there are any files or directories with their Prefix same as input Prefix-String

int Compare_Prefix(char *a, char *b) //to check if prefix of name string of some node is equal to the input prefix-string
{
    for (int i = 0; i < strlen(b); i++)
    {
        if (a[i] != b[i]) //a is the name string and b is the prefix string
        {
            return 0;
        }
    }
    return 1;
}

// this is a recursive function which traverses all the nodes in the current directory and finds all the strings with given prefix
void RecursiveFind(node *CurrentDirectory, char *Prefix)
{
    //check for the first child node of current directory
    if (CurrentDirectory->child != NULL)
    {
        if (Compare_Prefix(CurrentDirectory->child->name, Prefix)) //compares name and prefix
        {
            Flag = 1; //implies that their is a Directory/File with the given prefix
            printf("%s\n", CurrentDirectory->child->name);
        }

        if (Compare_Prefix(CurrentDirectory->child->alias, Prefix)) //compares alias and prefix
        {
            Flag = 1; //implies that their is a Directory/File with the given prefix
            printf("%s\n", CurrentDirectory->child->alias);
        }

        //recursively call the function to check child nodes and next nodes of the first child node and so on
        RecursiveFind(CurrentDirectory->child, Prefix);
    }

    //check for the first next node of current directory
    if (CurrentDirectory->next != NULL)
    {
        if (Compare_Prefix(CurrentDirectory->next->name, Prefix)) //compares name and prefix
        {
            Flag = 1; //implies that their is a Directory/File with the given prefix
            printf("%s\n", CurrentDirectory->next->name);
        }

        if (Compare_Prefix(CurrentDirectory->next->alias, Prefix)) //compares alias and prefix
        {
            Flag = 1; //implies that their is a Directory/File with the given prefix
            printf("%s\n", CurrentDirectory->next->alias);
        }

        //recursively call the function to check child nodes and next nodes of the first next node and so on
        RecursiveFind(CurrentDirectory->next, Prefix);
    }
}

void Find(node *T)
{
    int n;
    printf("\nEnter the size of Prefix-String:");
    scanf("%d", &n); //stores the size of the input prefix-string

    char Prefix[n];
    printf("\nEnter the Prefix-String:");
    scanf("%s", Prefix); //stores the input prefix-string
    printf("\n");

    Flag = 0; //intializes Flag to zero

    RecursiveFind(T, Prefix); //calls RecursiveFind function to find all the nodes with given prefix

    if (!Flag) //if we do not find any Directory/File with given prefix
    {
        printf("There are no Files or Directories with the given Prefix-String\n");
    }
}

// Terminates the program.
void quit(node *ROOT)
{
    //frees each node
    if (ROOT != NULL)
    {
        quit(ROOT->child);
        quit(ROOT->next);
        free(ROOT);
    }
}

int main(void)
{
    node *ROOT = (node *)malloc(sizeof(node));
    node *current = (node *)malloc(sizeof(node));
    ROOT->name = (char *)malloc(sizeof(char) * 100);
    ROOT->alias = (char *)malloc(sizeof(char) * 100);
    strcpy(ROOT->name, "Root");
    ROOT->child = NULL;
    ROOT->next = NULL;
    ROOT->alias = "NotPossible";
    ROOT->type = 1;
    current = ROOT;
    int operation;
    while (1)
    {
        printf("Current Directory/File:%s\n", current->name);
        printf("Enter 1 to add a file/directory\n");
        printf("Enter 2 to move a directory\n");
        printf("Enter 3 to add alias to a directory\n");
        printf("Enter 4 to teleport or change path of a directory\n");
        printf("Enter 5 to find/search for a directory\n");
        printf("Enter 6 to quit\n");

        scanf("%d", &operation);
        switch (operation)
        {
        case 1:
        {
            //add
            add(current);
            break;
        }
        case 2:
        {
            //move
            char *ADDRESS;
            ADDRESS = (char *)malloc(sizeof(char) * MAX_LENGTH_FOR_ADDRESS);
            printf("Please Enter the complete Address in backslash format (Tree/SubTree/SubTreeofSubTree and so on):\n");
            scanf("%s", ADDRESS);
            node *Temp;
            Temp = (node *)malloc(sizeof(struct node));
            Temp = MOVE(ROOT, ADDRESS);
            if (Temp)
            {
                current = Temp;
                printf("You have been successfully relocaled to your desired directory/file\n");
            }
            break;
        }
        case 3:
        {
            //alias
            alias(ROOT);
            break;
        }
        case 4:
        {
            ///teleport
            char *AliasName;
            AliasName = (char *)malloc(sizeof(char) * 30);
            printf("Please Enter the Alias of the file to which you want to teleport to:\n");
            scanf("%s", AliasName);

            node *Temp;
            Temp = (node *)malloc(sizeof(struct node));
            Temp = teleport(ROOT, AliasName);
            if (Temp)
            {
                current = Temp;
                printf("You have been successfully telepored to your desired directory/file\n");
            }
            else
            {
                printf("No File with Alias %s exist in the data\n", AliasName);
            }
            break;
        }
        case 5:
        {
            //find
            Find(current);
            break;
        }
        case 6:
        {
            //quit
            quit(ROOT);
            return 0;
        }
        default:
        {
            printf("Wrong Input! Try Again.\n");
        }
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeElement 
{
    char val;
    char* code;
    struct TreeElement* left;
    struct TreeElement* right;
};

struct TreeElement* newNode(char data)
{
    struct TreeElement* node = (struct TreeElement*)malloc(sizeof(struct TreeElement));
    if (node)
    {
        node->val = data;
        node->left = NULL;
        node->right = NULL;

        return node;
    }
}

struct TreeElement* sortedArrayToBST(char arr[], int start, int end)
{
    if (start > end)
    {
        return NULL;
    }

    int mid = (start + end) / 2;
    struct TreeElement* root = newNode(arr[mid]);
    root->left = sortedArrayToBST(arr, start, mid - 1);
    root->right = sortedArrayToBST(arr, mid + 1, end);

    return root;
}

struct huffElement 
{
    char val;
    int count;
    int code;
    struct huffElement* left;
    struct huffElement* right;
};

int compareHuffElement(const struct huffElement** a, const struct huffElement** b)
{
    int pa = (int)(*a)->count;
    int pb = (int)(*b)->count;

    if (pa > pb)
    {
        return 1;
    }
    else if (pa == pb)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int compareChar(const char* a, const char* b)
{
    int pa = *(char*)a;
    int pb = *(char*)b;

    if (pa == 0)
    {
        return 1;
    }
    if (pb == 0)
    {
        return -1;
    }

    if (pa > pb)
    {
        return 1;
    }
    else if (pa == pb)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

struct huffElement** huffmanArray(char* string, int unique)
{
    struct huffElement** huffArray = (struct huffElement**) malloc(sizeof(struct huffElement*) * unique);

    if (huffArray)
    {
        int count = -1;
        char prev = '\0';
        for (int i = 0; string[i] != '\0'; i++)
        {
            if (string[i] != prev)
            {
                count++;

                struct huffElement* huffPointer = (struct huffElement*) malloc(sizeof(struct huffElement));
                huffPointer->val = string[i];
                huffPointer->count = 1;
                huffPointer->code = 0x1;
                huffPointer->left = NULL;
                huffPointer->right = NULL;
                huffArray[count] = huffPointer;

                prev = string[i];
            }
            else
            {
                huffArray[count]->count++;
            }
        }

        return huffArray;
    }
}

void printHuffArray(struct huffElement** huffArray, int unique)
{
    printf("\n");
    for (int i = 0; i < unique; i++)
    {
        printf("%c\t", huffArray[i]->val);
        printf("%d\t", huffArray[i]->count);
        printf("\n");
    }
}

char* uniqueStr(char* sorted, int unique)
{
    char* uniqueString = (char*) malloc(sizeof(char) * (unique + 1));
    if (uniqueString)
    {
        int count = 0;
        char prev = '\0';
        for (int i = 0; sorted[i] != '\0'; i++)
        {
            if (sorted[i] != prev)
            {
                uniqueString[count] = sorted[i];
                prev = sorted[i];
                count++;
            }
        }
        uniqueString[count] = '\0';
    }
    return uniqueString;
}

struct huffElement** createHuffmanTree(struct huffElement** huffArray, int unique)
{
    if (unique > 1)
    {
        struct huffElement* huffPointer = (struct huffElement*)malloc(sizeof(struct huffElement));
        huffPointer->val = '@';
        huffPointer->count = huffArray[0]->count + huffArray[1]->count;
        huffPointer->code = 0x1;
        huffPointer->left = huffArray[0];
        huffPointer->right = huffArray[1];
        huffArray[0] = huffPointer;

        for(int i = 1; i < unique - 1; i++)
        {
            huffArray[i] = huffArray[i + 1];
        }

        unique--;
        printf("Zawartosc przy %d elementach\n", unique);
        printHuffArray(huffArray, unique);

        struct huffElement** smallerArray = (struct huffElement**) realloc(huffArray, (sizeof(struct huffElement*)) * unique);
        *huffArray = *smallerArray;
        if (smallerArray)
        {
            qsort(smallerArray, unique, sizeof(struct huffElement*), compareHuffElement);
        }
        createHuffmanTree(smallerArray, unique);
    }
    else
    {
        return huffArray;
    }
}

void fillCodeInTree(struct huffElement* huffmanTree)
{
    if (huffmanTree == NULL)
    {
        return;
    }
    else
    {
        if (huffmanTree->left != NULL)
        {
            huffmanTree->left->code = 2 * huffmanTree->code + 0;
            fillCodeInTree(huffmanTree->left);
        }
        if (huffmanTree->right != NULL)
        {
            huffmanTree->right->code = 2 * huffmanTree->code + 1;
            fillCodeInTree(huffmanTree->right);
        }
    }
}

int countDistinct(char* arr, int n)
{
    qsort(arr, n, sizeof(char), compareChar);

    int res = 0;
    for (int i = 0; i < n - 1; i++) 
    {
        while (i < n - 1 && arr[i] == arr[i + 1])
        {
            i++;
        }
        res++;
    }

    return res;
}

void printTree(struct TreeElement* head)
{
    if (head == NULL)
    {
        return;
    }
    else
    {
        printf("%c %s\n", head->val, head->code);

        printTree(head->left);
        printTree(head->right);
    }
}

char* stringNumber(int number, int unique)
{
    char* string = (char*)malloc(sizeof(char) * unique);
    int i = 0;
    while (number > 1)
    {
        int r = number % 2;
        if (r == 0)
        {
            string[i] = '0';
        }
        else
        {
            string[i] = '1';
        }
        number /= 2;
        i++;
    }
    string[i] = '\0';
    _strrev(string);
    return string;
}

void assignCodeTree(struct TreeElement* head, char letter, int num, int unique)
{
    if (head == NULL)
    {
        return;
    }
    else if (head->val == letter)
    {
        head->code = stringNumber(num, unique);
    }
    else
    {
        if (letter <= head->val)
        {
            assignCodeTree(head->left, letter, num, unique);
        }
        else
        {
            assignCodeTree(head->right, letter, num, unique);
        }
    }
}

void fillLettersTree(struct huffElement* huffmanTree, struct TreeElement* head, int unique)
{
    if (huffmanTree == NULL)
    {
        return;
    }
    else
    {
        if (huffmanTree->val != '@')
        {
            assignCodeTree(head, huffmanTree->val, huffmanTree->code, unique);
        }
        fillLettersTree(huffmanTree->left, head, unique);
        fillLettersTree(huffmanTree->right, head, unique);
    }
}

char* searchCodeInTree(struct TreeElement* head, char letter)
{
    if (head == NULL)
    {
        return;
    }
    else if (head->val == letter)
    {
        return head->code;
    }
    else
    {
        if (letter <= head->val)
        {
            searchCodeInTree(head->left, letter);
        }
        else
        {
            searchCodeInTree(head->right, letter);
        }
    }
}

void printHuffmanCode(char* string, struct TreeElement* head)
{
    printf("\nNapis to\n");
    printf("%s\n", string);
    printf("Kod to\n");
    int i = 0;
    while (string[i] != '\0')
    {
        printf("%s", searchCodeInTree(head, string[i]));
        i++;
    }
    printf("\n");
}

int main(void)
{
    char* str = (char*) malloc(sizeof(char) * 20);
    strcpy_s(str, 20, "Hello");
    strcat_s(str, 20, "def");

	//char string[] = "aabcdefddeeccffbcedfaaedcaedfb";
    char string[] = "aaaabbbccd";
	int size = sizeof(string) / sizeof(string[0]);
	printf("%d\n", size);

    char* sorted = (char*)malloc(sizeof(char) * size);
    if (sorted)
    {
        strcpy_s(sorted, size, string);
    }
    
	int unique = countDistinct(sorted, size);
	printf("%d\n", unique);
    printf("%s\n", sorted);

    char* uniqueString = uniqueStr(sorted, unique);

    struct huffElement** huffArray = huffmanArray(sorted, unique);
    printHuffArray(huffArray, unique);

    qsort(huffArray, unique, sizeof(struct huffElement*), compareHuffElement);
    printHuffArray(huffArray, unique);

    huffArray = createHuffmanTree(huffArray, unique);
    struct huffElement* huffmanTree = huffArray[0];
    fillCodeInTree(huffmanTree);
    
    struct TreeElement* head = sortedArrayToBST(uniqueString, 0, unique - 1);

    stringNumber(5, unique);
    fillLettersTree(huffmanTree, head, unique);

    printTree(head);
    printHuffmanCode(string, head);

	return 0;
}
// Make sure to run this program from inside the "data" directory that contains
// dictionary.txt

#define MAX_WORDS 2048
#define START_WORD "муха"
#define END_WORD "слон"
#define WORD_LEN 4
#define WORD_BYTES WORD_LEN*2+1 // UTF8 2-byte characters

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

int
StringLengthUTF8(char *String)
{
    int Result = 0;

    while(*String)
    {
        if(*String >= 0 && *String <= 127)
        {
            String += 1;
        }
        else if((*String & 0xE0) == 0xC0)
        {
            String += 2;
        }
        else if((*String & 0xF0) == 0xE0)
        {
            String += 3;
        }
        else if((*String & 0xF8) == 0xF0)
        {
            String += 4;
        }
        ++Result;
    }

    return(Result);
}

void
RemoveNewline(char *String)
{
    while(*String)
    {
        if(*String == '\n')
        {
            *String = '\0';
            break;
        }
        ++String;
    }
}

bool
DifferByOneLetter(char *Word1, char *Word2)
{
    int DifferenceCount = 0;

    while(*Word1 && *Word2)
    {
        // UTF8 2-byte characters
        if((*Word1 != *Word2) || (*(Word1+1) != *(Word2+1)))
        {
            ++DifferenceCount;
        }
        Word1 += 2;
        Word2 += 2;
    }

    bool Result = DifferenceCount == 1;

    return(Result);
}

struct word_node
{
    int WordIndex;
    word_node *Next;
};

int
main()
{
    char Words[MAX_WORDS][WORD_BYTES];
    int WordCount = 0;
    word_node *AdjacencyLists[MAX_WORDS];
    int EdgeTo[MAX_WORDS];
    bool Marked[MAX_WORDS];
    word_node *BFSQueueHead = 0;
    word_node *BFSQueueTail = 0;
    int StartWordIndex = -1;
    int EndWordIndex = -1;

    char Word[128];
    FILE *DictionaryFile = fopen("dictionary.txt", "r");

    if(!DictionaryFile)
    {
        printf("Cannot open dictionary.txt\n");
        return(1);
    }

    while(fgets(Word, ArrayCount(Word), DictionaryFile))
    {
        RemoveNewline(Word);
        if(StringLengthUTF8(Word) == WORD_LEN)
        {
            strcpy(Words[WordCount++], Word);
        }
    }

    fclose(DictionaryFile);

    for(int WordIndex = 0;
        WordIndex < WordCount;
        ++WordIndex)
    {
        for(int WordIndex2 = 0;
            WordIndex2 < WordCount;
            ++WordIndex2)
        {
            if (WordIndex != WordIndex2)
            {
                if(DifferByOneLetter(Words[WordIndex], Words[WordIndex2]))
                {
                    word_node *Node = (word_node *)malloc(sizeof(word_node));
                    Node->WordIndex = WordIndex2;

                    if(AdjacencyLists[WordIndex])
                    {
                        Node->Next = AdjacencyLists[WordIndex];
                        AdjacencyLists[WordIndex] = Node;
                    }
                    else
                    {
                        Node->Next = 0;
                        AdjacencyLists[WordIndex] = Node;
                    }
                }
            }
        }
    }

    for(int WordIndex = 0;
        WordIndex < WordCount;
        ++WordIndex)
    {
        if(strcmp(Words[WordIndex], START_WORD) == 0)
        {
            StartWordIndex = WordIndex;
            break;
        }
    }

    if(StartWordIndex < 0)
    {
        printf("Couldn't find start word.\n");
        return(1);
    }

    for(int WordIndex = 0;
        WordIndex < WordCount;
        ++WordIndex)
    {
        if(strcmp(Words[WordIndex], END_WORD) == 0)
        {
            EndWordIndex = WordIndex;
            break;
        }
    }

    if(EndWordIndex < 0)
    {
        printf("Couldn't find end word.\n");
        return(1);
    }

    Marked[StartWordIndex] = true;
    BFSQueueHead = (word_node *)malloc(sizeof(word_node));
    BFSQueueHead->WordIndex = StartWordIndex;
    BFSQueueHead->Next = 0;

    while(BFSQueueHead)
    {
        int WordIndex = BFSQueueHead->WordIndex;
        word_node *HeadNext = BFSQueueHead->Next;
        free(BFSQueueHead);
        BFSQueueHead = HeadNext;

        for(word_node *AdjacentNode = AdjacencyLists[WordIndex];
            AdjacentNode;
            AdjacentNode = AdjacentNode->Next)
        {
            if(!Marked[AdjacentNode->WordIndex])
            {
                Marked[AdjacentNode->WordIndex] = true;
                EdgeTo[AdjacentNode->WordIndex] = WordIndex;

                word_node *QueueNode = (word_node *)malloc(sizeof(word_node));
                QueueNode->WordIndex = AdjacentNode->WordIndex;
                QueueNode->Next = 0;

                if (!BFSQueueHead)
                {
                    BFSQueueHead = BFSQueueTail = QueueNode;
                }
                else
                {
                    BFSQueueTail->Next = QueueNode;
                    BFSQueueTail = QueueNode;
                }
            }
        }
    }

    if(Marked[EndWordIndex])
    {
        for(int WordIndex = EndWordIndex;
            ;
            WordIndex = EdgeTo[WordIndex])
        {
            printf("%s\n", Words[WordIndex]);
            if(WordIndex == StartWordIndex)
            {
                break;
            }
        }
    }
    else
    {
        printf("Can't convert start word to the end word.\n");
    }

    return(0);
}

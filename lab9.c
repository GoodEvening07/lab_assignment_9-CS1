/*
	Gabriel Hernandez
	COP3502C - CS1 Lab 9
	7-30-2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType record;
	struct HshType* next;
};


// Compute the hash function
int hash(int x)
{
	return x % 15;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\tIndex:%d Name:%c Order%d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

//New function//
void SettingHash(struct HashType* t)
{
	for (int i = 0; i < 15; i++)
	{
		t[i].record.id = 0;
		t[i].record.order = 0;
		t[i].record.name = '-';
		t[i].next = NULL;

	}
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
		int i;

	   for (i = 0; i < hashSz; i++)
    {
        if (pHashArray[i].record.id != 0)
        {
            printf("index %d -> id:%d, name:%c, order:%d", i, pHashArray[i].record.id, pHashArray[i].record.name, pHashArray[i].record.order);
            struct HashType* current = pHashArray[i].next;
            while (current != NULL)
            {
                printf(" -> id:%d, name:%c, order:%d", current->record.id, current->record.name, current->record.order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main()
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Implement the hash table
	struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * 15);
    SettingHash(hashTable);

    // Fill the hash table with the records from pRecords
    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].order);
        if (hashTable[index].record.id == 0)
        {
            // No collision
            hashTable[index].record = pRecords[i];
        }
        else
        {
            // Collision: add the new record to the linked list at the index
            struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
            newNode->record = pRecords[i];
            newNode->next = hashTable[index].next;
            hashTable[index].next = newNode;
        }
    }

    displayRecordsInHash(hashTable, 15);

    // Free memory
    free(hashTable);
    free(pRecords);
}
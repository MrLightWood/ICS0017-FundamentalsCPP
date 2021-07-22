#pragma warning(disable : 4996)
#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

using namespace std;

void PrintDataStructure(HEADER_C* pList)
{
	HEADER_C* pC = pList;
	int counter = 1;

	while (pC)
	{
		void** pCitemArr = pC->ppItems;
		for (int j = 0; j < 26; j++)
		{
			if (*(pCitemArr + j))
			{
				ITEM1* pCitem = (ITEM1*)(*(pCitemArr + j));
				cout << counter << ") " << pCitem->pID << "\t" << pCitem->Code << "\t" << pCitem->pTime << endl;
				counter++;

			}
		}
		pC = pC->pNext;
	}
}

char* GetWordOne(char* pNewItemID)
{
	char* pWordOne = (char*)malloc(1 * sizeof(char));
	char* pSpace = strchr(pNewItemID, ' ');
	if (!pSpace)
	{
		return NULL;
	}

	for (int i = 1; *(pNewItemID + i); i++)
	{
		if ((pNewItemID + i) == pSpace)
		{
			pWordOne = (char*)malloc(i + 1 * sizeof(char));
			if (*pWordOne != NULL)
			{
				*(pWordOne + i) = '\0';
			}
		}
	}

	if (pSpace)
	{
		int i = 0;
		for (i = 0; (pNewItemID + i) != pSpace; *(pWordOne + i) = *(pNewItemID + i), i++);
	}

	return pWordOne;
}

char* GetWordTwo(char* pNewItemID)
{
	char* pWordTwo = (char*)malloc(1 * sizeof(char));
	char* pSpace = strchr(pNewItemID, ' ');
	if (!pSpace || !(*(pSpace + 1)))
	{
		return NULL;
	}

	for (int i = 1; *(pSpace + i); i++)
	{
		if (!(*(pSpace + i + 1)))
		{
			pWordTwo = (char*)malloc((i + 1) * sizeof(char));
			if (*pWordTwo != NULL)
			{
				*(pWordTwo + i) = '\0';
			}
		}
	}

	if ((pSpace + 1))
	{
		for (int i = 0; *(pSpace + i + 1); *(pWordTwo + i) = *(pSpace + i + 1), i++);
	}

	return pWordTwo;
}

HEADER_C* GetHeaderPos(HEADER_C* pList, char* pChar)
{
	HEADER_C* pC = pList;
	while (pC)
	{
		if (pC->cBegin == *pChar)
		{
			return pC;
		}
		pC = pC->pNext;
	}
	return NULL;
}

ITEM1* GetItemLastPos(ITEM1* pItem)
{
	ITEM1* pI = pItem;
	while (pI)
	{
		if (pI->pNext == NULL)
		{
			return pI;
		}
		pI = pI->pNext;
	}

	return pItem;
}

bool StringCompare(char* str1, char* str2)
{
	if (strlen(str1) != strlen(str2))
	{
		return false;
	}
	else
	{
		size_t stlen = strlen(str1);
		for (int i = 0; i < stlen; i++)
		{
			if (*(str1 + i) == *(str2 + i))
			{
				if (i + 1 == stlen)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

ITEM1* GetItemPreviousPos(ITEM1* pItem, ITEM1* pItemInit)
{
	ITEM1* pPrevItem = pItemInit;

	while (pPrevItem)
	{
		if (pPrevItem->pNext == pItem)
		{
			return pPrevItem;
		}
		else if (pPrevItem = pItem)
		{
			return NULL;
		}
		pPrevItem = pPrevItem->pNext;
	}
}

bool CheckForValidity(char* pChar)
{
	char* pSpace = strchr(pChar, ' ');
	if (pSpace != NULL)
	{
		if (*(pChar + 1) != ' ')
		{
			return false;
		}

		if (*(pSpace + 2) != NULL)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

HEADER_C* InsertItem(HEADER_C* pList, char* pNewItemID = 0)
{
	ITEM1* pNewItem = (ITEM1*)malloc(sizeof(ITEM1));
	if (pNewItemID != 0)
	{
		if (CheckForValidity(pNewItemID) == false)
		{
			cout << "ERROR! Given index is wrong" << endl;
			return pList;
		}
	}

	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* pWordOne = GetWordOne(pNewItemID);
	char* pWordTwo = GetWordTwo(pNewItemID);

	HEADER_C* pC = GetHeaderPos(pList, pWordOne);

	if (pC != NULL)
	{
		void** ppItems = pC->ppItems; //Dummy
		for (int i = 0; i < 26; i++)
		{
			if (*(ppItems + i) != NULL && *pWordTwo == abc[i])
			{
				ITEM1* pItems = (ITEM1*)*(ppItems + i); //Dummy
				char* pItemWordOne = GetWordOne(pItems->pID);
				char* pItemWordTwo = GetWordTwo(pItems->pID);

				if (*pItemWordOne == *pWordOne)
				{
					//Iterate through Items
					for (int j = 0; pItems; pItems = pItems->pNext, j++) //Compare input strings with item strings. Check for duplicate
					{
						pItemWordOne = GetWordOne(pItems->pID);
						pItemWordTwo = GetWordTwo(pItems->pID);
						if (StringCompare(pWordOne, pItemWordOne) && StringCompare(pWordTwo, pItemWordTwo))
						{
							cout << "ERROR! This ID already exists";
							return pList;
						}
						//else if ((pWordOne == pItemWordOne && pWordTwo != pItemWordTwo) || (pWordOne != pItemWordOne && pWordTwo != pItemWordTwo))
					}
					// INSERTING NEW ITEM //
					ITEM1* pLastItem = GetItemLastPos(pItems);
					if(pNewItemID == 0)
					{
						pNewItem = (ITEM1*)GetItem(1, NULL);
					} else 
					{
						pNewItem = (ITEM1*)GetItem(1, pNewItemID);
					}
					pNewItem->pNext = NULL;
					pLastItem->pNext = pNewItem;
					return pList;
				}
			}
		}
		for (int i = 0; i < 26; i++) 
		{
			if (abc[i] == *pWordTwo) 
			{
				if (pNewItemID == 0)
				{
					pNewItem = (ITEM1*)GetItem(1, NULL);
				}
				else
				{
					pNewItem = (ITEM1*)GetItem(1, pNewItemID);
				}
				pNewItem->pNext = NULL;
				*(ppItems + i) = (void*)pNewItem;
				return pList;
			}
		}
	}
	else
	{
		pC = pList; //DUMMY. pC has a pointer to the last header
		while (pC->pNext != NULL)
		{
			pC = pC->pNext;
		}

		HEADER_C* newC = (HEADER_C*)malloc(sizeof(HEADER_C));

		void** ppItems = (void**)malloc(26);

		for (int i = 0; i < 26; i++)
		{
			if (abc[i] == *pWordTwo)
			{
				ITEM1* pNewItem = (ITEM1*)GetItem(1, pNewItemID);
				pNewItem->pNext = NULL;
				*(ppItems + i) = (void*)pNewItem;
				continue;
			}
			*(ppItems + i) = NULL;
		}

		newC->cBegin = *pWordOne;
		newC->pNext = NULL;
		newC->ppItems = ppItems;
		pC->pNext = newC;
		return pList;
	}
	return pList;
}

HEADER_C* RemoveItem(HEADER_C* pList, char* pItemID)
{
	if (CheckForValidity(pItemID) == false)
	{
		cout << "ERROR! Given index is wrong" << endl;
		return pList;
	}
	char* pWordOne = GetWordOne(pItemID);
	char* pWordTwo = GetWordTwo(pItemID);
	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	HEADER_C* pC = pList;

	while (pC)
	{
		if (pC->cBegin == *pWordOne)
		{
			for (int i = 0; i < 26; i++)
			{
				if (*(pC->ppItems + i) != NULL && abc[i] == *pWordTwo)
				{
					ITEM1* pItem = (ITEM1*)*(pC->ppItems + i); //DUMMY

					while (pItem != NULL)
					{
						char* pItemWordOne = GetWordOne(pItem->pID);
						char* pItemWordTwo = GetWordTwo(pItem->pID);

						if (StringCompare(pItemWordOne, pWordOne) && StringCompare(pItemWordTwo, pWordTwo))
						{
							ITEM1* pPrevItem = GetItemPreviousPos(pItem, (ITEM1*)*(pC->ppItems + i));
							ITEM1* pNextItem = NULL;
							if (pItem->pNext != NULL)
							{
								ITEM1* pNextItem = pItem->pNext;
							}
							else
							{
								ITEM1* pNextItem = NULL;
							}

							if (pPrevItem == NULL)
							{
								*(pC->ppItems + i) = pNextItem;
								delete(pItem);
								return pList;
							}

							if (pNextItem == NULL)
							{
								pPrevItem->pNext = NULL;
								delete(pItem);
								return pList;
							}
							else
							{
								pPrevItem->pNext = pNextItem;
								delete(pItem);
								return pList;
							}

						}
						pItem = pItem->pNext;
					}
				}
			}
		}
		pC = pC->pNext;
	}
	cout << "ERROR! No index matches with give input";

	return pList;
}

int main()
{
	const char* Cases[] = { "Z A", "Z Z", "Z K", "A Z", "A A", "A K", "G Z", "G A", "G K", "M A", "MBa", "M Bb", "M Z" };
	HEADER_C* ps1 = GetStruct2(1, 30);

	for (int i = 0; i < 13; i++)
	{
		char* lol = (char*)malloc(strlen(Cases[i]) * sizeof(char));
		strcpy(lol, Cases[i]);
		ps1 = InsertItem(ps1, lol);
	}
	PrintDataStructure(ps1);
	for (int i = 0; i < 13; i++)
	{
		char* lol = (char*)malloc(strlen(Cases[i]) * sizeof(char));
		strcpy(lol, Cases[i]);
		ps1 = RemoveItem(ps1, lol);
	}
	PrintDataStructure(ps1);


	return 0;
}

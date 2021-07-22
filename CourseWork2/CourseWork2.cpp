#pragma warning(disable : 4996)
#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <iomanip>
#include <string.h>
#include <string>
#include <fstream>
#include <exception>

using namespace std;

class DataStructure
{
private:
	HEADER_B* dsB;
	char* GetWordOne(char* pNewItemID)
	{
		char* pWordOne = new char;
		char* pSpace = strchr(pNewItemID, ' ');
		if (!pSpace)
		{
			return NULL;
		}

		for (int i = 1; *(pNewItemID + i); i++)
		{
			if ((pNewItemID + i) == pSpace)
			{
				//pWordOne = (char*)malloc(i + 1 * sizeof(char));
				pWordOne = new char[i + 1];
				*(pWordOne + i) = '\0';
				break;
			}
		}

		if (pSpace)
		{
			int i = 0;

			for (i = 0; (pNewItemID + i) != pSpace; i++)
			{
				*(pWordOne + i) = *(pNewItemID + i);
			}
		}

		return pWordOne;
	}

	char* GetWordTwo(char* pNewItemID)
	{
		char* pWordTwo = new char;
		char* pSpace = strchr(pNewItemID, ' ');
		if (!pSpace || !(*(pSpace + 1)))
		{
			return NULL;
		}

		for (int i = 1; *(pSpace + i); i++)
		{
			if (!(*(pSpace + i + 1)))
			{
				//pWordTwo = (char*)malloc((i + 1) * sizeof(char));
				pWordTwo = new char[i + 1];
				*(pWordTwo + i) = '\0';
				break;
			}
		}

		if ((pSpace + 1))
		{
			for (int i = 0; *(pSpace + i + 1); *(pWordTwo + i) = *(pSpace + i + 1), i++);
		}

		return pWordTwo;
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
	HEADER_B* GetHeaderPos(HEADER_B* pList, char* pChar)
	{
		HEADER_B* pB = pList;
		while (pB)
		{
			if (pB->cBegin == *pChar)
			{
				return pB;
			}
			pB = pB->pNext;
		}
		return NULL;
	}
	HEADER_A* GetHeaderLastPos(HEADER_A* pA)
	{
		HEADER_A* ppA = pA;
		while (ppA)
		{
			if (ppA->pNext == NULL)
			{
				return ppA;
			}
			ppA = ppA->pNext;
		}

		return pA;
	}

	HEADER_B* GetHeaderLastPos(HEADER_B* pB)
	{
		if (pB == NULL) { return NULL; }
		HEADER_B* ppB = pB;
		while (ppB)
		{
			if (ppB->pNext == NULL)
			{
				return ppB;
			}
			ppB = ppB->pNext;
		}

		return pB;
	}
	ITEM4* GetItemPreviousPos(ITEM4* pI, ITEM4* pInitI)
	{
		ITEM4* pPrevItem = pInitI;

		while (pPrevItem)
		{
			if (pPrevItem->pNext == pI)
			{
				return pPrevItem;
			}
			else if (pPrevItem = pI)
			{
				return NULL;
			}
			pPrevItem = pPrevItem->pNext;
		}
	}

	HEADER_B* GetHeaderPreviousPos(HEADER_B* pB, HEADER_B* pInitB)
	{
		HEADER_B* pPrevItem = pInitB;

		while (pPrevItem)
		{
			if (pPrevItem->pNext == pB)
			{
				return pPrevItem;
			}
			else if (pPrevItem == pB)
			{
				return NULL;
			}
			pPrevItem = pPrevItem->pNext;
		}
	}

	HEADER_A* GetHeaderPreviousPos(HEADER_A* pA, HEADER_A* pInitA)
	{
		HEADER_A* pPrevItem = pInitA;

		while (pPrevItem)
		{
			if (pPrevItem->pNext == pA)
			{
				return pPrevItem;
			}
			else if (pPrevItem == pA)
			{
				return NULL;
			}
			pPrevItem = pPrevItem->pNext;
		}
	}
public:
	DataStructure()
	{
		HEADER_B dsB;
		//dsB = GetStruct1(4, 0);
	}
	void Open(char* pFilename)
	{
		fstream File;
		File.open(pFilename, fstream::in | fstream::binary | fstream::app);
		string buf;
		ITEM4* newItem = NULL;

		try {
			if (File.good())
			{
				while (getline(File, buf, ';'))
				{
					if(buf == "\n")
					{
						break;
					}
					newItem = new ITEM4;
					char* newID = new char[buf.length() + 1];
					strcpy_s(newID, buf.length() + 1, buf.c_str());
					newItem->pID = newID;
					getline(File, buf, ';');
					newItem->Code = stoul(buf);
					getline(File, buf, ';');
					char* newDate = new char[buf.length() + 1];
					strcpy_s(newDate, buf.length() + 1, buf.c_str());
					newItem->pDate = newDate;
					getline(File, buf, '\n');
					*this += newItem;
				}
			}
			else
			{
				throw exception("Could not open file");
			}
		}catch(const exception &e)
		{
			cout << e.what() << endl;
		}
	}

	~DataStructure()
	{
		DataDelete();
	}
	void GetStructure()
	{
		dsB = GetStruct1(4, 10);
	}
	//DataStructure(char* pFilename);
	//DataStructure(const DataStructure& Original);
	int GetItemsNumber()
	{
		int counter = 0;
		HEADER_B* pB = dsB;
		HEADER_A* pA;
		ITEM4* pItem;

		while (pB)
		{
			pA = pB->pHeaderA;
			while (pA)
			{
				if (pA->pItems)
				{
					pItem = (ITEM4*)pA->pItems;
					while (pItem)
					{
						counter++;
						pItem = pItem->pNext;
					}
				}
				pA = pA->pNext;
			}
			pB = pB->pNext;
		}
		return counter;
	}

	ITEM4* GetItem(char* pID)
	{
		HEADER_B* pB = dsB;
		HEADER_A* pA;
		ITEM4* pItem;
		
		try {
			while (pB)
			{
				pA = pB->pHeaderA;
				while (pA)
				{
					if (pA->pItems)
					{
						pItem = (ITEM4*)pA->pItems;
						while (pItem)
						{
							if (strcmp(pItem->pID, pID) == 0)
							{
								return pItem;
							}
							pItem = pItem->pNext;
						}
					}
					pA = pA->pNext;
				}
				pB = pB->pNext;
			}
			throw exception("No such item");
		} catch(const exception &e)
		{
			cout << e.what() << endl;
			return NULL;
		}
	}

	bool CheckItem(char* pID)
	{
		HEADER_B* pB = dsB;
		HEADER_A* pA;
		ITEM4* pItem;

		while (pB)
		{
			pA = pB->pHeaderA;
			while (pA)
			{
				if (pA->pItems)
				{
					pItem = (ITEM4*)pA->pItems;
					while (pItem)
					{
						if (strcmp(pItem->pID, pID) == 0)
						{
							return true;
						}
						pItem = pItem->pNext;
					}
				}
				pA = pA->pNext;
			}
			pB = pB->pNext;
		}
		return false;
	}

	void operator +=(ITEM4* pNewItem)
	{
		pNewItem->pNext = NULL;
		HEADER_B* pList = dsB;

		char* pNewItemID = pNewItem->pID;
		//cout << pNewItemID;
		/*
		if (!CheckForValidity(pNewItemID))
		{
			cout << "ERROR! Given index is wrong" << endl;
			return;
		}
		*/
		char* pWordOne = GetWordOne(pNewItemID);
		char* pWordTwo = GetWordTwo(pNewItemID);

		HEADER_B* pB = GetHeaderPos(pList, pWordOne);

		if (pB != NULL)
		{
			HEADER_A* pA = pB->pHeaderA;
			while (pA)
			{
				if ((pA) != NULL)
				{
					ITEM4* pItems = (ITEM4*)(pA->pItems);
					char* pItemWordOne = GetWordOne(pItems->pID);
					char* pItemWordTwo = GetWordTwo(pItems->pID);

					if (*pItemWordOne == *pWordOne)
					{
						//Iterate through Items
						while (pItems)
						{
							try {
								if (strcmp(pWordOne, pItemWordOne) == 0 && strcmp(pWordTwo, pItemWordTwo) == 0)
								{
									throw exception("ERROR! This ID already exists");
								}
							} catch(const exception &e)
							{
								cout << e.what() << endl;
								return;
							}
							if (!pItems->pNext)
							{
								pItems->pNext = pNewItem;
								return;
							}
							pItems = pItems->pNext;
						}
					}
				}
				pA = pA->pNext;
			}

			HEADER_A* pLastA = GetHeaderLastPos(pB->pHeaderA);
			HEADER_A* NewA = new HEADER_A;

			try{
				if (NewA && pLastA)
				{
					NewA->cBegin = *pWordTwo;
					NewA->pItems = pNewItem;
					NewA->pNext = NULL;
					pLastA->pNext = NewA;
					return;
				}
				else
				{
					throw exception("Coudln't allocate memory space");
				}
			}catch(const exception &e)
			{
				cout << e.what() << endl;
				return;
			}
		}
		else
		{
			pB = GetHeaderLastPos(pList);

			HEADER_B* newB = new HEADER_B;
			HEADER_A* newA = new HEADER_A;

			if (newB && newA)
			{
				newA->pItems = pNewItem;
				newA->pNext = NULL;
				newA->cBegin = *pWordTwo;

				newB->cBegin = *pWordOne;
				newB->pNext = NULL;
				newB->pHeaderA = newA;
				if(pB != NULL){
					pB->pNext = newB;
				} else 
				{
					dsB = newB;
				}
			}
		}
	}

	void operator-=(char* pID)
	{
		char* pWordOne = GetWordOne(pID);
		char* pWordTwo = GetWordTwo(pID);
		HEADER_B* pB = dsB;

		try {
		while (pB)
		{
			if (pB->cBegin == *pWordOne)
			{
				HEADER_A* pA = pB->pHeaderA;

				while (pA)
				{
					if (pA->cBegin == *pWordTwo)
					{
						if (pA->pItems != NULL)
						{
							ITEM4* pItem = (ITEM4*)pA->pItems;

							while (pItem)
							{
								char* pItemWordOne = GetWordOne(pItem->pID);
								char* pItemWordTwo = GetWordTwo(pItem->pID);

								if (strcmp(pItemWordOne, pWordOne) == 0 && strcmp(pItemWordTwo, pWordTwo) == 0)
								{
									ITEM4* pPrevItem = GetItemPreviousPos(pItem, (ITEM4*)(pA->pItems));
									//ITEM4* pNextItem = NULL;

									if (pItem->pNext != NULL && pPrevItem != NULL)
									{
										ITEM4* pPrevItem = pItem->pNext;
										delete(pItem);
										return;
									}

									if (pPrevItem == NULL)
									{
										if (pItem->pNext == NULL)
										{
											HEADER_A* pPrevA = GetHeaderPreviousPos(pA, pB->pHeaderA);
											delete pItem;

											if (pPrevA == NULL)
											{
												if(pA->pNext == NULL)
												{
													HEADER_B* pPrevB = GetHeaderPreviousPos(pB, dsB);
													delete(pA);

													if (pPrevB == NULL)
													{
														if (pB->pNext)
														{
															dsB = pB->pNext;
															delete pB;
															return;
														}
														delete(pB);
														dsB = new HEADER_B;
													}
													else
													{
														if (pB->pNext != NULL)
														{
															pPrevB->pNext = pB->pNext;
														}
														else
														{
															pPrevB->pNext = NULL;
														}
													}
												}
												else 
												{
													pB->pHeaderA = pA->pNext;
													delete pA;
													return;
												}
											}
											else if (pPrevA != NULL)
											{
												if (pA->pNext != NULL)
												{
													pPrevA->pNext = pA->pNext;
												}
												else
												{
													pPrevA->pNext = NULL;
												}
												delete pA;
											}
											return;
										} else 
										{
											pA->pItems = pItem->pNext;
											delete pItem;
											return;
										}
									} else
									{
										if (pItem->pNext == NULL)
										{
											pPrevItem->pNext = NULL;
										}
										else
										{
											pPrevItem->pNext = pItem->pNext;
										}
										delete(pItem);
										return;
									}
								}
								pItem = pItem->pNext;
							}
						}
						else { break; }
					}
					pA = pA->pNext;
				}
			}
			pB = pB->pNext;
		}
		throw exception("ERROR! No index matches with give input");
		} catch(const exception &e)
		{
			cout << e.what() << endl;
			return;
		}
	}

	int operator==(DataStructure& Other)
	{
		if (GetItemsNumber() == Other.GetItemsNumber())
		{
			HEADER_B* pB = dsB;
			HEADER_A* pA;
			ITEM4* pItem;

			while (pB)
			{
				pA = pB->pHeaderA;
				while (pA)
				{
					if (pA->pItems)
					{
						pItem = (ITEM4*)pA->pItems;
						while (pItem)
						{
							if (!Other.CheckItem(pItem->pID))
							{
								return 0;
							}
							pItem = pItem->pNext;
						}
					}
					pA = pA->pNext;
				}
				pB = pB->pNext;
			}
			return 1;
		}
		else
		{
			return 0;
		}
	}

	void DataCopy(const DataStructure& Original)
	{
		HEADER_B* pB = Original.dsB;
		HEADER_B* newB = NULL;
		HEADER_B* newBprev = NULL;

		while (pB)
		{
			newB = new HEADER_B;

			newB->cBegin = pB->cBegin;
			HEADER_A* pA = pB->pHeaderA;
			HEADER_A* newA = NULL;
			HEADER_A* newAprev = NULL;
			while (pA)
			{
				newA = new HEADER_A;
				newA->cBegin = pA->cBegin;

				ITEM4* pItem = (ITEM4*)pA->pItems;
				ITEM4* newItem = NULL;
				ITEM4* newItemprev = NULL;
				while (pItem)
				{
					newItem = new ITEM4;
					newItem->Code = pItem->Code;
					newItem->pID = new char[strlen(pItem->pID) + 1];
					strcpy_s(newItem->pID, strlen(pItem->pID) + 1, pItem->pID);
					newItem->pDate = new char[strlen(pItem->pDate) + 1];
					strcpy_s(newItem->pDate, strlen(pItem->pDate) + 1, pItem->pDate);
					if (newItemprev != NULL)
					{
						newItemprev->pNext = newItem;
					}
					else
					{
						newA->pItems = newItem;
					}

					newItemprev = newItem;
					if (!pItem->pNext)
					{
						newItem->pNext = NULL;
					}
					pItem = pItem->pNext;
				}
				if (newAprev != NULL)
				{
					newAprev->pNext = newA;
				}
				else
				{
					newB->pHeaderA = newA;
				}

				newAprev = newA;
				if (!pA->pNext)
				{
					newA->pNext = NULL;
				}
				pA = pA->pNext;
			}
			if (newBprev != NULL)
			{
				newBprev->pNext = newB;
			}
			else
			{
				dsB = newB;
			}

			newBprev = newB;
			if (!pB->pNext)
			{
				newB->pNext = NULL;
			}
			pB = pB->pNext;
		}
	}

	void DataDelete()
	{
		HEADER_B* pB = dsB;
		HEADER_B* pBprev = NULL;

		while (pB)
		{
			HEADER_A* pA = pB->pHeaderA;
			HEADER_A* pAprev = NULL;
			while (pA)
			{
				ITEM4* pItem = (ITEM4*)pA->pItems;
				ITEM4* pItemprev = NULL;
				while (pItem)
				{
					if (pItemprev != NULL)
					{
						delete pItemprev;
					}

					pItemprev = pItem;
					if (!pItem->pNext)
					{
						delete pItem;
						break;
					}
					else
					{
						pItem = pItem->pNext;
					}
				}
				if (pAprev != NULL)
				{
					delete pAprev;
				}

				pAprev = pA;
				if (!pA->pNext)
				{
					delete pA;
					break;
				}
				else
				{
					pA = pA->pNext;
				}
			}
			if (pBprev != NULL)
			{
				delete pBprev;
			}

			pBprev = pB;
			if (!pB->pNext)
			{
				delete pB;
				break;
			}
			else
			{
				pB = pB->pNext;
			}
		}
	}

	DataStructure(const DataStructure& Original)
	{
		DataCopy(Original);
	}

	DataStructure& operator=(const DataStructure& Right)
	{
		DataDelete();
		DataCopy(Right);
		return *this;
	}

	void Write(char *pFilename)
	{
		ofstream File;
		File.open(pFilename, ofstream::out | ofstream::binary | ofstream::trunc);

		if(File.good())
		{
			HEADER_B* pB = dsB;
			while(pB)
			{
				HEADER_A* pA = pB->pHeaderA;

				while(pA)
				{
					ITEM4* pItem = (ITEM4*)pA->pItems;

					while(pItem)
					{
						File << pItem->pID << ";" << pItem->Code << ";" << pItem->pDate << ";\n";
						
						pItem = pItem->pNext;
					}

					pA = pA->pNext;
				}

				pB = pB->pNext;
			}
		} else 
		{
			cout << "Could not open file";
		}
		File.close();
	}

	friend std::ostream &operator<<(std::ostream &ostr, const DataStructure &str)
	{
		HEADER_B* pB = str.dsB;
		const char separator = ' ';
		const int nameWidth = 30;
		const int numWidth = 20;

		int counter = 1;

		cout << "[ID] " << left << setw(nameWidth) << setfill(separator) << "[NAME]" << setw(numWidth) << setfill(separator) << "[CODE]" << setw(numWidth) << setfill(separator) << "[DATE]" << endl;

		while (pB)
		{
			HEADER_A* pA = pB->pHeaderA;

			while (pA)
			{
				ITEM4* res = (ITEM4*)pA->pItems;
				while (res)
				{
					if (counter < 10) { cout << "0"; }
					 cout << counter << ") " << left << setw(nameWidth) << setfill(separator) << res->pID << setw(numWidth) << setfill(separator) << res->Code << setw(numWidth) << setfill(separator) << res->pDate << endl;
					counter++;

					res = res->pNext;
				}
				pA = pA->pNext;
			}

			pB = pB->pNext;
		}
		return cout;
	}

	void PrintDataStructure()
	{
		HEADER_B* pB = dsB;
		const char separator = ' ';
		const int nameWidth = 30;
		const int numWidth = 20;

		int counter = 1;

		cout << "[ID] " << left << setw(nameWidth) << setfill(separator) << "[NAME]" << setw(numWidth) << setfill(separator) << "[CODE]" << setw(numWidth) << setfill(separator) << "[DATE]" << endl;

		while(pB)
		{
			HEADER_A* pA = pB->pHeaderA;

			while(pA)
			{
				ITEM4* res = (ITEM4*)pA->pItems;
				while(res)
				{
					if (counter < 10) { cout << "0"; }
					cout << counter << ") " << left << setw(nameWidth) << setfill(separator) << res->pID << setw(numWidth) << setfill(separator) << res->Code << setw(numWidth) << setfill(separator) << res->pDate << endl;
					counter++;

					res = res->pNext;
				}
				pA = pA->pNext;
			}

			pB = pB->pNext;
		}
	}
	
};

int main()
{
	try {
		int itemcount = 0;
		cout << "How many items should I add: ";
		cin >> itemcount;
		if(cin.fail())
		{
			throw exception("Wrong input");
		}

		DataStructure ds;
		for (int i = 0; i < itemcount; i++)
		{
			ds += (ITEM4*)GetItem(4);
		}
		cout << ds << endl << ds.GetItemsNumber() << endl;
		ITEM4* someItem = ds.GetItem((char*)"Light Cyan");
		cout << someItem->pID << endl << someItem->Code << endl << someItem->pDate << endl;
		
		someItem = ds.GetItem((char*)"X X");
		cout << ds;
		//cout << someItem->pID << endl << someItem->Code << endl << someItem->pDate << endl;
		DataStructure ds2 = ds;
		ds -= (char*)"Banana Mania";
		ds -= (char*)"Persian Green";
		ds -= (char*)"Vegas Gold";
		cout << "DataStrcutres are: " << (ds == ds2) << endl;
		ds.Write((char*)"result.txt");
		DataStructure ds3;
		ds3.Open((char*)"result.txt");
		cout << "DataStrcutres are: " << (ds2 == ds3) << endl;
		ds2 = ds3;
		cout << ds2 << endl;

	} catch(const exception &e)
	{
		cout << e.what() << endl;
		return 0;
	}

	return 0;
}
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "Str.h"

typedef struct hashnode
{
	lfStr key;
	void* value;
	hashnode* next;
}hashnode;

class hashtable
{
public:
	hashtable( int newtablesize = 256 );
	~hashtable();

	void* Get(lfStr key);
	void Put(lfStr key, void* value);

private:
	hashnode* HashStr(lfStr key);

	void Resize(int size);
	void Free();
	hashnode* GetFreePos();
	hashnode* NewKey(lfStr key);
protected:

private:
	hashnode* node;
	int tablesize;
};


#endif



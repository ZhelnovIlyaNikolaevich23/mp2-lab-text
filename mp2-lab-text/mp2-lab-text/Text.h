#include <fstream>
#include <string>
#include "Stack.h"
using namespace std;
class TText;
class TTextLink;

struct TTextMem {
	TTextLink* pFirst;
	TTextLink* pLast;
	TTextLink* pFree;
	friend class TTextLink;
};

class TTextLink {
protected:
	TTextLink* pNext, *pDown;
	static TTextMem MemHeader;
public:
	char str[151];
	TTextLink(const char _str[]) {
		strcpy_s(str, _str);
		pNext = NULL;
		pDown = NULL;
	}
	~TTextLink() {}

	static void InitMemSystem(int size = 100);
	static void PrintFreeLink();
	void* operator new(size_t size);
	void operator delete (void* pM);
	static void MemCleaner(TText&);

	int IsAtom() { return pDown == NULL; }
	TTextLink* GetNext() { return pNext; }
	TTextLink* GetDown() { return pDown; }
	friend ostream& operator<<(ostream& os, const TTextLink& TextLink) {
		return os << TextLink.str;
	}
	friend class TText;
};

class TText {
	TTextLink *pFirst, *pCur;
	TStack<TTextLink*> stack;
public:
	TText();
	~TText();
	void GoFirstLink();
	void GoNextLink();
	void GoDownLink();
	void GoPrevLink();
	void SetLine(string str);
	std::string GetLine();
	void InsNextLine(string str);
	void InsNextSection(string str);
	void InsDownLine(string str);
	void InsDownSection(string str);
	void DelNext();
	void DelDown();
	void Read(string fn);
	TTextLink* ReadRec(ifstream& ifs);
	void Write(string fn);
	void WriteRec(ofstream& ofs, TTextLink* p);
	friend ostream& operator<<(ostream& os, TText txt) {
		txt.PrintRec(os, txt.pFirst, "");
		return os;
	}
	void PrintRec(ostream& os, TTextLink* p, string spacing);
	int Reset();
	int IsEnd() const;
	int GoNext();
};


template <class TKey, class TValue>
struct TRecord {
	TKey key;
	TValue value;
};

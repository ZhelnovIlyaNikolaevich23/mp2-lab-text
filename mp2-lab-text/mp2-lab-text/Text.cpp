#define _CRT_SECURE_NO_WARNINGS
#include "Text.h"
#include <iostream>

void TTextLink::InitMemSystem(int size) {
	MemHeader.pFirst = (TTextLink*) new char[sizeof(TTextLink) * size];
	MemHeader.pFree = MemHeader.pFirst;
	MemHeader.pLast = MemHeader.pFirst + (size - 1);
	TTextLink* pLink = MemHeader.pFirst;
	for (int i = 0; i < size - 1; i++) {
		pLink->str[0] = '\0';
		pLink->pNext = pLink + 1;
		pLink++;
	}
	MemHeader.pLast->pNext = NULL;
}
void TTextLink::PrintFreeLink() {
	cout << "List of free links:" << endl;
	for (TTextLink* pLink = MemHeader.pFree; pLink->pNext != NULL; pLink = pLink->pNext) {
		if (pLink->str[0] != '\0') {
			cout << pLink->str << endl;
		}
	}
}
void* TTextLink::operator new(size_t size) {
	TTextLink* pLink = MemHeader.pFree;
	if (MemHeader.pFree != NULL)
		MemHeader.pFree = pLink->pNext;
	return pLink;
}
void TTextLink::operator delete(void *pM) {
	TTextLink* pLink = (TTextLink*)pM;
	pLink->pNext = MemHeader.pFree;
	MemHeader.pFree = pLink;
}
void TTextLink::MemCleaner(TText& txt) {
	for (txt.Reset(); !txt.IsEnd(); txt.GoNext()) {
		string tmp = "&&&";
		tmp += txt.GetLine();
		txt.SetLine(tmp);
	}
	TTextLink* pLink;
	for (pLink = MemHeader.pFree; pLink != NULL; pLink = pLink->pNext) {
		string tmp = "&&&";
		tmp += pLink->str;
		strcpy(pLink->str, tmp.c_str());
	}
	for (pLink = MemHeader.pFirst; pLink <= MemHeader.pLast; pLink++) {
		if (strstr(pLink->str, "&&&") != NULL)
			strcpy(pLink->str, pLink->str + 3);
		else
			delete pLink;
	}
}

TText::TText() {
	pFirst = 0;
	pCur = 0;
	stack.Clear();
}

TText::~TText() {
}

void TText::GoFirstLink() {
	pCur = pFirst;
	stack.Clear();
}

void TText::GoNextLink() {
	if (!pCur) throw - 1;
	if (pCur->pNext) {
		stack.Push(pCur);
		pCur = pCur->pNext;
	}
}

void TText::GoDownLink() {
	if (!pCur) throw - 1;
	if (pCur->pDown) {
		stack.Push(pCur);
		pCur = pCur->pDown;
	}
}

void TText::GoPrevLink() {
	if (!pCur) throw - 1;
	if (!stack.IsEmpty())
		pCur = stack.Pop();
}

void TText::SetLine(std::string str) {
	if (!pCur) throw - 1;
	strcpy_s(pCur->str, str.c_str());
}

std::string TText::GetLine() {
	if (!pCur) throw - 1;
	return pCur->str;
}

void TText::InsNextLine(std::string str) {
	if (!pCur) throw - 1;
	TTextLink* tmp = new TTextLink(str.c_str());
	tmp->pNext = pCur->pNext;
	pCur->pNext = tmp;
}

void TText::InsNextSection(std::string str) {
	if (!pCur) throw - 1;
	TTextLink* tmp = new TTextLink(str.c_str());
	tmp->pDown = pCur->pNext;
	pCur->pNext = tmp;
}

void TText::InsDownLine(std::string str) {
	if (!pCur) throw - 1;
	TTextLink* tmp = new TTextLink(str.c_str());
	tmp->pNext = pCur->pDown;
	pCur->pDown = tmp;
}

void TText::InsDownSection(std::string str) {
	if (!pCur) throw - 1;
	TTextLink* tmp = new TTextLink(str.c_str());
	tmp->pDown = pCur->pDown;
	pCur->pDown = tmp;
}

void TText::DelNext() {
	if (!pCur) throw - 1;
	if (!pCur->pNext) throw - 1;
	TTextLink* tmp = pCur->pNext;
	pCur->pNext = tmp->pNext;
	delete tmp;
}

void TText::DelDown() {
	if (!pCur) throw - 1;
	if (!pCur->pDown) throw - 1;
	TTextLink* tmp = pCur->pDown;
	pCur->pDown = tmp->pNext;
	delete tmp;
}

void TText::Read(std::string fn) {
	std::ifstream ifs(fn);
	stack.Clear();
	pFirst = ReadRec(ifs);
	pCur = pFirst;
}

TTextLink* TText::ReadRec(std::ifstream& ifs) {
	TTextLink *pHead, *p;
	char _str[81];
	pHead = NULL;
	p = NULL;
	while (!ifs.eof()) {
		ifs.getline(_str, 80, '\n');
		if (_str[0] == '}') {
			stack.Pop();
			break;
		}
		else
			if (_str[0] == '{') {
				stack.Push(p);
				p->pDown = ReadRec(ifs);
			}
			else {
				TTextLink* tmp = new TTextLink(_str);
				if (!pHead) pHead = p = tmp;
				else {
					p->pNext = tmp;
					p = p->pNext;
				}
			}
	}
	return pHead;
}

void TText::Write(std::string fn) {
	std::ofstream ofs(fn);
	WriteRec(ofs, pFirst);
}

void TText::WriteRec(std::ofstream& ofs, TTextLink* p) {
	ofs << p->str << std::endl;
	if (p->pDown) {
		ofs << '{' << std::endl;
		WriteRec(ofs, p->pDown);
		ofs << '}' << std::endl;
	}
	if (p->pNext) {
		WriteRec(ofs, p->pNext);
	}
}

void TText::PrintRec(std::ostream& os, TTextLink* p, string spacing) {
	os << spacing << p->str << std::endl;
	if (p->pDown) {
		PrintRec(os, p->pDown, spacing + "	");
	}
	if (p->pNext) {
		PrintRec(os, p->pNext, spacing);
	}
}
int TText::Reset() {
	stack.Clear();
	pCur = pFirst;
	if (pCur != NULL) {
		stack.Push(pCur);
		if (pCur->pNext != NULL) stack.Push(pCur->pNext);
		if (pCur->pDown != NULL) stack.Push(pCur->pDown);
	}
	return IsEnd();
}

int TText::IsEnd() const {
	return stack.IsEmpty();
}

int TText::GoNext() {
	if (!IsEnd()) {
		pCur = stack.Pop();
		if (pCur != pFirst) {
			if (pCur->pNext != NULL) stack.Push(pCur->pNext);
			if (pCur->pDown != NULL) stack.Push(pCur->pDown);
		}
		return true;
	}
	return false;
}
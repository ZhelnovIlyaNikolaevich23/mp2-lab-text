#include "Text.h"
#include <iostream>

using namespace std;

TTextMem TTextLink::MemHeader;

int main() {
	TTextLink::InitMemSystem(50);
	TText t;
	int c;
	string str;
	char n;
	t.Read("text.txt");
	TTextLink::MemCleaner(t);
	t.GoFirstLink();
	cout << "1 - print, < > ^ - prev, next, down, 2 - add next line, 3 - add down line" << endl;
	cout << "4 - del next line, 5 - del down line, 6 - get line, 7- rubbish, 8 - exit " << endl;
	do {
		cin >> n;
		switch (n) {
		case '1':
			cout << t;
			break;

		case '>':
			t.GoNextLink();
			break;

		case '^':
			t.GoDownLink();
			break;

		case '<':
			t.GoPrevLink();
			break;

		case '6':
			cout << t.GetLine() << endl;
			break;

		case '2':
			cout << "Text: " << endl;
			cin.get();
			getline(cin, str);
			t.InsNextLine(str);
			break;

		case '3':
			cout << "Text: " << endl;
			cin.get();
			getline(cin, str);
			t.InsDownLine(str);
			break;

		case '4':
			t.DelNext();
			break;


		case '5':
			t.DelDown();
			break;

		case '7':
			c = 0;
			TTextLink::MemCleaner(t);
			cout << "Deleted: " << c << endl;
			TTextLink::PrintFreeLink();
			break;
		case '8':
			break;
		}
	} while (n != '8');
	return 0;
}
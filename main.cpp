#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <random>
#include <string>
#include <algorithm>
using namespace std;

#define CHK_DOT(x, sz) ((x) >= 0 && (x) < (sz))
#define POINT_ITEM(a, r, c) (*((*(a + r)) + c))

enum PLAYER {HUMAN='X', AI='O', EMPTY='_'};

typedef struct {
   int szY;
   int szX;
   PLAYER** map_game;
   int towin;
} Field;

char getval(PLAYER**, const int, const int);

void setval(PLAYER**, const int, const int, PLAYER);

int isvalid(Field &, int, int);

int isempty(Field &, int, int);

void init(Field &field) {
   field.towin = 3;
   field.szY = 3;
   field.szX = 3;
   field.map_game = new PLAYER*[field.szY];
   if(field.map_game == nullptr){//check memory
    cout << "Error memory";
   }
   for (int y = 0; y < field.szY; ++y) {
       *(field.map_game + y) = new PLAYER[field.szX];
       for (int x = 0; x < field.szX; ++x) {
           setval(field.map_game, y, x, EMPTY);
       }
   }
}

void print(Field &field) {
   system("cls");
   cout << "-------" << "\n";
   for (int i = 0; i < field.szY; ++i) {
       cout << "|";
       for (int j = 0; j < field.szX; ++j) {
            cout << getval(field.map_game, i, j) << "|";
       }
       cout << endl;
   }
}

bool InputFormat(string strUser, string strRef)
{
	if (strRef.length() != strUser.length())
		return false;

	// ѕеребираем каждый символ пользовательского ввода
	for (unsigned int i = 0; i < strRef.length(); i++)
	{
		switch (strRef[i])
		{
		case 'x': // = цифра
			if (!isdigit(strUser[i]))
				return false;
			break;
		case '_': // = пробел
			if (!isspace(strUser[i]))
				return false;
			break;

		default: // = точное совпадение с символом
			if (strUser[i] != strRef[i])
				return false;
		}
	}
	return true;
}

void human(Field &field) {
  string strUser;
  string strRef = "x x";
   int x, y;

   do {
// check input format "x_x":
       while(1){
            cout << "¬ведите координаты хода X и Y (от 1 до " << field.szY << ") через пробел >> ";
           getline(cin, strUser);
           if (InputFormat(strUser, strRef))
        break;
       }

    x = strUser[0] - '0';
	y = strUser[2] - '0';

    x--; y--;
   } while (!isvalid(field, x, y) || !isempty(field, x, y));
   setval(field.map_game, y, x, HUMAN);
}

void ai(Field &field) {
   int x, y;

   random_device rd;
   mt19937 mt(rd());
   uniform_real_distribution<double> dist(0, 3);
   do {
       x = dist(mt);
       y = dist(mt);
   } while (!isempty(field, x, y));
   setval(field.map_game, y, x, AI);
}

int linechk(Field &field, int y, int x, int vx, int vy, int len, PLAYER c) {
   const int endx = x + (len - 1) * vx;
   const int endy = y + (len - 1) * vy;
   if (!isvalid(field, endx, endy))
       return 0;
   for (int i = 0; i < len; i++)
       if (getval(field.map_game, (y + i * vy), (x + i * vx)) != c)
           return 0;
   return 1;
}

int winchk(Field &field, PLAYER c) {
   for (int y = 0; y < field.szX; y++) {
       for (int x = 0; x < field.szY; x++) {
           if (linechk(field, y, x, 1, 0, field.towin, c)) return 1;
           if (linechk(field, y, x, 1, 1, field.towin, c)) return 1;
           if (linechk(field, y, x, 0, 1, field.towin, c)) return 1;
           if (linechk(field, y, x, 1, -1, field.towin, c)) return 1;
       }
   }
   return 0;
}

int aiwinchk(Field &field) {
   for (int y = 0; y < field.szY; y++) {
       for (int x = 0; x < field.szX; x++) {
           if (isempty(field, x, y)) {
               setval(field.map_game, y, x, AI);
               if (winchk(field, AI))
                   return 1;
               setval(field.map_game, y, x, EMPTY);
           }
       }
   }
   return 0;
}

int humwinchk(Field &field) {
   for (int y = 0; y < field.szY; y++) {
       for (int x = 0; x < field.szX; x++) {
           if (isempty(field, x, y)) {
               setval(field.map_game, y, x, HUMAN);
               if (winchk(field, HUMAN)) {
                   setval(field.map_game, y, x, AI);
                   return 1;
               }
               setval(field.map_game, y, x, EMPTY);
           }
       }
   }
   return 0;
}

int isdraw(Field &field) {
   for (int y = 0; y < field.szY; y++)
       for (int x = 0; x < field.szX; x++)
           if (isempty(field, x, y))
               return 0;
   return 1;
}

int gamechk(Field &field, PLAYER dot, const string &winString) {
   if (winchk(field, dot)) {
       cout << winString << endl;
       return 1;
   }
   if (isdraw(field)) {
       cout << "draw" << endl;
       return 1;
   }
   return 0;
}

int main(){
   Field field;
   setlocale(LC_CTYPE, "rus");

   while (1) {
       init(field);
       print(field);
       while (1) {
           human(field);
           print(field);
           if (gamechk(field, HUMAN, "Human win!")) break;
           ai(field);
           print(field);
           if (gamechk(field, AI, "AI win!")) break;
       }

       string answer;
       cout << "Play again? ";

       cin >> answer;

       transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

       if (answer.find('y') != 0) // consider y, yo, ya, yes, yeah, yep, yay, etc
           break;

        cin.ignore(2000, '\n'); //clean cin-buffer;
   }

    system ("Pause");
    return 0;
}

char getval(PLAYER** array, const int row, const int col) {
   return POINT_ITEM(array, row, col);
}
void setval(PLAYER** array, const int row, const int col, PLAYER value) {
   POINT_ITEM(array, row, col) = value;
}
int isvalid(Field &field, int x, int y) {
   return CHK_DOT(x, field.szX) && CHK_DOT(y, field.szY);
}
int isempty(Field &field, int x, int y) {
   return getval(field.map_game, y, x) == EMPTY;
}

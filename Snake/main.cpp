#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;
void MoveXY(int x, int y){
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

#define mWidth 80
#define mHeight 25

struct TMap{
	char map1[mHeight][mWidth + 1];
	void Clear();
	void Show();
	int ex;
	int ey;
};

enum TDirection {
	dirLeft,
	dirRight,
	dirUp,
	dirDown
};

enum TResult{
	reOk,
	reKill
};

#define sLen 1000

class TSnake{
	int x, y;
	TMap *mp;
	TDirection dir;
	POINT tail[sLen];
	int len;
public:
	void AddTail(int _x, int _y);
	void MoveTail(int _x, int _y);
	TSnake(TMap *_mp);
	void Init(int _x, int _y, TDirection _dir);
	void PutOnMap();
	TResult UserControl(char w, char s, char a, char d);
};

void TSnake::MoveTail(int _x, int _y){
	for(int i = len - 1; i >= 0; i--)
		tail[i + 1] = tail[i];
	tail[0].x = _x;
	tail[0].y = _y;
}

void TSnake::AddTail(int _x, int _y){
	MoveTail(_x, _y);
	len++;
	if(len >= sLen) len = sLen - 1;
}

TResult TSnake::UserControl(char w, char s, char a, char d){
	POINT old;
	old.x = x;
	old.y = y;
	if(GetKeyState(w) < 0) dir = dirUp;
	if(GetKeyState(s) < 0) dir = dirDown;
	if(GetKeyState(a) < 0) dir = dirLeft;
	if(GetKeyState(d) < 0) dir = dirRight;
	
	if(dir == dirLeft) x--;
	if(dir == dirRight) x++;
	if(dir == dirUp) y--;
	if(dir == dirDown) y++;
	
	if((x < 0) || (x >= mWidth) || (y < 0) || y >= mHeight)
		return reKill;
	
	if (mp->map1[y][x] == '*')
	{
		AddTail(old.x, old.y);
		mp->ex = rand() % 78 +1;;
		mp->ey = rand() % 23 +1;
	}

	else
		MoveTail(old.x, old.y);
	
	return reOk;
}

void TSnake::Init(int _x, int _y, TDirection _dir){
	x = _x;
	y = _y;
	dir = _dir;
	len = 0;
}

void TSnake::PutOnMap(){
	mp->map1[y][x] = '#';
	for(int i = 0; i < len; i++){
		mp->map1[tail[i].y][tail[i].x] = '#';
	}
}

TSnake::TSnake(TMap *_mp){
	mp = _mp;
	Init(0, 0, dirRight);
}

void TMap::Show(){
	MoveXY(0,0);
	map1[mHeight - 1][mWidth - 1] = '\0';
	for(int j = 0; j < mHeight; j++){
		cout << map1[j];
	}
}

void TMap::Clear(){
	for(int i = 0; i < mWidth; i++){
		map1[0][i] = ' ';
	}
	map1[0][mWidth] = '\0';
	for(int j = 1; j < mHeight; j++){
		strncpy(map1[j], map1[0], mWidth + 1);
	}
	map1[ey][ex] = '*';
}

int main() {
	srand(time(0));
	TMap map1;
	map1.ex = rand() % 78 +1;
	map1.ey = rand() % 23 +1;
	TSnake snake1(&map1);
	snake1.Init(10, 5, dirRight);
	do{
		if(snake1.UserControl('W', 'S', 'A', 'D') == reKill)
			snake1.Init(10, 5, dirRight);
		map1.Clear();
		snake1.PutOnMap();
		map1.Show();
		Sleep(80);
	}
	while(GetKeyState(VK_ESCAPE) >= 0);
	return 0;
}








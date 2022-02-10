#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include<ctime>
#include <windows.h>

using namespace std;

pair <int, int> WorldSize{ 25, 25 };

class Character {
protected:
	int heals;
	int damage;
	int LVL;
	char name;
	pair<int, int> position, oldPosition;
public:

	int GetHeals() {return heals;}

	int GetDamage() {return damage;}

	char GetName() {return name;}

	pair<int, int> GetPosition() {return position;}

	pair<int, int> GetOldPosition() { return oldPosition; }

	void SetHeals(int _hp) {
		heals += _hp;
		return;
	}

	void SetDamage(int _dm) {
		damage += _dm;
		return;
	}

	virtual void SetPosition(char _button) {};

};

class Hero: public Character {
private:

public:
	Hero() {
		heals = 100, damage = 10, LVL = 1, name = 'A', position = { 0, 0 };
	};

	void SetPosition(char _button) override {
		if (_button == 's') {
			if (position.first + 1 < WorldSize.first) {
				oldPosition = position;
				position.first++;
			}
		}
		else if (_button == 'w') {
			if (position.first - 1 >= 0) {
				oldPosition = position;
				position.first--;
			}
		}
		else if (_button == 'd') {
			if (position.second + 1 < WorldSize.second) {
				oldPosition = position;
				position.second++;
			}
		}
		else if (_button == 'a') {
			if (position.second - 1 >= 0) {
				oldPosition = position;
				position.second--;
			}
		}
	}
};

class Zombi : public Character {
public:
	Zombi(pair<int, int> _position) {
		heals = 20, damage = 5, LVL = 1, name = 'Z', oldPosition = position = _position;
	}

	void SetPosition(char _button) override {
		if (_button == 'w' || _button == 's' || _button == 'a' || _button == 'd') {
			int rand_y = rand() % 3 - 1;
			int rand_x = rand() % 3 - 1;
			if ((position.first + rand_y < WorldSize.second) && (position.first + rand_y >= 0) &&
				(position.second + rand_x < WorldSize.first) && (position.second + rand_x >= 0)) {
				
				oldPosition = position;
				position = { position.first + rand_y, position.second + rand_x };
			}
		}
		
	}
};

class Room {
protected:
	int enemyCount;
	int enemyMaxLVL;
	int itemsCount;
	char wall;
	pair<int, int>roomPointFirst;
	pair<int, int>roomPointSecond;
public:
	pair<int, int>GetRoomPointFirst(){ return roomPointFirst; }
	pair<int, int>GetRoomPointSecond(){ return roomPointSecond; }
	char GetWall() { return wall; }

	vector<Character*> SpawnEnemys() {
		vector<Character*>Enemys(enemyCount);
		for (int i = 0; i < enemyCount; i++) {
			pair<int, int>positionInRoom {
				(roomPointFirst.first + 1) + rand() % (roomPointSecond.first - (roomPointFirst.first + 2)),
				(roomPointFirst.second + 1) + rand() % (roomPointSecond.second - (roomPointFirst.second + 2))
			};
			Enemys[i] = new Zombi(positionInRoom);

		}
		return Enemys;
	}
};

class EzRoom : public Room {
public:
	EzRoom() {
		enemyCount = 3, enemyMaxLVL = 1, itemsCount = 0, wall = '#', roomPointFirst = { 8, 10 }, roomPointSecond = { 16, 16 };
	}
};

//класс карты
class Map {
private:
	int width;
	int height;
	char floor;
	vector<vector<char>> location;
public:
	Map() : location(vector<vector<char>>(width, vector<char>(height, '_'))), width(WorldSize.second), height(WorldSize.first), floor('_') {}

	void WriteMap(){
		system("cls");
		for (vector<char> i : location) {
			for (char j : i) {
				cout << j << " ";
			}
			cout << endl;
		}
	}

	void SetPosition(vector<Character*> Objects) {

		for (Character* Obj : Objects) {
			location[Obj->GetOldPosition().first][Obj->GetOldPosition().second] = floor;
			location[Obj->GetPosition().first][Obj->GetPosition().second] = Obj->GetName();
		}
	}

	void SetPosition(Room* room) {
		for (int i = room->GetRoomPointFirst().first; i <= room->GetRoomPointSecond().first;  i++) {
			for (int j = room->GetRoomPointFirst().second; j <= room->GetRoomPointSecond().second; j++) {
				if (i == room->GetRoomPointFirst().first ||
					i == room->GetRoomPointSecond().first ||
					j == room->GetRoomPointFirst().second ||
					j == room->GetRoomPointSecond().second) {

					location[i][j] = room->GetWall();
				}
				else { location[i][j] = '*'; }
			}
		}
	}
};

void GamePlay() {
	Map map;
	EzRoom room;
	map.SetPosition(&room);
	vector<Character*>gameObjects = room.SpawnEnemys();
	Character* hero = new Hero;
	gameObjects.push_back(hero);
	char moveBatton;
	while (1) {
		map.SetPosition(gameObjects);
		map.WriteMap();
		moveBatton = _getch();
		for (int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->SetPosition(moveBatton);
		}
	}
}

int main() {
	srand(time(0));
	GamePlay();
}
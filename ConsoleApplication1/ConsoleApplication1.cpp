#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include<ctime>
#include <windows.h>

using namespace std;

pair <int, int> WorldSize{ 32, 60};

struct RoomPosition {
	pair<int, int>firstPoints;
	pair<int, int>secondPoints;
};

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

	void SetPositionOld() { position = oldPosition; }

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
	RoomPosition position;
public:
	RoomPosition GetRoomPosition(){ return position; }
	char GetWall() { return wall; }

	vector<Character*> SpawnEnemys() {
		vector<Character*>Enemys(enemyCount);
		for (int i = 0; i < enemyCount; i++) {
			pair<int, int>positionInRoom {
				(position.firstPoints.first + 1) + rand() % (position.secondPoints.first - (position.firstPoints.first + 2)),
				(position.firstPoints.second + 1) + rand() % (position.secondPoints.second - (position.firstPoints.second + 2))
			};
			Enemys[i] = new Zombi(positionInRoom);

		}
		return Enemys;
	}
};

class EzRoom : public Room {
public:
	EzRoom(RoomPosition _position) {
		enemyCount = 3, enemyMaxLVL = 1, itemsCount = 0, wall = '#', position = _position;
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
	Map() : location(vector<vector<char>>(height, vector<char>(width, '_'))), width(WorldSize.second), height(WorldSize.first), floor('_') {}

	void WriteMap(){
		/*system("cls");*/
		for (vector<char> i : location) {
			for (char j : i) {cout << j << " ";}
			cout << endl;
		}
	}

	void SetPosition(vector<Character*> Objects) {

		for (Character* Obj : Objects) {
			if (location[Obj->GetPosition().first][Obj->GetPosition().second] != '#')
			{
				location[Obj->GetOldPosition().first][Obj->GetOldPosition().second] = floor;
				location[Obj->GetPosition().first][Obj->GetPosition().second] = Obj->GetName();
			}
			else {
				Obj->SetPositionOld();
			}
		}
	}

	void SetPosition(vector<Room*> rooms) {
		for (Room* room: rooms) {
			for (int i = room->GetRoomPosition().firstPoints.first; i <= room->GetRoomPosition().secondPoints.first; i++) {
				for (int j = room->GetRoomPosition().firstPoints.second; j <= room->GetRoomPosition().secondPoints.second; j++) {
					if (i == room->GetRoomPosition().firstPoints.first ||
						i == room->GetRoomPosition().secondPoints.first ||
						j == room->GetRoomPosition().firstPoints.second ||
						j == room->GetRoomPosition().secondPoints.second) {

						location[i][j] = room->GetWall();
					}
					else { location[i][j] = '*'; }
				}
			}
		}
	}
};


vector<RoomPosition>GenRooms(int count) {

	int part = 8;
	vector<RoomPosition> PartOfRooms(part);
	vector<bool>randPart(part, true);
	int t1 = 0, l1 = 0, t2 = static_cast<int>(floor(WorldSize.first / 2) - 1), l2 = static_cast<int>(floor(WorldSize.second / (part/2) - 1));
	for (int i = 1; i <= part; i++) {
		RoomPosition partPoints;
		partPoints.firstPoints.first = t1; partPoints.firstPoints.second = l1;
		partPoints.secondPoints.first = t2; partPoints.secondPoints.second = l2;
		PartOfRooms[i-1] = partPoints;

		cout << partPoints.firstPoints.first << " " << partPoints.firstPoints.second << " : " << partPoints.secondPoints.first << " " << partPoints.secondPoints.second << endl;

		if (i == (part / 2)) {
			t1 += static_cast<int>(floor(WorldSize.first / 2));
			t2 += static_cast<int>(floor(WorldSize.first / 2));
			l1 = 0;
			l2 = static_cast<int>(floor(WorldSize.second / (part / 2) - 1));
			continue;
		}
		l1 += static_cast<int>(floor(WorldSize.second / (part / 2) - 1));
		l2 += static_cast<int>(floor(WorldSize.second / (part / 2) - 1));
	}

	vector<RoomPosition>RoomsPos;
	RoomPosition RoomPos;
	int partMap;
	for (int i = 0; i < count; i++) {

		do{
			partMap = rand() % (part - 1);
		}
		while (!randPart[partMap]);
		randPart[partMap] = false;

		RoomPos.firstPoints.first = PartOfRooms[partMap].firstPoints.first + rand() % (static_cast<int>(floor(PartOfRooms[partMap].secondPoints.first / 2 - PartOfRooms[partMap].firstPoints.first)));
		RoomPos.firstPoints.second = PartOfRooms[partMap].firstPoints.second + rand() % (static_cast<int>(floor(PartOfRooms[partMap].secondPoints.second / 2 - PartOfRooms[partMap].firstPoints.second)));
		RoomPos.secondPoints.first = RoomPos.firstPoints.first + 4 + rand() % (PartOfRooms[partMap].secondPoints.first - RoomPos.firstPoints.first - 4);
		RoomPos.secondPoints.second = RoomPos.firstPoints.second + 4 + rand() % (PartOfRooms[partMap].secondPoints.second - RoomPos.firstPoints.second - 4);
		RoomsPos.push_back(RoomPos);
		}
		
	return RoomsPos;
}


void GamePlay() {
	Map map;
	int count = 5;
	vector<Room*>myRooms;
	vector<RoomPosition> RoomPos = GenRooms(count);
	for (int i = 0; i < count; i++) {
		myRooms.push_back(new EzRoom(RoomPos[i]));
	}
	map.SetPosition(myRooms);
	vector<Character*>gameObjects;
	for (int i = 0; i < myRooms.size(); i++) {
		for(int j = 0; j < 3; j++)
		gameObjects.push_back(myRooms[i]->SpawnEnemys()[j]);
	}
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
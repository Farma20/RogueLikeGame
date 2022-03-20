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

	void Damage(int _dm) {
		damage += _dm;
		return;
	}

	virtual void SetPosition(char _button) {};

	void SetPositionOld() { position = oldPosition; }

};

class Hero: public Character {
private:

public:
	Hero(pair<int, int> _pos) {
		heals = 100, damage = 10, LVL = 1, name = 'A', position = _pos;
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
			Enemys[i] = new Zombi(GenPointsInRoom());
		}
		return Enemys;
	}

	Character* SpawnHero() {
		Character* myHero = new Hero(GenPointsInRoom());
		return myHero;
	}

	pair<int, int> GenPointsInRoom() {
		pair<int, int>positionInRoom{
				(position.firstPoints.first + 1) + rand() % (position.secondPoints.first - (position.firstPoints.first + 2)),
				(position.firstPoints.second + 1) + rand() % (position.secondPoints.second - (position.firstPoints.second + 2))
		};
		return positionInRoom;
	}
};

class EzRoom : public Room {
public:
	EzRoom(RoomPosition _position) {
		enemyCount = 3, enemyMaxLVL = 1, itemsCount = 0, wall = '#', position = _position;
	}
};

class Map {
private:
	int width;
	int height;
	char floor;
	vector<vector<char>> location;

	void SelectRoudPoint(RoomPosition firstWall, RoomPosition secondWall, char line) {
		if (line == '-') {
			if ((firstWall.firstPoints.first < secondWall.firstPoints.first + 2) && (secondWall.firstPoints.first + 2 < firstWall.secondPoints.first)) {
				AddRoad({ secondWall.firstPoints.first + 2, firstWall.firstPoints.second },
					{ secondWall.firstPoints.first + 2, secondWall.firstPoints.second },
					line);
			}
			else if ((firstWall.firstPoints.first < secondWall.secondPoints.first - 2) && (secondWall.secondPoints.first - 2 < firstWall.secondPoints.first)) {
				AddRoad({ secondWall.secondPoints.first - 2, firstWall.firstPoints.second },
					{ secondWall.secondPoints.first - 2, secondWall.firstPoints.second },
					line);
			}
			else if ((secondWall.firstPoints.first < firstWall.firstPoints.first + 2) && (firstWall.firstPoints.first + 2 < secondWall.secondPoints.first)) {
				AddRoad({ firstWall.firstPoints.first + 2, firstWall.firstPoints.second },
					{ firstWall.firstPoints.first + 2, secondWall.firstPoints.second },
					line);
			}
			else if ((secondWall.firstPoints.first < firstWall.secondPoints.first - 2) && (firstWall.secondPoints.first - 2 < secondWall.secondPoints.first)) {
				AddRoad({ firstWall.secondPoints.first - 2, firstWall.firstPoints.second },
					{ firstWall.secondPoints.first - 2, secondWall.firstPoints.second },
					line);
			}
			else if ((secondWall.firstPoints.first == firstWall.firstPoints.first) && (secondWall.secondPoints.first == firstWall.secondPoints.first)) {
				AddRoad({ secondWall.firstPoints.first + 2, firstWall.firstPoints.second },
					{ secondWall.firstPoints.first + 2, secondWall.firstPoints.second },
					line);
			}
		}/*_______________________________*/
		else {
			if ((firstWall.firstPoints.second < secondWall.firstPoints.second + 2) && (secondWall.firstPoints.second + 2 < firstWall.secondPoints.second)) {
				AddRoad({ firstWall.firstPoints.first, secondWall.firstPoints.second + 2 },
					{ secondWall.firstPoints.first, secondWall.firstPoints.second + 2 },
					line);
			}
			else if ((firstWall.firstPoints.second < secondWall.secondPoints.second - 2) && (secondWall.secondPoints.second - 2 < firstWall.secondPoints.second)) {
				AddRoad({ firstWall.secondPoints.first, secondWall.secondPoints.second - 2 },
					{ secondWall.secondPoints.first, secondWall.secondPoints.second - 2 },
					line);
			}
			else if ((secondWall.firstPoints.second < firstWall.firstPoints.second + 2) && (firstWall.firstPoints.first + 2 < secondWall.secondPoints.second)) {
				AddRoad({ firstWall.firstPoints.first, firstWall.firstPoints.second + 2},
					{ secondWall.firstPoints.first, firstWall.firstPoints.second + 2 },
					line);
			}
			else if ((secondWall.firstPoints.second < firstWall.secondPoints.second - 2) && (firstWall.secondPoints.second - 2 < secondWall.secondPoints.second)) {
				AddRoad({ firstWall.secondPoints.first, firstWall.secondPoints.second - 2 },
					{ secondWall.secondPoints.first, firstWall.secondPoints.second - 2},
					line);
			}
		}
	}

	void AddRoad(pair<int, int>point1, pair<int, int>point2, char line) {
		if (line == '-') {
			for (int i = point1.second; i <= point2.second; i++) {
				if (i == point1.second || i == point2.second)
					location[point1.first][i] = '|';
				else {
					location[point1.first - 1][i] = 127;
					location[point1.first + 1][i] = 127;
				}
			}
		}
		else {
			for (int i = point1.first; i <= point2.first; i++) {
				if (i == point1.first || i == point2.first)
					location[i][point1.second] = '-';
				else {
					location[i][point1.second - 1] = 127;
					location[i][point1.second + 1] = 127;
				}
			}
		}
	}

public:
	Map() : location(vector<vector<char>>(height, vector<char>(width, ' '))), width(WorldSize.second), height(WorldSize.first), floor(' ') {}

	void WriteMap(){
		system("cls");
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

	void SetPosition(vector<Room*> _rooms) {
		for (Room* room: _rooms) {
			for (int i = room->GetRoomPosition().firstPoints.first; i <= room->GetRoomPosition().secondPoints.first; i++) {
				for (int j = room->GetRoomPosition().firstPoints.second; j <= room->GetRoomPosition().secondPoints.second; j++) {
					if (i == room->GetRoomPosition().firstPoints.first ||
						i == room->GetRoomPosition().secondPoints.first ||
						j == room->GetRoomPosition().firstPoints.second ||
						j == room->GetRoomPosition().secondPoints.second) {

						location[i][j] = room->GetWall();
					}
					else { location[i][j] = ' '; }
				}
			}
		}
	}

	void GenRoads(vector<Room*> _rooms) {
		for (int i = 0; i < _rooms.size()-1; i++) {

			RoomPosition firstWall, secondWall;

			if ((i == 0) || (i == _rooms.size() / 2 - 1)) {
				firstWall.firstPoints = { _rooms[i]->GetRoomPosition().secondPoints.first, _rooms[i]->GetRoomPosition().firstPoints.second };
				firstWall.secondPoints = _rooms[i]->GetRoomPosition().secondPoints;

				secondWall.firstPoints = _rooms[i + _rooms.size()/2]->GetRoomPosition().firstPoints;
				secondWall.secondPoints = { _rooms[i + _rooms.size() / 2]->GetRoomPosition().firstPoints.first, _rooms[i + _rooms.size() / 2]->GetRoomPosition().secondPoints.second };

				SelectRoudPoint(firstWall, secondWall, '||');
			}

			
			firstWall.firstPoints = {_rooms[i]->GetRoomPosition().firstPoints.first, _rooms[i]->GetRoomPosition().secondPoints.second};
			firstWall.secondPoints = _rooms[i]->GetRoomPosition().secondPoints;

			secondWall.firstPoints = _rooms[i+1]->GetRoomPosition().firstPoints;
			secondWall.secondPoints = { _rooms[i + 1]->GetRoomPosition().secondPoints.first, _rooms[i+1]->GetRoomPosition().firstPoints.second };

			SelectRoudPoint(firstWall, secondWall, '-');
		}	
	}

};


vector<RoomPosition>GenRooms(int count) {

	int part = count;
	vector<RoomPosition> PartOfRooms(part);
	vector<bool>randPart(part, true);
	int t1 = 0, l1 = 0, t2 = static_cast<int>(floor(WorldSize.first / 2) - 1), l2 = static_cast<int>(floor(WorldSize.second / (part/2) - 1));
	for (int i = 1; i <= part; i++) {
		RoomPosition partPoints;
		partPoints.firstPoints.first = t1; partPoints.firstPoints.second = l1;
		partPoints.secondPoints.first = t2; partPoints.secondPoints.second = l2;
		PartOfRooms[i-1] = partPoints;

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


		int borderV1 = static_cast<int>(floor(PartOfRooms[i].secondPoints.first / 2)) - PartOfRooms[i].firstPoints.first;
		int borderH1 = static_cast<int>(floor(PartOfRooms[i].secondPoints.second / 2)) - PartOfRooms[i].firstPoints.second;
		
		if (!borderV1) borderV1++;
		if (!borderH1) borderH1++;

		RoomPos.firstPoints.first = PartOfRooms[i].firstPoints.first + rand() % (borderV1);
		RoomPos.firstPoints.second = PartOfRooms[i].firstPoints.second + rand() % (borderH1);

		int borderV2 = PartOfRooms[i].secondPoints.first - RoomPos.firstPoints.first - 4;
		int borderH2 = PartOfRooms[i].secondPoints.second - RoomPos.firstPoints.second - 4;

		if (!borderV2) borderV2++;
		if (!borderH2) borderH2++;

		RoomPos.secondPoints.first = RoomPos.firstPoints.first + 4 + rand() % (borderV2);
		RoomPos.secondPoints.second = RoomPos.firstPoints.second + 4 + rand() % (borderH2);
		RoomsPos.push_back(RoomPos);
		}
		
	return RoomsPos;
}


void GamePlay() {
	Map map;
	int count = 8;
	vector<Room*>myRooms;
	vector<RoomPosition> RoomPos = GenRooms(count);
	int heroRoom = rand() % count - 1;
	Character* hero;
	for (int i = 0; i < count; i++) {
		myRooms.push_back(new EzRoom(RoomPos[i]));
		if (heroRoom == i)
			hero = myRooms[i]->SpawnHero();
	}
	map.SetPosition(myRooms);
	vector<Character*>gameObjects;
	for (int i = 0; i < myRooms.size(); i++) {
		for(int j = 0; j < 3; j++)
			gameObjects.push_back(myRooms[i]->SpawnEnemys()[j]);
	}

	gameObjects.push_back(hero);
	
	map.GenRoads(myRooms);
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
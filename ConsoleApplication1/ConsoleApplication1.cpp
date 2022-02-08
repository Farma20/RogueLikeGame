#include <iostream>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include<ctime>
#include <windows.h>

using namespace std;

pair <int, int> WorldSize{ 20, 20 };

class Character {
protected:
	int heals;
	int damage;
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
		heals = 100, damage = 10,  name = 'A', position = { 0, 0 };
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
	Zombi() {
		heals = 20, damage = 5, name = 'Z', oldPosition = position = { rand() % WorldSize.first, rand() % WorldSize.second};
	}

	void SetPosition(char _button) override {
		if (_button == 'w' || _button == 's' || _button == 'a' || _button == 'd') {
			int rand_x = rand() % 3 - 1;
			int rand_y = rand() % 3 - 1;
			if ((position.first + rand_x < WorldSize.first) && (position.first + rand_x >= 0) &&
				(position.second + rand_y < WorldSize.second) && (position.second + rand_y >= 0)) {
				
				oldPosition = position;
				position = { position.first + rand_x, position.second + rand_y };
			}
		}
		
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
	Map() : location(vector<vector<char>>(width, vector<char>(height, '_'))), width(WorldSize.first), height(WorldSize.second), floor('_') {}

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
};

void GamePlay() {
	Map map;
	Character* hero = new Hero;
	Zombi zombi;
	vector<Character*>gameObjects(10);
	for (int i = 0; i < 10; i++) {
		gameObjects[i] = new Zombi;
	}
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
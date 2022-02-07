#include <iostream>
#include <vector>
#include <conio.h>
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

};

class Hero: public Character {
private:

public:
	Hero() {
		heals = 100, damage = 20,  name = 'A', position = { 0, 0 };
	};

	void SetPosition(char _button) {
		int buttonCode = static_cast<int>(_button);
		if (buttonCode == static_cast<int>('s')) {
			if (position.first + 1 < WorldSize.first) {
				oldPosition = position;
				position.first++;
			}
		}
		else if (buttonCode == static_cast<int>('w')) {
			if (position.first - 1 >= 0) {
				oldPosition = position;
				position.first--;
			}
		}
		else if (buttonCode == static_cast<int>('d')) {
			if (position.second + 1 < WorldSize.second) {
				oldPosition = position;
				position.second++;
			}
		}
		else if (buttonCode == static_cast<int>('a')) {
			if (position.second - 1 >= 0) {
				oldPosition = position;
				position.second--;
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

	void SetPosition(Character& Obj) {
		location[Obj.GetOldPosition().first][Obj.GetOldPosition().second] = floor;
		location[Obj.GetPosition().first][Obj.GetPosition().second] = Obj.GetName();
	}
};

void GamePlay() {
	Map map;
	Hero hero;
	char moveBatton;
	while (1) {
		map.SetPosition(hero);
		map.WriteMap();
		moveBatton = _getch();
		hero.SetPosition(moveBatton);
	}
}

int main() {
	GamePlay();
}
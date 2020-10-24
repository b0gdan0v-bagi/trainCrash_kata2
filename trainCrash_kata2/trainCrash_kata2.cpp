
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <vector>

struct Pos
{
	int x, y;
	Pos(int X, int Y) : x(X), y(Y) {}
};

struct Map
{
	Pos pos;
	int c_d, ac_d; //clockwise and anti clock wise;
	Map(int X, int Y, int C_D, int AC_D)
		:pos(Pos(X, Y)), c_d(C_D), ac_d(AC_D) {}
	Map(Pos p, int C_D, int AC_D)
		:pos(p), c_d(C_D), ac_d(AC_D) {}
	Map();
};

Pos getPosByLink(int n, Pos p)
{
	switch (n)
	{
	case 1: return Pos(p.x - 1, p.y - 1);
	case 2: return Pos(p.x, p.y - 1);
	case 3: return Pos(p.x + 1, p.y - 1);
	case 4: return Pos(p.x - 1, p.y);
	case 5: return Pos(p.x, p.y);
	case 6: return Pos(p.x + 1, p.y);
	case 7: return Pos(p.x - 1, p.y + 1);
	case 8: return Pos(p.x, p.y + 1);
	case 9: return Pos(p.x + 1, p.y + 1);

	default:
		break;
	}
	return p;
}

Map constuctMap(int n, Pos p)
{
	switch (n)
	{
	case 1: return Map(Pos(p.x - 1, p.y - 1), 0, 9);// Pos(p.x - 1, p.y - 1);
	case 2: return Map(Pos(p.x, p.y - 1),     0, 8);
	case 3: return Map(Pos(p.x + 1, p.y - 1), 0, 7);
	case 4: return Map(Pos(p.x - 1, p.y),     0, 6);
	case 5: return Map(Pos(p.x, p.y),         0, 5);
	case 6: return Map(Pos(p.x + 1, p.y),     0, 4);
	case 7: return Map(Pos(p.x - 1, p.y + 1), 0, 3);
	case 8: return Map(Pos(p.x, p.y + 1),     0, 2);
	case 9: return Map(Pos(p.x + 1, p.y + 1), 0, 1);

	default:
		break;
	}
}


void show(std::vector<std::string> v)
{
	for (auto& i : v) std::cout << i << "\n";
}

Pos startPos(std::vector<std::string> v)
{
	for (int i = 0; i < v.size(); i++)
		for (int j = 0; j < v[i].size(); j++)
			if ((v[i][j] != ' ') && (v[i][j] != 'W')) return Pos(j, i);
}

int train_crash(const std::string& track, const std::string& a_train, int a_train_pos, const std::string& b_train, int b_train_pos, int limit)
{
    std::string to;
	std::vector<std::string> vTrack;
    std::stringstream ss(track);
	int maxWidthSize = 0;
    while (std::getline(ss, to, '\n'))
    {
		to.insert(0, "W");
		vTrack.push_back(to);
		if (maxWidthSize < to.size())maxWidthSize = to.size();
        //std::cout << to << "\n";
    }
	maxWidthSize++;
	for (auto& i : vTrack)
		while (i.size() < maxWidthSize) i += "W";

	std::string wall;
	while (wall.size() < maxWidthSize) wall += "W";

	vTrack.insert(vTrack.begin(),wall);
	vTrack.push_back(wall);

	

	Pos p = startPos(vTrack);
	//std::cout << startPos(vTrack).y << " " << startPos(vTrack).x << "\n";
	//p.y++;
	Pos pStart = p;
	int link = 0;
	if (vTrack[pStart.y + 1][pStart.x - 1] == '/') link = 7;
	if (vTrack[pStart.y + 1][pStart.x] == '|') link = 8;

	std::vector<Map> road;
	Map map = Map(p, 6, link);
	
	road.push_back(map);
	//p.x++;
	std::vector<std::string> vTrack_show = vTrack;
	//std::cout << map.c_d << "\n";
	
	while (true)
	{
		map = constuctMap(map.c_d, p);
		std::cout << "vTrack = " << vTrack[p.y][p.x] << "\n" << "x,y = " << p.x << " " << p.y << "\n";
		std::cout << "map -  " << map.pos.x << " " << map.pos.y << "\n" ;
		p = map.pos;
		switch (vTrack[p.y][p.x])
		{
		case '-': {
			map.c_d = 6;
			break; }
		case '|': {
			map.c_d = 8;
			break; }
		case '/': { break; }
		case '\\': {
			//std::cout << " WOW " << "\n";
			if ((vTrack[p.y + 1][p.x] == '|') || (vTrack[p.y + 1][p.x] == '+')) map.c_d = 8;
			if ((vTrack[p.y + 1][p.x + 1] == '\\') || (vTrack[p.y + 1][p.x + 1] == 'S') 
				|| (vTrack[p.y + 1][p.x + 1] == 'X')) map.c_d = 9;
			break; }
		case '+': { break; }
		case 'X': { break; }
		case 'S': {
			map.c_d = 8;
			break; }
		default:
			break;
		}
		
		std::cout << map.c_d << "\n";
		road.push_back(map);
		vTrack_show[p.y][p.x] = '*';
		
		//std::cout << p.x << "\n";
		if (vTrack[p.y][p.x] != '-')
		{
			show(vTrack_show);
			system("pause");
		}
	}
	
	std::cout << road[1].pos.x << " " << road[1].pos.y << "\n";
	
	p = road[1].pos;
	road.push_back(constuctMap(6, p));
	std::cout << road[2].pos.x << " " << road[2].pos.y << "\n";
	//std::cout << getPosByLink(6, p).x << " " << getPosByLink(6, p).y;
	switch (vTrack[p.y][p.x])
	{
	case '-': {std::cout << "\nWOW"; break;
	}
	default:
		break;
	}

    return 42;
}

int main()
{
	/*
	https://www.codewars.com/kata/59b47ff18bcb77a4d1000076/train/cpp
	*/

	const std::string example_track =
		"                                /------------\\\n"
		"/-------------\\                /             |\n"
		"|             |               /              S\n"
		"|             |              /               |\n"
		"|        /----+--------------+------\\        |   \n"
		"\\       /     |              |      |        |     \n"
		" \\      |     \\              |      |        |                    \n"
		" |      |      \\-------------+------+--------+---\\\n"
		" |      |                    |      |        |   |\n"
		" \\------+--------------------+------/        /   |\n"
		"        |                    |              /    | \n"
		"        \\------S-------------+-------------/     |\n"
		"                             |                   |\n"
		"/-------------\\              |                   |\n"
		"|             |              |             /-----+----\\\n"
		"|             |              |             |     |     \\\n"
		"\\-------------+--------------+-----S-------+-----/      \\\n"
		"              |              |             |             \\\n"
		"              |              |             |             |\n"
		"              |              \\-------------+-------------/\n"
		"              |                            |               \n"
		"              \\----------------------------/ \n";

	train_crash(example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000);
}

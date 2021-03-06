
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <vector>

struct Pos
{
	int x, y;
	Pos(int X, int Y) : x(X), y(Y) {}
	//Pos();
	friend bool operator== (const Pos& p1, const Pos& p2);
};

bool operator== (const Pos& p1, const Pos& p2)
{
	return (p1.x == p2.x &&
		p1.y == p2.y);
}

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

struct Station
{
	Pos pos;
	int roadPlace;
	//clockwise and anti clock wise;
	Station(int X, int Y, int R)
		:pos(Pos(X, Y)), roadPlace(R) {}
	Station(Pos p, int R)
		:pos(p), roadPlace(R) {}
	Station();
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

struct Train
{
	std::vector<Pos> carriagePos;
	Pos headPos = { 0,0 };
	int roadPos, length;
	int stopTimer = { 0 };
	char type;
	bool express = { false };
	bool clockWise;
	Train(int ROAD_POS, std::string NAME) :roadPos(ROAD_POS)
	{
		//std::cout << NAME.size() << " = size\n";
		char ascii = NAME[0];
		length = NAME.size()-1;
		if (isupper(ascii)) {
			//std::cout << ascii << " = ascii\n";
			clockWise = false;
			type = ascii;	
		}
		else 
		{
			clockWise = true;
			type = toupper(ascii);
		}
		if (type == 'X') express = true;
	}

};

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

int corrPos(int c, int const m)
{
	if (c > m) return c - m - 1;
	if (c < 0) return m + c + 1;
	return c;
}

int train_crash(const std::string& track, const std::string& a_train, int a_train_pos, const std::string& b_train, int b_train_pos, int limit)
{
    std::string to;
	std::vector<std::string> vTrack;
	std::cout << "A " << a_train << " " << a_train_pos << "\n";
	std::cout << "B " << b_train << " " << b_train_pos << "\n";
	std::cout << "limit = " << limit << "\n";
    std::stringstream ss(track);
	int maxWidthSize = 0;
    while (std::getline(ss, to, '\n'))
    {
		
		to.insert(0, "W");
		vTrack.push_back(to);
		std::cout << to << "\n";
		if (maxWidthSize < to.size())maxWidthSize = to.size();
    }
	maxWidthSize++;
	for (auto& i : vTrack)
		while (i.size() < maxWidthSize) i += "W";

	std::string wall;
	while (wall.size() < maxWidthSize) wall += "W";

	vTrack.insert(vTrack.begin(),wall);
	vTrack.push_back(wall);

	

	Pos p = startPos(vTrack);
	Pos pStart = p;
	int link = 0;
	if (vTrack[pStart.y + 1][pStart.x - 1] == '/') link = 7;
	if (vTrack[pStart.y + 1][pStart.x] == '|') link = 8;

	std::vector<Map> road;
	Map map = Map(p, 6, link);
	std::vector<Station> stations;
	std::vector<Train> trains;
	road.push_back(map);
	std::vector<std::string> vTrack_show = vTrack;
	int count = 0;
	while (true)
	{
		count++;
		map = constuctMap(map.c_d, p);
		p = map.pos;
		switch (vTrack[p.y][p.x])
		{
		case '-': {
			//std::cout << map.ac_d << " map.ac_d\n";
			switch (map.ac_d)
			{
			case 4: {map.c_d = 6; break; }
			case 6: {map.c_d = 4; break; }
			default: break;
			}

			break; }
		case '|': {
			switch (map.ac_d)
			{
			case 2: {map.c_d = 8; break; }
			case 8: {map.c_d = 2; break; }
			default: break;
			}
			break; }
		case '/': { 
			if ((map.ac_d == 7) || (map.ac_d == 8)) {
				if ((vTrack[p.y - 1][p.x + 1] == '/') || (vTrack[p.y - 1][p.x + 1] == 'S')
					|| (vTrack[p.y - 1][p.x + 1] == 'X')) map.c_d = 3;
				if ((vTrack[p.y - 1][p.x] == '|') || (vTrack[p.y - 1][p.x] == '+')) map.c_d = 2;
			}
			if (vTrack[p.y][p.x - 1] == '-') map.c_d = 4;
			if (vTrack[p.y][p.x +1 ] == '-') map.c_d = 6;
			if ((map.ac_d == 2) || (map.ac_d == 3)) {
				if ((vTrack[p.y + 1][p.x] == '|') || (vTrack[p.y + 1][p.x] == '+')) map.c_d = 8;
				if ((vTrack[p.y + 1][p.x - 1] == '/') || (vTrack[p.y + 1][p.x - 1] == 'S')
					|| (vTrack[p.y + 1][p.x - 1] == 'X')) map.c_d = 7;
			}
			if ((map.ac_d == 4) || (map.ac_d == 6)) {
				if (vTrack[p.y - 1][p.x] == '|') map.c_d = 2;
				
			}
			if ((map.ac_d == 6) || (map.ac_d == 3)) {
				if ((vTrack[p.y + 1][p.x - 1] == '/') || (vTrack[p.y + 1][p.x - 1] == 'X')
					|| (vTrack[p.y + 1][p.x - 1] == 'S')) map.c_d = 7;
				if (vTrack[p.y + 1][p.x] == '|') map.c_d = 8;
			}
			if ((map.ac_d == 4) || (map.ac_d == 7)) {
				if (vTrack[p.y - 1][p.x + 1] == '/') map.c_d = 3;
			}
			break; }
		case '\\': {
			if ((map.ac_d == 4) || (map.ac_d == 3) || (map.ac_d == 2)) {
				if ((vTrack[p.y + 1][p.x] == '|') || (vTrack[p.y + 1][p.x] == '+')) map.c_d = 8;
				if ((vTrack[p.y + 1][p.x + 1] == '\\') || (vTrack[p.y + 1][p.x + 1] == 'S')
					|| (vTrack[p.y + 1][p.x + 1] == 'X') ) map.c_d = 9;
			}
			if ((map.ac_d == 6) || (map.ac_d == 7)) {
				if ((vTrack[p.y - 1][p.x] == '|') || (vTrack[p.y - 1][p.x] == '+')) map.c_d = 2;
				if ((vTrack[p.y - 1][p.x - 1] == '\\') || (vTrack[p.y - 1][p.x - 1] == 'S')
					|| (vTrack[p.y - 1][p.x - 1] == 'X')) map.c_d = 1;
			}
			if ((map.ac_d == 8) || (map.ac_d == 9)) {
				if (vTrack[p.y][p.x - 1] == '-') map.c_d = 4;
				if ((vTrack[p.y - 1][p.x] == '|') || (vTrack[p.y - 1][p.x] == '+')) map.c_d = 2;
				if ((vTrack[p.y - 1][p.x - 1] == '\\') || (vTrack[p.y - 1][p.x - 1] == 'S')
					|| (vTrack[p.y - 1][p.x - 1] == 'X')) map.c_d = 1;
			}
			if ((map.ac_d == 1) || (map.ac_d == 2)) {
				if (vTrack[p.y][p.x + 1] == '-') map.c_d = 6;
				if ((vTrack[p.y+1][p.x + 1] == '\\') || (vTrack[p.y + 1][p.x + 1] == 'X') 
					|| (vTrack[p.y + 1][p.x + 1] == 'S'))  map.c_d = 9;
				if (vTrack[p.y + 1][p.x] == '|')  map.c_d = 8;
			}
			break; }
		case '+': {
			switch (map.ac_d)
			{
			case 2: {map.c_d = 8; break; }
			case 8: {map.c_d = 2; break; }
			case 4: {map.c_d = 6; break; }
			case 6: {map.c_d = 4; break; }
			default:
				break;
			}
			//map.c_d = 8;
			break; }
		case 'X': { 
			switch (map.ac_d)
			{
			case 1: {map.c_d = 9; break; }
			case 9: {map.c_d = 1; break; }
			case 3: {map.c_d = 7; break; }
			case 7: {map.c_d = 3; break; }
			default:
				break;
			}
			break; }
		case 'S': {
			stations.push_back(Station(p, count));
			switch (map.ac_d)
			{
			case 1: {map.c_d = 9; break; }
			case 9: {map.c_d = 1; break; }
			case 3: {map.c_d = 7; break; }
			case 7: {map.c_d = 3; break; }
			case 2: {map.c_d = 8; break; }
			case 8: {map.c_d = 2; break; }
			case 4: {map.c_d = 6; break; }
			case 6: {map.c_d = 4; break; }
			default:
				break;
			}
			break; }
		default:
			break;
		}
		//vTrack_show[p.y][p.x] = '*';
		//show(vTrack_show);
		//std::cout << map.ac_d << " " << map.c_d << "\n";
		//system("pause");

		road.push_back(map);

		if ((count > 2) && (p.x == pStart.x) && (p.y == pStart.y)) break;
		if (count > vTrack.size() * vTrack[0].size()) return -3; // loop error
	}
	road.pop_back();
	int const roadMax = road.size() - 1;
	int curRoadPos;

	for (auto& i : stations) vTrack_show[i.pos.y][i.pos.x] = 'S';
	trains.push_back(Train(a_train_pos, a_train));
	trains.push_back(Train(b_train_pos, b_train));
	for (auto& tr : trains)
	{
		tr.headPos = road[corrPos(tr.roadPos, roadMax)].pos;
		vTrack_show[tr.headPos.y][tr.headPos.x] = tr.type;
		for (int i = 0; i < tr.length; i++)
		{
			if (!tr.clockWise) tr.carriagePos.push_back(road[corrPos(tr.roadPos + i + 1, roadMax)].pos);
			else tr.carriagePos.push_back(road[corrPos(tr.roadPos - i - 1, roadMax)].pos);
			vTrack_show[tr.carriagePos[i].y][tr.carriagePos[i].x] = tolower(tr.type);
		}
	}

	count = 0;
	while (true)
	{

		//collission
		if (trains[0].headPos == trains[1].headPos) return count;
		for (auto& tr : trains)
		{
			for (int i = 0; i < trains[0].length; i++) if (tr.headPos == trains[0].carriagePos[i]) return count;
			for (int i = 0; i < trains[1].length; i++) if (tr.headPos == trains[1].carriagePos[i]) return count;
		}
		for (auto& tr : trains)
			for (int i = 0; i < tr.length; i++)
				for (int j = 0; j < tr.length; j++)
					if (i != j)
						if (tr.carriagePos[i] == tr.carriagePos[j]) return count;
		for (int i = 0; i < trains[0].length; i++)
			for (int j = 0; j < trains[1].length; j++)
				if (trains[0].carriagePos[i] == trains[1].carriagePos[j]) return count;
		if (limit == 0) return -1;
		//moving
		for (auto& tr : trains)
		{
			if (tr.express) tr.stopTimer = 0;
			if (tr.stopTimer == 0)
			{
				if (!tr.clockWise) {
					tr.headPos = getPosByLink(road[corrPos(tr.roadPos, roadMax)].ac_d, tr.headPos);
					vTrack_show[tr.headPos.y][tr.headPos.x] = tr.type;
					for (int i = 0; i < tr.length; i++)
					{

						tr.carriagePos[i] = getPosByLink(road[corrPos(tr.roadPos + i + 1, roadMax)].ac_d, tr.carriagePos[i]);
						vTrack_show[tr.carriagePos[i].y][tr.carriagePos[i].x] = tolower(tr.type);
					}
					Pos t = getPosByLink(road[corrPos(tr.roadPos + tr.length + 1, roadMax)].ac_d, road[corrPos(tr.roadPos + tr.length + 1, roadMax)].pos);
					vTrack_show[t.y][t.x] = '*';
					tr.roadPos--;
					if (tr.roadPos < 0) tr.roadPos = roadMax;
					for (auto& i : stations) if (tr.roadPos == i.roadPlace) tr.stopTimer = tr.length;
				}
				else
				{
					tr.headPos = getPosByLink(road[corrPos(tr.roadPos, roadMax)].c_d, tr.headPos);
					vTrack_show[tr.headPos.y][tr.headPos.x] = tr.type;
					for (int i = 0; i < tr.length; i++)
					{
						tr.carriagePos[i] = getPosByLink(road[corrPos(tr.roadPos - i - 1, roadMax)].c_d, tr.carriagePos[i]);
						vTrack_show[tr.carriagePos[i].y][tr.carriagePos[i].x] = tolower(tr.type);
						//std::cout << corrPos(tr.roadPos + i + 1, roadMax) << " tr.roadPos+i+1\n";
						//std::cout << tolower(tr.type) << " " << tr.carriagePos[i].y << " " << tr.carriagePos[i].x << "\n";
					}
					Pos t = getPosByLink(road[corrPos(tr.roadPos - tr.length - 1, roadMax)].c_d, road[corrPos(tr.roadPos - tr.length - 1, roadMax)].pos);
					vTrack_show[t.y][t.x] = '*';
					tr.roadPos++;
					if (tr.roadPos > roadMax) tr.roadPos = 0;
					for (auto& i : stations) if (tr.roadPos == i.roadPlace) tr.stopTimer = tr.length;
				}
			}
			else tr.stopTimer--;
		}
		count++;

		if (count == limit + 1) return -1;
	}
	return -2; // ERROR
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

	const std::string s4 =
		"/-------\\\n"
		"|       |\n"
		"|       |\n"
		"|       |\n"
		"\\-------S--------\\ \n"
		"        |        | \n"
		"        |        | \n"
		"        |        | \n"
		"        \\--------/\n";

	const std::string Crashes__Tricky =
		"/----\\     /----\\\n"
		"|     \\   /     |\n"
		"|      \\ /      |\n"
		"|       S       |\n"
		"|      / \\      |\n"
		"|     /   \\     |\n"
		"\\----/     \\----/\n";

	const std::string r1 =
		"/------\\               /--\\\n"
		"|      |               |  |\n"
		"|      \\---------------/  |\n"
		"\\------\\               /--/\n"
		"       |               |\n"
		"       \\---------------/\n";
	const std::string No_crash_0___Tricky =
		"/-----\\   /-----\\   /-----\\   /-----\\\n"
		"|      \\ /       \\ /       \\ /      |\n"
		"|       X         X         X       |\n"
		"|      / \\       / \\       / \\      |\n"
		"\\-----/   \\-----/   \\-----/   \\-----/\n";

	const std::string Start_exactly_on_the_station =
		"/------S----------\\\n"
		"|                 |\n"
		"|                 |\n"
		"|                 |\n"
		"|                 |\n"
		"\\----------S------/\n";
	std::cout << "\nANSWER = " << train_crash(example_track, "Aaaa", 147, "Bbbbbbbbbbb", 288, 1000) << "\n";
	std::cout << "\nANSWER = " << train_crash(s4, "aaaaaA", 10, "bbbbbB", 20, 100) << "\n";
	std::cout << "\nANSWER = " << train_crash(Crashes__Tricky, "aaaaaA", 10, "bbbbbB", 20, 100) << "\n";
	std::cout << "\nANSWER = " << train_crash(r1, "aaaaaA", 10, "bbbbbB", 20, 100) << "\n";
	std::cout << "\nANSWER = " << train_crash(No_crash_0___Tricky, "aaaA", 16, "bbbB", 5, 100) << "\n";
	std::cout << "\nANSWER = " << train_crash(Start_exactly_on_the_station, "aaA", 7, "Bbbb", 9, 100) << "\n";
	std::cout << "\nANSWER = " << train_crash(Start_exactly_on_the_station, "aaaaaaaaaaaaA", 7, "xxxX", 30, 100) << "\n";
}

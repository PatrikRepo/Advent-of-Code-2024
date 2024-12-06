#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <unordered_set>

void parseInput(std::vector<std::string> &map)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
		while(getline(input,line))
		{
			map.push_back(line);
		}
  }
  input.close();
}

std::pair<uint64_t,uint64_t> traverseMap(std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	size_t startX = 0;
	size_t startY = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '^')
			{
				startX = j;
				startY = i;
				goto startPosFound;
			}
		}
	}
startPosFound:

	unsigned direction = 0;
	size_t x = startX;
	size_t y = startY;
	result.first = 1; //startPos
	std::vector<std::pair<size_t,size_t>> obstructions;
	
	while(x>0 && x+1<map[0].size() && y>0 && y+1<map.size())
	{
		map[y][x] = 'X';
		bool turn = true;
		while(turn)
		{
			switch(direction)
			{
				case 0 : if(map[y-1][x] != '#') turn = false; break;
				case 1 : if(map[y][x+1] != '#') turn = false; break;
				case 2 : if(map[y+1][x] != '#') turn = false; break;
				case 3 : if(map[y][x-1] != '#') turn = false; break;
			}
			if(turn)
			{
				direction = (direction+1)%4;
			}
		}
		
		switch(direction)
		{
			case 0:
			{
				while(y+1>0 && map[y][x] != '#')
				{
					if(map[y][x] == '.')
					{
						result.first++;
						obstructions.emplace_back(x,y);
					}
					map[y][x] = 'X';
					y--;
				}
				y++;
				break;
			}
			case 1:
			{
				while(x<map[0].size() && map[y][x] != '#')
				{
					if(map[y][x] == '.')
					{
						result.first++;
						obstructions.emplace_back(x,y);
					}
					map[y][x] = 'X';
					x++;
				}
				x--;
				break;
			}
			case 2:
			{
				while(y<map.size() && map[y][x] != '#')
				{
					if(map[y][x] == '.')
					{
						result.first++;
						obstructions.emplace_back(x,y);
					}
					map[y][x] = 'X';
					y++;
				}
				y--;
				break;
			}
			case 3:
			{
				while(x+1>0 && map[y][x] != '#')
				{
					if(map[y][x] == '.')
					{
						result.first++;
						obstructions.emplace_back(x,y);
					}
					map[y][x] = 'X';
					x--;
				}
				x++;
				break;
			}
		}
		
		if(map[y][x] == '.')
		{
			result.first++;
		}
	}

	for(auto obstruction:obstructions)
	{
		int direction = 0;
		x = startX;
		y = startY;
		map[obstruction.second][obstruction.first] = '#';
		
		std::unordered_set<uint64_t> cache;
		
		while(x>0 && x+1<map[0].size() && y>0 && y+1<map.size())
		{
			if(cache.count(y*10000000 + x*10 + direction) == 1)
			{
				result.second += 1;
				break;
			}
			cache.insert(y*10000000 + x*10 + direction);
			bool turn = true;
			while(turn)
			{
				switch(direction)
				{
					case 0 : if(map[y-1][x] != '#') turn = false; break;
					case 1 : if(map[y][x+1] != '#') turn = false; break;
					case 2 : if(map[y+1][x] != '#') turn = false; break;
					case 3 : if(map[y][x-1] != '#') turn = false; break;
				}
				if(turn)
				{
					direction = (direction+1)%4;
				}
			}
			
			switch(direction)
			{
				case 0:
				{
					while(y+1>0 && map[y][x] != '#')
					{
						y--;
					}
					y++;
					break;
				}
				case 1:
				{
					while(x<map[0].size() && map[y][x] != '#')
					{
						x++;
					}
					x--;
					break;
				}
				case 2:
				{
					while(y<map.size() && map[y][x] != '#')
					{
						y++;
					}
					y--;
					break;
				}
				case 3:
				{
					while(x+1>0 && map[y][x] != '#')
					{
						x--;
					}
					x++;
					break;
				}
			}
		}
		map[obstruction.second][obstruction.first] = '.';
	}

	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<std::string> map;
	
  parseInput(map);

	result = traverseMap(map);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

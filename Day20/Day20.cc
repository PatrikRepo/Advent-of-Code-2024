#include <iostream>
#include <fstream>
#include <vector>
#include <array>

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

std::pair<uint64_t,uint64_t> countCheats(const std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result;
	
	std::array<std::array<int,141>,141> stepMap;
	for(auto &line:stepMap)
	{
		line.fill(-1);
	}	
	
	unsigned x = 0;
	unsigned y = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == 'S')
			{
				x = j;
				y = i;
			}
		}
	}
	
	unsigned step = 0;
	
	while(map[y][x] != 'E')
	{
		stepMap[y][x] = step;
		if(map[y-1][x] != '#' && stepMap[y-1][x] == -1) y--;
		else if(map[y][x+1] != '#' && stepMap[y][x+1] == -1) x++;
		else if(map[y+1][x] != '#' && stepMap[y+1][x] == -1) y++;
		else if(map[y][x-1] != '#' && stepMap[y][x-1] == -1) x--;
		step++;
	}	
	stepMap[y][x] = step;
	
	int cheatSize = 20;
	for(int i=1; i<(int)map.size()-1; i++)
	{
		for(int j=1; j<(int)map[0].size()-1; j++)
		{
			if(map[i][j] != '#')
			{
				for(int k = (i - cheatSize >= 1) ? i - cheatSize : 0; k <= ((i + cheatSize + 1 < (int)map.size()) ? i + cheatSize : (int)map.size()-1); k++)
				{
					for(int l = (j - cheatSize >= 1) ? j - cheatSize : 0; l <= ((j + cheatSize + 1 < (int)map[0].size()) ? j + cheatSize : (int)map[0].size()-1); l++)
					{
						if(abs(k-i) + abs(l-j) <= cheatSize && map[k][l] != '#' && stepMap[k][l] - (stepMap[i][j] + abs(k-i) + abs(l-j)) >= 100)
						{
							if(abs(k-i) + abs(l-j) <= 2)
							{
								result.first++;
							}
							result.second++;
						}
					}
				}
			}
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<std::string> map;

	parseInput(map);

	result = countCheats(map);

	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;

	return 0;
}


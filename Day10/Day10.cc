#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <tuple>
#include <utility>

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

std::pair<uint64_t,uint64_t> findTrails(const std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] != '0') continue;

			std::unordered_set<uint64_t> tops;
			std::vector<std::tuple<unsigned,unsigned,unsigned>> steps;
			steps.emplace_back(0,j,i);
			while(!steps.empty())
			{
				auto step = steps.back();
				steps.pop_back();
				int count = std::get<0>(step);
				unsigned x = std::get<1>(step);
				unsigned y = std::get<2>(step);
				
				if(count == 9)
				{
					tops.emplace(x*std::max(map.size(),map[0].size()) + y);
					result.second++;
					continue;
				}
				
				if(y>0 && map[y-1][x] == count + 1 + '0') steps.emplace_back(count + 1, x, y-1);
				if(x+1<map[0].size() && map[y][x+1] == count + 1 + '0') steps.emplace_back(count + 1, x+1, y);
				if(y+1<map.size() && map[y+1][x] == count + 1 + '0') steps.emplace_back(count + 1, x, y+1);
				if(x>0 && map[y][x-1] == count + 1 + '0') steps.emplace_back(count + 1, x-1, y);
			}
			result.first += tops.size();
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;
	
	std::vector<std::string> map;
	
  parseInput(map);

	result = findTrails(map);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

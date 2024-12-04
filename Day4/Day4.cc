#include <iostream>
#include <fstream>
#include <vector>

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

uint64_t findWords(const std::vector<std::string> &map)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == 'X')
			{
				if(j+3 < map[0].size() && map[i][j+1] == 'M' && map[i][j+2] == 'A' && map[i][j+3] == 'S') result++;
				if(j+3 < map[0].size() && i+3 < map.size() && map[i+1][j+1] == 'M' && map[i+2][j+2] == 'A' && map[i+3][j+3] == 'S') result++;
				if(i+3 < map.size() && map[i+1][j] == 'M' && map[i+2][j] == 'A' && map[i+3][j] == 'S') result++;
				if(j >= 3 && i+3 < map.size() && map[i+1][j-1] == 'M' && map[i+2][j-2] == 'A' && map[i+3][j-3] == 'S') result++;
				if(j >= 3 && map[i][j-1] == 'M' && map[i][j-2] == 'A' && map[i][j-3] == 'S') result++;
				if(j >= 3 && i >= 3 && map[i-1][j-1] == 'M' && map[i-2][j-2] == 'A' && map[i-3][j-3] == 'S') result++;
				if(i >= 3 && map[i-1][j] == 'M' && map[i-2][j] == 'A' && map[i-3][j] == 'S') result++;
				if(j+3 < map[0].size() && i >= 3 && map[i-1][j+1] == 'M' && map[i-2][j+2] == 'A' && map[i-3][j+3] == 'S') result++;
			}
		}
	}
	
	return result;
}

uint64_t findX(const std::vector<std::string> &map)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == 'A')
			{
				if(j>0 && j+1 < map[0].size() && i>0 && i+1 < map.size() && map[i-1][j-1] == 'M' && map[i+1][j+1] == 'S' && map[i+1][j-1] == 'M' && map[i-1][j+1] == 'S') result++;
				else if(j>0 && j+1 < map[0].size() && i>0 && i+1 < map.size() && map[i-1][j-1] == 'S' && map[i+1][j+1] == 'M' && map[i+1][j-1] == 'M' && map[i-1][j+1] == 'S') result++;
				else if(j>0 && j+1 < map[0].size() && i>0 && i+1 < map.size() && map[i-1][j-1] == 'S' && map[i+1][j+1] == 'M' && map[i+1][j-1] == 'S' && map[i-1][j+1] == 'M') result++;
				else if(j>0 && j+1 < map[0].size() && i>0 && i+1 < map.size() && map[i-1][j-1] == 'M' && map[i+1][j+1] == 'S' && map[i+1][j-1] == 'S' && map[i-1][j+1] == 'M') result++;
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<std::string> map;
	
  parseInput(map);

	resultA = findWords(map);
	resultB = findX(map);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

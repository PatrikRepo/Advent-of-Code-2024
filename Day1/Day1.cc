#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

void parseInput(std::array<std::vector<unsigned>,2> &lists)
{
	std::string line;
  std::ifstream input("input.txt");
  
  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		size_t pos = line.find_first_of(" ");
  		lists[0].push_back(std::stoi(line.substr(0,pos)));
  		pos = line.find_first_of("1234567890", pos);
  		lists[1].push_back(std::stoi(line.substr(pos)));
  	}
  }
  input.close();
}

uint64_t compareLists(std::array<std::vector<unsigned>,2> &lists)
{	
	uint64_t result = 0;
	
	std::sort(lists[0].begin(), lists[0].end());
	std::sort(lists[1].begin(), lists[1].end());
	
	for(unsigned i=0; i<lists[0].size(); i++)
	{
		result += abs(lists[0][i] - lists[1][i]);
	}
	
	return result;
}

uint64_t similarLists(std::array<std::vector<unsigned>,2> &lists)
{
	uint64_t result = 0;
	
	uint64_t numsum = 0;
	unsigned num = 0;
	unsigned index = 0;
	for(unsigned j=0; j<lists[0].size(); j++)
	{
		if(num == lists[0][j])
		{
			result += numsum;
			continue;
		}
		
		num = lists[0][j];
		
		unsigned multiplier = 0;
		while(index < lists[1].size() && lists[1][index] <= num)
		{
			if(num == lists[1][index])
			{
				multiplier++;
			}
			index++;
		}
		numsum = (num*multiplier);
		result += (num*multiplier);
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::array<std::vector<unsigned>,2> lists;
	
  parseInput(lists);

	resultA = compareLists(lists);
	resultB = similarLists(lists);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

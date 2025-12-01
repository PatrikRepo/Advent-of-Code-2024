#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

void parseInput(std::vector<std::array<unsigned,5>> &keys, std::vector<std::array<unsigned,5>> &locks)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			if(line == "#####")
			{
				std::array<unsigned,5> lock;
				std::string first = line;
				getline(input,line);
				std::string second = line;
				getline(input,line);
				std::string third = line;
				getline(input,line);
				std::string fourth = line;
				getline(input,line);
				std::string fifth = line;
				getline(input,line);
				std::string sixth = line;
				getline(input,line);
				std::string seventh = line;
				for(unsigned i=0; i<5; i++)
				{
					if(seventh[i] == '#') lock[i] = 6;
					else if (sixth[i] == '#') lock[i] = 5;
					else if(fifth[i] == '#') lock[i] = 4;
					else if(fourth[i] == '#') lock[i] = 3;
					else if(third[i] == '#') lock[i] = 2;
					else if(second[i] == '#') lock[i] = 1;
					else lock[i] = 0;
				}
				locks.push_back(lock);
			}
			else
			{
				std::array<unsigned,5> key;
				std::string first = line;
				getline(input,line);
				std::string second = line;
				getline(input,line);
				std::string third = line;
				getline(input,line);
				std::string fourth = line;
				getline(input,line);
				std::string fifth = line;
				getline(input,line);
				std::string sixth = line;
				getline(input,line);
				std::string seventh = line;
				for(unsigned i=0; i<5; i++)
				{
					if(first[i] == '#') key[i] = 6;
					else if (second[i] == '#') key[i] = 5;
					else if(third[i] == '#') key[i] = 4;
					else if(fourth[i] == '#') key[i] = 3;
					else if(fifth[i] == '#') key[i] = 2;
					else if(sixth[i] == '#') key[i] = 1;
					else key[i] = 0;
				}
				keys.push_back(key);
			}
			getline(input,line);
		}
	}
	input.close();

}

uint64_t matchKeys(std::vector<std::array<unsigned,5>> &keys, std::vector<std::array<unsigned,5>> &locks)
{
	uint64_t result = 0;
	
	for(auto &key:keys)
	{
		for(auto &lock:locks)
		{
			bool overlap = false;
			for(unsigned i=0; i<5; i++)
			{
				if(key[i] + lock[i] >= 6)
				{
					overlap = true;
					break;
				}
			}
			if(!overlap)
			{
				result++;
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t result = 0;

	std::vector<std::array<unsigned,5>> keys;
	std::vector<std::array<unsigned,5>> locks;

	parseInput(keys, locks);

	result = matchKeys(keys, locks);

	std::cout << "result: " << result << '\n';

	return 0;
}


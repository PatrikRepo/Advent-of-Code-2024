#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

void parseInput(std::unordered_map<std::string, std::vector<std::string>> &computers)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			std::string comp1 = line.substr(0,2);
			std::string comp2 = line.substr(3);
			computers[comp1].push_back(comp2);
			computers[comp2].push_back(comp1);
		}
	}
	input.close();

}

std::string generateKey(std::vector<std::string> &names)
{
	std::string result;
	
	std::sort(names.begin(), names.end());
	
	for(auto &name:names)
	{
		result += name;
	}
	
	return result;
}

std::pair<uint64_t,std::string> findSets(const std::unordered_map<std::string, std::vector<std::string>> &computers)
{
	std::pair<uint64_t,std::string> result;
	result.first = 0;
	
	std::unordered_set<std::string> sets;
	
	for(auto it=computers.begin(); it!=computers.end(); it++)
	{
		for(auto &comp1:it->second)
		{	
			for(auto &comp2:computers.at(comp1))
			{
				for(auto &comp3:computers.at(comp2))
				{
					if(comp3 == it->first)
					{
						std::vector<std::string> names;
						names.push_back(comp1);
						names.push_back(comp2);
						names.push_back(comp3);
						std::string key = generateKey(names);
						sets.emplace(key);
						break;
					}
				}
			}
		}
	}
	
	for(auto it=sets.begin(); it!=sets.end(); it++)
	{
		if((*it)[0] == 't' || (*it)[2] == 't' || (*it)[4] == 't')
		{
			result.first++;
		}
	}
	
	bool done = false;
	unsigned size = 6;
	while(!done)
	{
		done = true;
		for(auto it=sets.begin(); it!=sets.end(); it++)
		{
			if(it->size() != size) continue;
			std::vector<std::string> names;
			for(unsigned i=0; i<it->size(); i+=2)
			{
				names.push_back((*it).substr(i,2));
			}
			for(auto &comp:computers.at(names[0]))
			{
				bool clique = true;
				for(unsigned i=1; i<names.size(); i++)
				{
					clique = false;
					for(auto comp2:computers.at(names[i]))
					{
						if(comp2 == comp)
						{
							clique = true;
							break;
						}
					}
					if(!clique)
					{
						break;
					}
				}
				if(!clique) continue;
				
				auto newNames = names;
				newNames.push_back(comp);
				std::string key = generateKey(newNames);
				sets.emplace(key);
				done = false;
			}
		}
		if(!done)
		{
			size += 2;
		} 
	}
	
	for(auto it=sets.begin(); it!=sets.end(); it++)
	{
		if(it->size() == size)
		{
			std::vector<std::string> names;
			for(unsigned i=0; i<it->size(); i+=2)
			{
				names.push_back((*it).substr(i,2));
			}
			std::string key = generateKey(names);
			result.second = key.substr(0,2);
			for(unsigned i=2; i<key.size(); i+=2)
			{
				result.second += ',' + key.substr(i,2);
			}
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,std::string> result;

	std::unordered_map<std::string, std::vector<std::string>> computers;

	parseInput(computers);

	result = findSets(computers);

	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;

	return 0;
}


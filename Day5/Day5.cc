#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>

void parseInput(std::unordered_map<unsigned,std::unordered_set<unsigned>> &rules, std::vector<std::vector<unsigned>> &updates)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
  	getline(input,line);
  	while(line != "")
  	{
  		size_t pos = line.find_first_of("|");
  		unsigned first = std::stoi(line.substr(0,pos));
  		unsigned second = std::stoi(line.substr(pos+1));
  		
  		if(rules.count(first) == 0) 
  		{
  			rules[first] = std::unordered_set<unsigned>();
  		}
  		rules[first].insert(second);
  		if(rules.count(second) == 0)
  		{
  			rules[second] = std::unordered_set<unsigned>();
  		}
  		
  		getline(input,line);
  	}
  	
  	while(getline(input,line))
  	{
  		std::vector<unsigned> update;
  		size_t pos = 0;
  		while(pos < line.size())
  		{
  			size_t endpos = line.find_first_of(",",pos);
  			endpos = (endpos == std::string::npos) ? line.size() : endpos;
  			update.push_back(std::stoi(line.substr(pos,endpos-pos)));
  			pos = endpos+1;
  		}
  		updates.push_back(update);
  	}

  }
  input.close();
}

std::pair<uint64_t,uint64_t> printUpdates(const std::unordered_map<unsigned,std::unordered_set<unsigned>> &rules, const std::vector<std::vector<unsigned>> &updates)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	for(unsigned k=0; k<updates.size(); k++)
	{
		std::vector<unsigned> sorted = updates[k];
		std::sort(sorted.begin(),sorted.end(),[&rules](const unsigned leftPage, const unsigned rightPage)
		{
			return rules.at(leftPage).count(rightPage) == 1;
		});
		
		if(updates[k] == sorted)
		{
			result.first += updates[k][updates[k].size()/2];
		}
		else
		{
			result.second += sorted[sorted.size()/2];
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::unordered_map<unsigned,std::unordered_set<unsigned>> rules;
	std::vector<std::vector<unsigned>> updates;
	
  parseInput(rules, updates);

	result = printUpdates(rules, updates);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

void parseInput(std::vector<std::string> &patterns, std::vector<std::string> &designs)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		getline(input,line);
		
		size_t pos = 0;
		while(pos < line.size())
		{
			size_t endpos = line.find_first_of(",", pos);
			endpos = (endpos == std::string::npos) ? line.size() : endpos;
			patterns.push_back(line.substr(pos,endpos-pos));
			pos = endpos+2;
		}
		
		getline(input,line);
		while(getline(input,line))
		{
			designs.push_back(line);
		}
	}
	input.close();
}

uint64_t designPossible(const std::string &design, const std::vector<std::string> &patterns, unsigned index, std::unordered_map<std::string, uint64_t> &cache)
{
	uint64_t result = false;
	
	for(auto &pattern:patterns)
	{
		if(design.size() >= index + pattern.size() && design.substr(index, pattern.size()) == pattern)
		{
			if(index + pattern.size() == design.size())
			{
				result += 1;
			}
			else
			{
				if(cache.count(design.substr(index+pattern.size())) == 0)
				{
					cache[design.substr(index+pattern.size())] = designPossible(design, patterns, index+pattern.size(), cache);
				}
				if(cache[design.substr(index+pattern.size())])
				{
					result += cache[design.substr(index+pattern.size())];
				}
			}
		}
	}	
	
	return result;
}	

std::pair<uint64_t,uint64_t> createDesigns(const std::vector<std::string> &patterns, const std::vector<std::string> &designs)
{
	std::pair<uint64_t,uint64_t> result;
	
	std::unordered_map<std::string, uint64_t> cache;
	for(auto &design:designs)
	{
		uint64_t newResult = designPossible(design, patterns, 0, cache);
		if(newResult > 0)
		{
			result.first++;
			result.second += newResult;
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<std::string> patterns;
	std::vector<std::string> designs;

	parseInput(patterns, designs);

	result = createDesigns(patterns, designs);

	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;

	return 0;
}


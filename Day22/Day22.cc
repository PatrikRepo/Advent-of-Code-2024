#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <array>
#include <unordered_set>

void parseInput(std::vector<uint64_t> &initialSecrets)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			initialSecrets.push_back(std::stoll(line));
		}
	}
	input.close();

}

std::pair<uint64_t,uint64_t> getBananas(const std::vector<uint64_t> &initialSecrets)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	std::array<std::array<std::array<std::array<uint64_t,19>,19>,19>,19> bananaMap;
	for(auto &line1:bananaMap)
	{
		for(auto &line2:line1)
		{
			for(auto &line3:line2)
			{
				line3.fill(0);
			}
		}
	}
	
	for(auto secretNumber:initialSecrets)
	{
		std::unordered_set<int> seen;
		char fifth = secretNumber%10;
		
		secretNumber = ((secretNumber*64)^secretNumber)%16777216;
		secretNumber = ((secretNumber/32)^secretNumber)%16777216;
		secretNumber = ((secretNumber*2048)^secretNumber)%16777216;
		char fourth = secretNumber%10;
		
		secretNumber = ((secretNumber*64)^secretNumber)%16777216;
		secretNumber = ((secretNumber/32)^secretNumber)%16777216;
		secretNumber = ((secretNumber*2048)^secretNumber)%16777216;
		char third = secretNumber%10;
		
		secretNumber = ((secretNumber*64)^secretNumber)%16777216;
		secretNumber = ((secretNumber/32)^secretNumber)%16777216;
		secretNumber = ((secretNumber*2048)^secretNumber)%16777216;
		char second = secretNumber%10;
		
		for(unsigned i=3; i<2000; i++)
		{
			secretNumber = ((secretNumber*64)^secretNumber)%16777216;
			secretNumber = ((secretNumber/32)^secretNumber)%16777216;
			secretNumber = ((secretNumber*2048)^secretNumber)%16777216;
			char first = secretNumber%10;
			int key = (first - second) + ((second - third)*100) + ((third - fourth)*10000) + ((fourth - fifth)*1000000);
			if(seen.count(key) == 0)
			{
				bananaMap[fourth-fifth+9][third-fourth+9][second-third+9][first-second+9] += first;
				seen.emplace(key);
			}
			fifth = fourth;
			fourth = third;
			third = second;
			second = first;
		}
		
		result.first += secretNumber;
	}
	
	for(auto &line1:bananaMap)
	{
		for(auto &line2:line1)
		{
			for(auto &line3:line2)
			{
				for(auto num:line3)
				{
					result.second = (num > result.second) ? num : result.second;
				}
			}
		}
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<uint64_t> initialSecrets;

	parseInput(initialSecrets);

	result = getBananas(initialSecrets);

	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;

	return 0;
}


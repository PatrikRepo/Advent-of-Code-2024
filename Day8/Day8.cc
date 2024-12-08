#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

void parseInput(std::unordered_map<char,std::vector<std::pair<unsigned,unsigned>>> &antennas, std::pair<int,int> &mapSize)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
  	unsigned y = 0;
  	getline(input,line);
  	mapSize.first = line.size();
		do
		{
			size_t pos = line.find_first_not_of(".");
			while(pos < line.size())
			{
				antennas[line[pos]].emplace_back(pos,y);
				pos = line.find_first_not_of(".", pos+1);
			}
			y++;
		}while(getline(input,line));
		mapSize.second = y;
  }
  input.close();
}

std::pair<uint64_t,uint64_t> findAntinodes(const std::unordered_map<char,std::vector<std::pair<unsigned,unsigned>>> &antennas, const std::pair<int,int> &mapSize)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	std::unordered_set<uint64_t> antinodes;
	std::unordered_set<uint64_t> resonance;
	
	for(auto it=antennas.begin(); it!=antennas.end(); it++)
	{
		auto& freq = it->second;
		for(unsigned i=0; i<freq.size()-1; i++)
		{
			for(unsigned j=i+1; j<freq.size(); j++)
			{
				int antiX = (2*freq[i].first) - freq[j].first;
				int antiY = (2*freq[i].second) - freq[j].second;

				if(antiX >= 0 && antiX < mapSize.first && antiY >= 0 && antiY < mapSize.second)
				{
					antinodes.insert(antiX*(mapSize.first+1) + antiY);
				}
				
				antiX = (2*freq[j].first) - freq[i].first;
				antiY = (2*freq[j].second) - freq[i].second;

				if(antiX >= 0 && antiX < mapSize.first && antiY >= 0 && antiY < mapSize.second)
				{
					antinodes.insert(antiX*(std::max(mapSize.first,mapSize.second)+1) + antiY);
				}
				
				int distX = freq[j].first - freq[i].first;
				int distY = freq[j].second - freq[i].second;
				antiX = freq[i].first;
				antiY = freq[i].second;

				while(antiX >= 0 && antiX < mapSize.first && antiY >= 0 && antiY < mapSize.second)
				{
					resonance.insert(antiX*(mapSize.first+1) + antiY);
					antiX -= distX;
					antiY -= distY;
				}
				
				antiX = freq[i].first + distX;
				antiY = freq[i].second + distY;
				
				while(antiX >= 0 && antiX < mapSize.first && antiY >= 0 && antiY < mapSize.second)
				{
					resonance.insert(antiX*(std::max(mapSize.first,mapSize.second)+1) + antiY);
					antiX += distX;
					antiY += distY;
				}
			}
		}
	}
	
	result.first = antinodes.size();
	result.second = resonance.size();
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::unordered_map<char,std::vector<std::pair<unsigned,unsigned>>> antennas;
	std::pair<int,int> mapSize;
	
  parseInput(antennas, mapSize);

	result = findAntinodes(antennas, mapSize);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

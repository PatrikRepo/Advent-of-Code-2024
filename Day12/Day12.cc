#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

void parseInput(std::vector<std::string> &map)
{
std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {
    while(getline(input,line))
		{
			map.push_back("." + line + ".");
    }
  }
  input.close();
 
  map.insert(map.begin(), std::string(map[0].size(), '.'));
  map.push_back(std::string(map[0].size(), '.'));

}

std::pair<uint64_t,uint64_t> regionSize(const std::vector<std::string> &map, std::vector<std::string> &checkMap, unsigned y, unsigned x)
{
  std::pair<uint64_t,uint64_t> result = {0,0};
 
  uint64_t area = 0;
  uint64_t perimeter = 0;
  uint64_t sides = 0;
  char crop = map[y][x];
 
  std::vector<std::pair<unsigned,unsigned>> plots;
  plots.emplace_back(y,x);
 
  std::vector<std::pair<unsigned,unsigned>> corners;
 
  while(!plots.empty())
  {
    auto plot = plots.back();
    plots.pop_back();
    unsigned pY = plot.first;
    unsigned pX = plot.second;
    if(checkMap[plot.first][plot.second] != crop)
    {
        continue;
    }
    area++;
    checkMap[plot.first][plot.second] = crop+32;

    if(map[pY-1][pX] == crop)
    {
      plots.emplace_back(pY-1,pX);
    }  
    else if (checkMap[pY-1][pX] != crop+32)
    {
      perimeter++;
    }
   
    if(map[pY][pX+1] == crop)
    {
      plots.emplace_back(pY,pX+1);
    }  
    else if (checkMap[pY][pX+1] != crop+32)
    {
      perimeter++;
    }
   

    if(map[pY+1][pX] == crop)
    {
      plots.emplace_back(pY+1,pX);
    }  
    else if (checkMap[pY+1][pX] != crop+32)
    {
      perimeter++;
    }

    if(map[pY][pX-1] == crop)
    {
      plots.emplace_back(pY,pX-1);
    }  
    else if (checkMap[pY][pX-1] != crop+32)
    {
      perimeter++;
    }
    
    int arr[2] = {-1,1};		
    
    for(unsigned i=0; i<2; i++)
    {
    	for(unsigned j=0; j<2; j++)
    	{
				if(map[pY+arr[i]][pX+arr[j]] != crop && map[pY+arr[i]][pX] == crop && map[pY][pX+arr[j]] == crop)
				{
				  sides++;
				}
				else if(map[pY+arr[i]][pX] != crop && map[pY][pX+arr[j]] != crop)
				{
				  sides++;
				}
    	}
    }
  }
 
  result.first = area*perimeter;
  result.second = area*sides;
 
  return result;
}

std::pair<uint64_t,uint64_t> fencingCost(std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result = {0,0};

	auto checkMap = map;

	for(unsigned i=1; i<map.size()-1; i++)
	{
		for(unsigned j=1; j<map[0].size()-1; j++)
		{
			if(checkMap[i][j] <= 'Z')
			{
				auto newResult = regionSize(map, checkMap, i, j);
				result.first += newResult.first;
				result.second += newResult.second;
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

	result = fencingCost(map);

  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

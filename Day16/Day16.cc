#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>

struct Reindeer
{
	unsigned x = 0;
	unsigned y = 0;
	unsigned cost = 0;
	unsigned direction = 0;
	std::vector<std::vector<std::pair<unsigned,unsigned>>> corners;
};

void parseInput(std::vector<std::string> &map)
{
	std::string line;
  std::ifstream input("bigboy.txt");

  if(input.is_open())
  {
		while(getline(input,line))
		{
			map.push_back(line);
		}
  }
  input.close();
}

void trimMap(std::vector<std::string> &map)
{
	for(unsigned i=1; i<map.size()-1; i++)
	{
		for(unsigned j=1; j<map[0].size()-1; j++)
		{
			if(map[i][j] == '.')
			{
				unsigned blocks = 0;
				if(map[i-1][j] == '#') blocks++;
				if(map[i][j+1] == '#') blocks++;
				if(map[i+1][j] == '#') blocks++;
				if(map[i][j-1] == '#') blocks++;
				
				if(blocks == 4)
				{
					map[i][j] = '#';
				}
				else if(blocks == 3)
				{
					unsigned x = j;
					unsigned y = i;
					while(blocks == 3)
					{
						map[y][x] = '#';
						if(map[y-1][x] != '#') y--;
						else if(map[y][x+1] != '#') x++;
						else if(map[y+1][x] != '#') y++;
						else if(map[y][x-1] != '#') x--;
						if(map[y][x] != '.')
						{
							break;
						}
						blocks = 0;
						if(map[y-1][x] == '#') blocks++;
						if(map[y][x+1] == '#') blocks++;
						if(map[y+1][x] == '#') blocks++;
						if(map[y][x-1] == '#') blocks++;
					}
				}
			}
		}
	}
}

std::pair<uint64_t,uint64_t> traverseMap(std::vector<std::string> &map)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	unsigned startX = 1;
	unsigned startY = map.size()-2;
	unsigned goalX = map[0].size()-2;
	unsigned goalY = 1;

	std::vector<Reindeer> openList;
	std::unordered_set<unsigned> closedList;

	Reindeer reindeer;
	reindeer.x = startX;
	reindeer.y = startY;
	reindeer.direction = 1;
	reindeer.corners.emplace_back(std::vector<std::pair<unsigned,unsigned>>());
	reindeer.corners[0].emplace_back(reindeer.x,reindeer.y);
	openList.push_back(reindeer);
	
	reindeer.direction = 0;
	reindeer.cost = 1000;
	openList.push_back(reindeer);
	
	Reindeer winningDeer;
	
	while(!openList.empty())
	{
		reindeer = openList.back();
		openList.pop_back();

		if(reindeer.x == goalX && reindeer.y == goalY)
		{
			result.first = reindeer.cost;
			winningDeer = reindeer;
			break;
		}
		closedList.emplace((reindeer.x*10000) + reindeer.y);
		
		bool forward = true;
		while(forward)
		{
			forward = false;
			switch(reindeer.direction)
			{
				case 0: if(map[reindeer.y-1][reindeer.x] != '#') forward = true; reindeer.y -= 1; break;
				case 1: if(map[reindeer.y][reindeer.x+1] != '#') forward = true; reindeer.x += 1; break;
				case 2: if(map[reindeer.y+1][reindeer.x] != '#') forward = true; reindeer.y += 1; break;
				case 3: if(map[reindeer.y][reindeer.x-1] != '#') forward = true; reindeer.x -= 1; break;
			}
			if(!forward)
			{
				break;
			}
			reindeer.cost += 1;
			
			if(reindeer.x == goalX && reindeer.y == goalY)
			{
				reindeer.corners[0].emplace_back(reindeer.x,reindeer.y);
				openList.push_back(reindeer);
			}
			
			std::vector<Reindeer> newDeers;
			bool turnLeft = false;
			switch(reindeer.direction)
			{
				case 0: if(map[reindeer.y][reindeer.x-1] != '#') turnLeft = true; break;
				case 1: if(map[reindeer.y-1][reindeer.x] != '#') turnLeft = true; break;
				case 2: if(map[reindeer.y][reindeer.x+1] != '#') turnLeft = true; break;
				case 3: if(map[reindeer.y+1][reindeer.x] != '#') turnLeft = true; break;
			}
			if(turnLeft)
			{
				Reindeer newDeer = reindeer;
				newDeer.cost += 1000;
				newDeer.direction = (newDeer.direction + 3)%4;
				newDeer.corners[0].emplace_back(newDeer.x,newDeer.y);
				newDeers.push_back(newDeer);
			}
			
			bool turnRight = false;
			switch(reindeer.direction)
			{
				case 0: if(map[reindeer.y][reindeer.x+1] != '#') turnRight = true; break;
				case 1: if(map[reindeer.y+1][reindeer.x] != '#') turnRight = true; break;
				case 2: if(map[reindeer.y][reindeer.x-1] != '#') turnRight = true; break;
				case 3: if(map[reindeer.y-1][reindeer.x] != '#') turnRight = true; break;
			}
			if(turnRight)
			{
				Reindeer newDeer = reindeer;
				newDeer.cost += 1000;
				newDeer.direction = (newDeer.direction + 1)%4;
				newDeer.corners[0].emplace_back(newDeer.x,newDeer.y);
				newDeers.push_back(newDeer);
			}
			
			for(auto &newDeer:newDeers)
			{
				auto it = std::find_if(openList.begin(), openList.end(), [&newDeer](const Reindeer &oDeer){ return newDeer.x == oDeer.x && newDeer.y == oDeer.y && newDeer.direction == oDeer.direction;});
				if(it != std::end(openList))
				{
					if(it->cost > newDeer.cost) 
					{
						it->cost = newDeer.cost;
						it->corners = newDeer.corners;
					}
					else if(it->cost == newDeer.cost)
					{
						for(auto &cornerList:newDeer.corners)
						{
							if(std::find_if(it->corners.begin(), it->corners.end(), [&cornerList](const std::vector<std::pair<unsigned,unsigned>> &cornerL){ return cornerList == cornerL; }) == std::end(it->corners))
							{
								it->corners.push_back(cornerList);
							}
						}
					}
				}
				else
				{
					if(closedList.count((newDeer.x*10000) + newDeer.y) == 0)
					{
						openList.insert(openList.begin(), newDeer);
					}
				}
			}
		}
		std::sort(openList.begin(), openList.end(), [](const Reindeer &aDeer, const Reindeer &bDeer){ return aDeer.cost > bDeer.cost; });
	}
	
	for(auto &corners:winningDeer.corners)
	{
		for(unsigned i=0; i<corners.size()-1; i++)
		{
			auto from = std::min(corners[i].first, corners[i+1].first);
			auto to = std::max(corners[i].first, corners[i+1].first);
			for(unsigned j=from; j<=to; j++)
			{
				map[corners[i].second][j] = 'O';
			}
			
			from = std::min(corners[i].second, corners[i+1].second);
			to = std::max(corners[i].second, corners[i+1].second);
			for(unsigned j=from; j<=to; j++)
			{
				map[j][corners[i].first] = 'O';
			}
		}
	}
	
	for(auto &line:map)
	{
		for(auto c:line)
		{
			if(c == 'O')
			{
				result.second++;
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

	trimMap(map);
	
	result = traverseMap(map);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

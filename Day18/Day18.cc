#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <unordered_set>
#include <algorithm>

struct Node
{
	unsigned x = 0;
	unsigned y = 0;
	uint64_t cost = 0;
};

void parseInput(std::vector<std::pair<unsigned,unsigned>> &blocks)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			unsigned pos = line.find_first_of(",");
			blocks.emplace_back(std::stoi(line.substr(0,pos)), std::stoi(line.substr(pos+1)));
		}
	}
	input.close();
}

uint64_t findPath(const std::array<std::array<char,71>,71> &map)
{
	uint64_t result = 0;

	Node node;
	unsigned goalX = 70;
	unsigned goalY = 70;
	std::vector<Node> openList;
	std::unordered_set<uint64_t> closedList;
	openList.push_back(node);

	while(!openList.empty())
	{
		node = openList.back();
		openList.pop_back();
		if(node.x == goalX && node.y == goalY)
		{
			result = node.cost;
			break;
		}
		closedList.emplace((node.x*1000) + node.y);

		std::vector<Node> newNodes;
		if(node.y > 0 && map[node.y-1][node.x] != '#')
		{
			Node newNode = node;
			newNode.y -= 1;
			newNode.cost += 1;
			newNodes.push_back(newNode);
		}
		if(node.x < 70 && map[node.y][node.x+1] != '#')
		{
			Node newNode = node;
			newNode.x += 1;
			newNode.cost += 1;
			newNodes.push_back(newNode);
		}
		if(node.y < 70 && map[node.y+1][node.x] != '#')
		{
			Node newNode = node;
			newNode.y += 1;
			newNode.cost += 1;
			newNodes.push_back(newNode);
		}
		if(node.x > 0 && map[node.y][node.x-1] != '#')
		{
			Node newNode = node;
			newNode.x -= 1;
			newNode.cost += 1;
			newNodes.push_back(newNode);
		}
		for(auto &newNode:newNodes)
		{
			auto it = std::find_if(openList.begin(), openList.end(), [&newNode](const Node &oNode) {return newNode.x == oNode.x && newNode.y == oNode.y;});
			if(it != std::end(openList))
			{
				it->cost = (newNode.cost < it->cost) ? newNode.cost : it->cost;
			}
			else if(closedList.count((newNode.x*1000) + newNode.y) == 0)
			{
				openList.insert(openList.begin(), newNode);
			}
		}
		std::sort(openList.begin(), openList.end(), [](const Node &aNode, const Node &bNode){return aNode.cost > bNode.cost;});
	}

	return result;
}

uint64_t kByte(const std::vector<std::pair<unsigned,unsigned>> &blocks)
{
	uint64_t result = 0;

	std::array<std::array<char,71>,71> map;

	for(unsigned i=0; i<1024; i++)
	{
		map[blocks[i].second][blocks[i].first] = '#';
	}

	result = findPath(map);

	return result;
}

std::string findBlock(const std::vector<std::pair<unsigned,unsigned>> &blocks)
{
	std::string result;

	unsigned offset = 0;
	unsigned size = blocks.size()/2;

	while(size > 0)
	{
		std::array<std::array<char,71>,71> map;
		for(auto &line:map)
		{
			line.fill('.');
		}
		for(unsigned i=0; i<=offset+size; i++)
		{
			map[blocks[i].second][blocks[i].first] = '#';
		}

		if(findPath(map) > 0)
		{
			offset += size;
		}
		size /= 2;
	}

	std::array<std::array<char,71>,71> map;
	for(auto &line:map)
	{
		line.fill('.');
	}
	for(unsigned i=0; i<=offset; i++)
	{
		map[blocks[i].second][blocks[i].first] = '#';
	}
	if(findPath(map) > 0) offset++;

	result = std::to_string(blocks[offset].first) + "," + std::to_string(blocks[offset].second);

	return result;
}

int main()
{
	uint64_t resultA = 0;
	std::string resultB;

	std::vector<std::pair<unsigned,unsigned>> blocks;

	parseInput(blocks);

	resultA = kByte(blocks);
	resultB = findBlock(blocks);

	std::cout << "resultA: " << resultA << '\n';
	std::cout << "resultB: " << resultB << std::endl;

	return 0;
}


#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void parseInput(std::vector<std::string> &map, std::string &instructions)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {
  	getline(input,line);
		while(line != "")
		{
			map.push_back(line);
			getline(input,line);
		}
		
		while(getline(input,line))
		{
			instructions += line;
		}
  }
  input.close();
}

uint64_t findGoods(std::vector<std::string> map, const std::string &instructions)
{
	uint64_t result = 0;
	
	size_t x = 0;
	size_t y = 0;
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '@')
			{
				x = j;
				y = i;
				map[i][j] = '.';
				goto foundRobot;
			}
		}
	}

foundRobot:
	
	for(auto instruction:instructions)
	{
		switch(instruction)
		{
			case '^':
			{
				size_t toPos = y-1;
				while(map[toPos][x] == 'O')
				{
					toPos--;
				}
				if(map[toPos][x] == '.')
				{
					map[toPos][x] = map[y-1][x];
					map[y-1][x] = '.';
					y -= 1;
				}
				break;
			}
			case '>':
			{
				size_t toPos = x+1;
				while(map[y][toPos] == 'O')
				{
					toPos++;
				}
				if(map[y][toPos] == '.')
				{
					map[y][toPos] = map[y][x+1];
					map[y][x+1] = '.';
					x += 1;
				}
				break;
			}
			case 'v':
			{
				size_t toPos = y+1;
				while(map[toPos][x] == 'O')
				{
					toPos++;
				}
				if(map[toPos][x] == '.')
				{
					map[toPos][x] = map[y+1][x];
					map[y+1][x] = '.';
					y += 1;
				}
				break;
			}
			case '<':
			{
				size_t toPos = x-1;
				while(map[y][toPos] == 'O')
				{
					toPos--;
				}
				if(map[y][toPos] == '.')
				{
					map[y][toPos] = map[y][x-1];
					map[y][x-1] = '.';
					x -= 1;
				}
				break;
			}
		}
	}
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == 'O')
			{
				result += (i*100) + j;
			}
		}
	}
	
	return result;
}

bool checkIfFree(const std::vector<std::string> &map, size_t x, size_t y, char direction)
{
	bool result = true;

	switch(direction)
	{
		case '^':
		{
			if(map[y-1][x] == '#')
			{
				result = false;
			}
			else if((map[y-1][x] == '[' || map[y-1][x] == ']'))
			{
				if(!checkIfFree(map, x, y-1, direction))
				{
					result = false;
				}
				else if(map[y-1][x] == '[')
				{
					if(!checkIfFree(map, x+1, y-1, direction))
					{
						result = false;
					}
				}
				else if(map[y-1][x] == ']') 
				{
					if(!checkIfFree(map, x-1, y-1, direction))
					{
						result = false;
					}
				}
			}
			break;
		}
		case '>':
		{
			if(map[y][x+1] == '#')
			{
				result = false;
			}
			else if(map[y][x+1] == ']' || map[y][x+1] == '[')
			{
				if(!checkIfFree(map, x+1, y, direction))
				{
					result = false;
				}
			}
			break;
		}
		case 'v':
		{
			if(map[y+1][x] == '#')
			{
				result = false;
			}
			else if((map[y+1][x] == '[' || map[y+1][x] == ']'))
			{
				if(!checkIfFree(map, x, y+1, direction))
				{
					result = false;
				}
				else if(map[y+1][x] == '[')
				{
					if(!checkIfFree(map, x+1, y+1, direction))
					{
						result = false;
					}
				}
				else if(map[y+1][x] == ']') 
				{
					if(!checkIfFree(map, x-1, y+1, direction))
					{
						result = false;
					}
				}
			}
			break;
		}
		case '<':
		{
			if(map[y][x-1] == '#')
			{
				result = false;
			}
			else if(map[y][x-1] == ']' || map[y][x-1] == '[')
			{
				if(!checkIfFree(map, x-1, y, direction))
				{
					result = false;
				}
			}
			break;
		}
	}
	
	return result;
}

void push(std::vector<std::string> &map, size_t x, size_t y, char direction)
{
	switch(direction)
	{
		case '^':
		{
			if(map[y-1][x] != '.')
			{
				if(map[y-1][x] == '[')
				{
					push(map, x+1, y-1, direction);
				}
				else if(map[y-1][x] == ']')
				{
					push(map, x-1, y-1, direction);
				}
				push(map, x, y-1, direction);
			}
			map[y-1][x] = map[y][x];
			map[y][x] = '.';

			break;
		}
		case '>':
		{
			if(map[y][x+1] != '.')
			{
				push(map, x+1, y, direction);
			}
			map[y][x+1] = map[y][x];
			map[y][x] = '.';
			break;
		}
		case 'v':
		{
			if(map[y+1][x] != '.')
			{
				if(map[y+1][x] == '[')
				{
					push(map, x+1, y+1, direction);
				}
				else if(map[y+1][x] == ']')
				{
					push(map, x-1, y+1, direction);
				}
				push(map, x, y+1, direction);
			}
			map[y+1][x] = map[y][x];
			map[y][x] = '.';
			break;
		}
		case '<':
		{
			if(map[y][x-1] != '.')
			{
				push(map, x-1, y, direction);
			}
			map[y][x-1] = map[y][x];
			map[y][x] = '.';
			break;
		}
	}
}

uint64_t wideMap(std::vector<std::string> &map, const std::string &instructions)
{
	uint64_t result = 0;
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j+=2)
		{
			if(map[i][j] == '#')
			{
				map[i].insert(map[i].begin()+j+1,'#');
			}
			else if(map[i][j] == 'O')
			{
				map[i][j] = '[';
				map[i].insert(map[i].begin()+j+1,']');
			}
			else
			{
				map[i].insert(map[i].begin()+j+1,'.');
			}
		}
	}
	
	size_t x = 0;
	size_t y = 0;
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '@')
			{
				x = j;
				y = i;
				map[i][j] = '.';
				goto foundRobot;
			}
		}
	}

foundRobot:
	
	for(auto instruction:instructions)
	{
		if(checkIfFree(map, x, y, instruction))
		{
			push(map, x, y, instruction);
			switch(instruction)
			{
				case '^': y--; break;
				case '>': x++; break;
				case 'v': y++; break;
				case '<': x--; break;
			}
		}
	}
	
	for(unsigned i=0; i<map.size(); i++)
	{
		for(unsigned j=0; j<map[0].size(); j++)
		{
			if(map[i][j] == '[')
			{
				result += (i*100) + j;
			}
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<std::string> map;
	std::string instructions;

  parseInput(map, instructions);

	resultA = findGoods(map, instructions);
	resultB = wideMap(map, instructions);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <array>

struct Robot
{
	int x = 0;
	int y = 0;
	int vX = 0;
	int vY = 0;
};

void parseInput(std::vector<Robot> &robots)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {
		while(getline(input,line))
		{
			Robot robot;
			size_t pos = line.find_first_of("1234567890");
			size_t endpos = line.find_first_of(",", pos);
			robot.x = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("1234567890", endpos);
			endpos = line.find_first_of(" ", pos);
			robot.y = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890", endpos);
			endpos = line.find_first_of(",", pos);
			robot.vX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("-1234567890", endpos);
			robot.vY = std::stoi(line.substr(pos));
			robots.push_back(robot);
		}
  }
  input.close();
}

uint64_t calculatePositions(const std::vector<Robot> &robots)
{
	uint64_t result = 0;
	
	const int width = 101;
	const int height = 103;
	
	uint64_t q1 = 0;
	uint64_t q2 = 0;
	uint64_t q3 = 0;
	uint64_t q4 = 0;
	
	for(auto &robot:robots)
	{
		int posX = robot.x;
		int posY = robot.y;
		for(unsigned i=0; i<100; i++)
		{
			posX += robot.vX;
			posY += robot.vY;
		}
		posX = (posX+(100*width))%width;
		posY = (posY+(100*height))%height;
		
		if(posX > width/2)
		{
			if(posY < height/2)
			{
				q1++;
			}
			else if(posY > height/2)
			{
				q2++;
			}
		}
		else if(posX < width/2)
		{
			if(posY < height/2)
			{
				q3++;
			}
			else if(posY > height/2)
			{
				q4++;
			}
		}
	}
	
	result = ((q1*q2)*q3)*q4;
	
	return result;
}

uint64_t findTree(std::vector<Robot> &robots)
{
	uint64_t result = 0;
	
	const int width = 101;
	const int height = 103;
	
	for(unsigned i=0; i<100000; i++)
	{
		bool tree = true;
		std::array<std::array<char,width>,height> grid;
		for(auto &line:grid)
		{
			line.fill('.');
		}
		
		for(auto &robot:robots)
		{
			
			robot.x = (robot.x + robot.vX + width)%width;
			robot.y = (robot.y + robot.vY + height)%height;
			
			if(grid[robot.y][robot.x] == '#')
			{
				tree = false;
			}
			grid[robot.y][robot.x] = '#';
		}
		if(tree)
		{
			result = i+1;
			break;
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<Robot> robots;

  parseInput(robots);

	resultA = calculatePositions(robots);
	resultB = findTree(robots);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

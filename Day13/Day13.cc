#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <numeric>

struct Machine
{
	int aX = 0;
	int aY = 0;
	int bX = 0;
	int bY = 0;
	int64_t goalX = 0;
	int64_t goalY = 0;
};

void parseInput(std::vector<Machine> &machines)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {
		do
		{
			Machine machine;

			getline(input,line);
			size_t pos = line.find_first_of("1234567890");
			size_t endpos = line.find_first_of(",",pos);
			machine.aX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("1234567890", endpos);
			machine.aY = std::stoi(line.substr(pos));
			
			getline(input,line);
			pos = line.find_first_of("1234567890");
			endpos = line.find_first_of(",",pos);
			machine.bX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("1234567890", endpos);
			machine.bY = std::stoi(line.substr(pos));
			
			getline(input,line);
			pos = line.find_first_of("1234567890");
			endpos = line.find_first_of(",",pos);
			machine.goalX = std::stoi(line.substr(pos,endpos-pos));
			
			pos = line.find_first_of("1234567890", endpos);
			machine.goalY = std::stoi(line.substr(pos));
			
			machines.push_back(machine);
    }while(getline(input,line));
  }
  input.close();
}

uint64_t calculateTokens(const Machine &machine)
{
	uint64_t result = 0;
	
	int64_t det = (machine.aX*machine.bY) - (machine.bX*machine.aY);
	bool flip = false;
	
	if(det == 0)
	{
		return 0;
	}
	if(det < 0)
	{
		flip = true;
		det = abs(det);
	}
	
	int64_t numeratorA = (machine.goalX*machine.bY) - (machine.bX*machine.goalY);
	
	if(flip)
	{
		numeratorA = -numeratorA;
	}

	if(numeratorA < 0)
	{
		return 0;
	}
	if(std::lcm(numeratorA,det) != numeratorA)
	{
		return 0;
	}
	result = 3*(numeratorA/det);
	
	int64_t numeratorB = (machine.aX*machine.goalY) - (machine.goalX*machine.aY);
	
	if(flip)
	{
		numeratorB = -numeratorB;
	}
	if(numeratorB < 0)
	{
		return 0;
	}
	if(std::lcm(numeratorB,det) != numeratorB)
	{
		return 0;
	}
	result += numeratorB/det;
	
	return result;
}

std::pair<uint64_t,uint64_t> getPrizes(std::vector<Machine> &machines)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	for(auto &machine:machines)
	{
		result.first += calculateTokens(machine);
		machine.goalX += 10000000000000;
		machine.goalY += 10000000000000;
		result.second += calculateTokens(machine);
	}

	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<Machine> machines;

  parseInput(machines);

	result = getPrizes(machines);

  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <stack>

void parseInput(std::array<uint64_t,3> &registers, std::vector<unsigned> &instructions)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		getline(input,line);
		registers[0] = std::stoll(line.substr(line.find_first_of("1234567890")));
		getline(input,line);
		registers[1] = std::stoll(line.substr(line.find_first_of("1234567890")));
		getline(input,line);
		registers[2] = std::stoll(line.substr(line.find_first_of("1234567890")));

		getline(input,line);
		getline(input,line);
		size_t pos = line.find_first_of("1234567890");
		while(pos < line.size())
		{
			instructions.push_back(line[pos] - '0');
			pos += 2;
		}
  }
  input.close();
}

unsigned combo(unsigned operand, const std::array<uint64_t,3> &registers)
{
	unsigned result = 0;

	if(operand <= 3)
	{
		result = operand;
	}
	else
	{
		result = registers[operand-4];
	}

	return result;
}

std::string executeProgram(std::array<uint64_t,3> &registers, const std::vector<unsigned> &instructions)
{
	std::string result;

	for(int i=0; i<(int)instructions.size(); i+=2)
	{
		switch(instructions[i])
		{
			case 0: registers[0] = (registers[0]/(std::pow(2,combo(instructions[i+1],registers)))); break;
			case 1: registers[1] ^= instructions[i+1]; break;
			case 2: registers[1] = combo(instructions[i+1], registers) % 8; break;
			case 3: i = (registers[0] != 0) ? instructions[i+1] - 2 : i; break;
			case 4: registers[1] ^= registers[2]; break;
			case 5: if(result != "") result += ','; result += std::to_string(combo(instructions[i+1], registers) % 8); break;
			case 6: registers[1] = (registers[0]/(std::pow(2,combo(instructions[i+1],registers)))); break;
			case 7: registers[2] = (registers[0]/(std::pow(2,combo(instructions[i+1],registers)))); break;
		}
	}

	return result;
}

uint64_t twoPow(unsigned j)
{
	uint64_t result = 1;

	for(unsigned i=0; i<j; i++)
	{  
		result *= 2;
	}

	return result;
}

uint64_t findA(const std::vector<unsigned> &instructions)
{
	uint64_t result = 0;

	unsigned bitxor = 0;
	for(unsigned i=instructions.size()-2; i+1>0; i-=2)
	{
		if(instructions[i] == 1)
		{
			bitxor = instructions[i+1];
			break;
		}
	}

	unsigned offset = 0;
	std::stack<unsigned> addedNumbers;
	std::array<uint64_t,3> registers = {0,0,0};

	for(unsigned i=instructions.size()-1; i+1>0;i--)
	{
		bool incorrectGuess = true;
		for(uint64_t j=offset; j<1000; j++)
		{
			uint64_t b = j%8;
			b ^= 1;
			uint64_t c = ((result*8) +j)/(twoPow(b));
			b ^= c;
			b ^= bitxor;

			if(b % 8 == instructions[i])
			{
				offset = 0;
				incorrectGuess = false;
				result *= 8;
				result += j;  
				registers = {result,0,0};
				std::string tempResult = executeProgram(registers, instructions);

				for(unsigned k=0; k<tempResult.size(); k+=2)
				{
					if((unsigned) tempResult[k] - '0' != instructions[i+(k/2)])
					{
						incorrectGuess = true;
						break;
					}
				}
				if(incorrectGuess)
				{
					result -= j;  
					result /= 8;
					break;
				}
				addedNumbers.push(j);
				break;
			}
		}
		
		if(incorrectGuess)
		{
			result -= addedNumbers.top();
			result /= 8;
			offset = addedNumbers.top()+1;
			addedNumbers.pop();
			i+=2;
		}
	}

	return result;
}

int main()
{
	std::string resultA;
	uint64_t resultB;

	std::array<uint64_t,3> registers;
	std::vector<unsigned> instructions;

	parseInput(registers, instructions);

	resultA = executeProgram(registers, instructions);
	resultB = findA(instructions);
 
	std::cout << "resultA: " << resultA << '\n';
	std::cout << "resultB: " << resultB << std::endl;

	return 0;
}

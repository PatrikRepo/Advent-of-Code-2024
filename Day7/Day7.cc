#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<std::vector<uint64_t>> &equations)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
		while(getline(input,line))
		{
			std::vector<uint64_t> equation;
			
			size_t pos = line.find_first_of(":");
			equation.push_back(std::stoll(line.substr(0,pos)));
			pos += 2;
			
			while(pos < line.size())
			{
				size_t endpos = line.find_first_of(" ",pos);
				endpos = (endpos == std::string::npos) ? line.size() : endpos;
				equation.push_back(std::stoll(line.substr(pos,endpos-pos)));
				pos = endpos+1;
			}
			equations.push_back(equation);
		}
  }
  input.close();
}

bool solveEquation(uint64_t sum, const std::vector<uint64_t> &equation, unsigned index, uint64_t partialSum)
{
	bool result = false;

	if(index+1<equation.size())
	{
		if(partialSum*equation[index] <= sum && solveEquation(sum, equation, index+1, partialSum*equation[index]))
		{
			result = true;
		}
		if(!result && partialSum+equation[index] <= sum && solveEquation(sum, equation, index+1, partialSum+equation[index]))
		{
			result = true;
		}
	}
	else
	{
		if((sum == partialSum*equation[index]) || (sum == partialSum+equation[index]))
		{
			result = true;
		}
	}
	
	return result;
}

uint64_t solveEquations(const std::vector<std::vector<uint64_t>> &equations)
{
	uint64_t result = 0;
	
	for(auto &equation:equations)
	{
		uint64_t sum = equation[0];
		if(solveEquation(sum,equation,2,equation[1]))
		{
			result += sum;
		}
	}

	return result;
}

uint64_t concat(uint64_t num1, uint64_t num2)
{
	uint64_t result = 0;
	
	std::string s = std::to_string(num1);
	s += std::to_string(num2);
	result = std::stoll(s);
	
	return result;
}

bool solveEquationWithConcat(uint64_t sum, const std::vector<uint64_t> &equation, unsigned index, uint64_t partialSum)
{
	bool result = false;

	if(index+1<equation.size())
	{
		if(partialSum*equation[index] <= sum && solveEquationWithConcat(sum, equation, index+1, partialSum*equation[index]))
		{
			result = true;
		}
		if(!result && (partialSum+equation[index] <= sum) && solveEquationWithConcat(sum, equation, index+1, partialSum+equation[index]))
		{
			result = true;
		}
		uint64_t concatSum = concat(partialSum,equation[index]);
		if(!result && concatSum <= sum && solveEquationWithConcat(sum, equation, index+1, concatSum))
		{
			result = true;
		}
	}
	else
	{
		if((sum == partialSum*equation[index]) || (sum == partialSum+equation[index]) || (sum == concat(partialSum,equation[index])))
		{
			result = true;
		}
	}
	
	return result;
}

uint64_t withConcat(const std::vector<std::vector<uint64_t>> &equations)
{
	uint64_t result = 0;
	
	for(auto &equation:equations)
	{
		uint64_t sum = equation[0];
		if(solveEquationWithConcat(sum,equation,2,equation[1]))
		{
			result += sum;
		}
	}

	return result;
}

int main()
{
	uint64_t resultA;
	uint64_t resultB;

	std::vector<std::vector<uint64_t>> equations;
	
  parseInput(equations);

	resultA = solveEquations(equations);
	resultB = withConcat(equations);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

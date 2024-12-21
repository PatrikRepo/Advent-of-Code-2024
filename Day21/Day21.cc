#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <unordered_map>

void parseInput(std::vector<std::string> &codes)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		while(getline(input,line))
		{
			codes.push_back(line);
		}
	}
	input.close();

}

std::string findNumpadPresses(const std::string &code)
{
	std::string result;

	int current = 0;
	if(code[0] == '0' || code[0] % 3 == 2)
	{
		result += '<';
	}
	else if(code[0] <= '3')
	{
		result += '^';
		current = 3;
	}
	else if(code[0] <= '6')
	{
		result += "^^";
		current = 6;
	}
	else if(code[0] <= '9')
	{
		result += "^^^";
		current = 9;
	}
	
	for(unsigned i=0; i<3; i++)
	{
		while(current != code[i] - '0')
		{
			if(code[i] != '0')
			{
				if(current == 0)
				{
					result += '^';
					current = 2;
				}
				while((current%3 == 0 && (code[i]-'0')%3 != 0) || (current%3 == 2 && (code[i]-'0')%3 == 1))
				{
					result += '<';
					current -= 1;
				}
				while(current+3 <= (code[i]-'0'))
				{
					result += '^';
					current += 3;
				}
				while(current-1 >= (code[i]-'0'))
				{
					result += 'v';
					current -= 3;
				}
				while((current%3 == 1 && (code[i]-'0')%3 != 1) || (current%3 == 2 && (code[i]-'0')%3 == 0))
				{
					result += '>';
					current += 1;
				}
			}
			else
			{
				if(current%3 < 2)
				{
					result += '>';
					current++;
				}
				else if(current%3 > 2)
				{
					result += '<';
					current--;
				}
				while(current > 2)
				{
					result += 'v';
					current -= 3;
				}
				result += 'v';
				current = 0;
			}
		}
		result += 'A';
	}
	
	if(current == 0) result += ">A";
	else if(current == 1) result += ">>vA";
	else if(current == 2) result += "v>A";
	else if(current == 3) result += "vA";
	else if(current == 4) result += ">>vvA";
	else if(current == 5) result += "vv>A";
	else if(current == 6) result += "vvA";
	else if(current == 7) result += ">>vvvA";
	else if(current == 8) result += "vvv>A";
	else if(current == 9) result += "vvvA";

	return result;
}

std::string findKeypadPresses(const std::string &code)
{
	std::string result;
	
	char current = 'A';
	
	for(auto press:code)
	{
		if(current != press)
		{
			if(press == 'A')
			{
				if(current == '<') result += ">>^";
				else if(current == 'v') result += "^>";
				else if(current == '>') result += '^';
				else result += '>';
				current = 'A';
			}
			else if(press == '^')
			{
				if(current == 'A') result += '<';
				else if(current == '>') result += "<^";
				else if(current == '<') result += ">^";
				else result += '^';
				current = '^';
			}	
			else if(press == '>')
			{
				if(current == 'A') result += 'v';
				else if(current == '^') result += "v>";
				else if(current == '<') result += ">>";
				else result += '>';
				current = '>';
			}
			else if(press == 'v')
			{
				if(current == 'A') result += "<v";
				else if(current == '^') result += 'v';
				else if(current == '>') result += '<';
				else result += '>';
				current = 'v';
			}
			else if(press == '<')
			{
				if(current == 'A') result += "v<<";
				else if(current == '^') result += "v<";
				else if(current == '>') result += "<<";
				else result += '<';
				current = '<';
			}
		}
		result += 'A';
	}
	
	return result;
}

uint64_t recursiveFind(const std::string &code, unsigned iterations, std::unordered_map<std::string,uint64_t> &cache)
{
	uint64_t result = 0;
	
	size_t pos = 0;
	while(pos < code.size())
	{
		size_t endpos = code.find_first_of("A",pos)+1;
		
		std::string subString = code.substr(pos,endpos-pos);
		pos = endpos;
		
		if(iterations == 1)
		{
			result += findKeypadPresses(subString).size();
		}
		else
		{
			if(cache.count(subString + std::to_string(iterations)) == 0)
			{
				cache[subString + std::to_string(iterations)] = recursiveFind(findKeypadPresses(subString), iterations-1, cache);
			}
			result += cache[subString + std::to_string(iterations)];
		}
	}

	return result;
}

uint64_t findKeyPressSize(const std::string &code, unsigned iterations, std::unordered_map<std::string,uint64_t> &cache)
{
	uint64_t result = 0;
	
	std::string keypadPresses = findNumpadPresses(code);
	
	result += recursiveFind(keypadPresses, iterations, cache);
		
	return result;
}

std::pair<uint64_t,uint64_t> inputCodes(const std::vector<std::string> &codes)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	std::unordered_map<std::string,uint64_t> cache;
	for(auto &code:codes)
	{
		result.first += findKeyPressSize(code, 2, cache)*std::stoi(code.substr(0,3));
		result.second += findKeyPressSize(code, 25, cache)*std::stoi(code.substr(0,3));
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<std::string> codes;

	parseInput(codes);

	result = inputCodes(codes);

	std::cout << "resultA: " << result.first << '\n';
	std::cout << "resultB: " << result.second << std::endl;

	return 0;
}


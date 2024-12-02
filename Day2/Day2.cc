#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

void parseInput(std::vector<std::vector<int>> &reports)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		std::vector<int> report;
  		size_t pos = 0;

  		while(pos < line.size())
  		{
	  		size_t endpos = line.find_first_of(" ", pos);
	  		endpos = (endpos == std::string::npos) ? line.size() : endpos;
	  		report.push_back(std::stoi(line.substr(pos,endpos-pos)));
	  		pos = endpos+1;
  		}
  		reports.push_back(report);
  	}
  }
  input.close();
}

uint64_t checkSafety(const std::vector<std::vector<int>> &reports)
{
	uint64_t result = 0;
	
	for(const auto &report:reports)
	{
		bool safe = true;
		bool increasing = report[0] < report[1];
		for(unsigned i=0; i<report.size()-1; i++)
		{
			if(increasing)
			{
				if(!(report[i] < report[i+1] && report[i]+3 >= report[i+1]))
				{ 
					safe = false;
					break;
				}
			}
			else
			{
				if(!(report[i] > report[i+1] && ((report[i] > 3) ? report[i]-3 : 0)  <= report[i+1]))
				{ 
					safe = false;
					break;
				}
			}
		}
		if(safe)
		{
			result++;
		}
	}
	
	return result;
}

uint64_t oneFault(const std::vector<std::vector<int>> &reports)
{
	uint64_t result = 0;

	for(const auto &report:reports)
	{
		bool safe = true;
		bool increasing = ((report[0] < report[1]) == (report[1] < report[2])) ? (report[0] < report[1]) : (report[2] < report[3]);
		std::vector<unsigned> skips;
		for(unsigned i=0; i<report.size()-1; i++)
		{
			unsigned prev = i;
			unsigned next = i+1;
			if(increasing)
			{
				if(!(report[prev] < report[next] && report[prev]+3 >= report[next]))
				{ 
					safe = false;
					skips.push_back(i);
					skips.push_back(i+1);
				}
			}
			else
			{
				if(!(report[prev] > report[next] && ((report[prev] > 3) ? report[prev]-3 : 0)  <= report[next]))
				{ 
					safe = false;
					skips.push_back(i);
					skips.push_back(i+1);
				}
			}
		}
		if(safe)
		{
			result++;
			continue;
		}
		safe = true;
		for(auto skip:skips)
		{
			for(unsigned i=0; i<report.size()-1; i++)
			{
				if(i+1 == skip || (i==0 && skip==0))
				{
					continue;
				}
				unsigned prev = (skip == i) ? i-1 : i;
				unsigned next = i+1;
				if(increasing)
				{
					if(!(report[prev] < report[next] && report[prev]+3 >= report[next]))
					{ 
						safe = false;
						break;
					}
				}
				else
				{
					if(!(report[prev] > report[next] && ((report[prev] > 3) ? report[prev]-3 : 0)  <= report[next]))
					{ 
						safe = false;
						break;
					}
				}
			}
			if(safe)
			{
				result++;
				break;
			}
			safe = true;
		}
	}

	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::vector<std::vector<int>> reports;
	
  parseInput(reports);

	resultA = checkSafety(reports);
	resultB = oneFault(reports);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

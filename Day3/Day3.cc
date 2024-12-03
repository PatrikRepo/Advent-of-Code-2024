#include <iostream>
#include <fstream>
#include <utility>

void parseInput(std::string &data)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {	
  	while(getline(input,line))
  	{
  		data += line;
  		data += " ";
  	}
  }
  input.close();
}

std::pair<uint64_t,uint64_t> mul(const std::string &data)
{
	std::pair<uint64_t,uint64_t> result = {0,0};
	
	bool doMul = true;
	size_t pos = data.find_first_of("md");
	
	while(pos < data.size())
	{
		if(data[pos] == 'd')
		{
			if(data.substr(pos,4) == "do()")
			{	
				doMul = true;
			}	
			if(data.substr(pos,7) == "don't()")
			{
				doMul = false;
			}
		}
		else
		{
			size_t midpos = data.find_first_of(",",pos);
			size_t endpos = data.find_first_of(")",pos)+1;
			
			midpos = (midpos == std::string::npos) ? data.size() : midpos;
			endpos = (endpos == std::string::npos) ? data.size() : endpos;
			
			if(endpos-pos >= 8 && endpos-pos <= 12 && midpos > pos && midpos < endpos && data.substr(pos,4) == "mul(")
			{
				unsigned first = 0;
				unsigned second = 0;
				bool corrupt = false;
				for(unsigned i=pos+4; i<midpos; i++)
				{
					if(data[i] < '0' || data[i] > '9')
					{
						corrupt = true;
						break;
					}
				}
				if(!corrupt)
				{
					first = std::stoi(data.substr(pos+4,midpos-pos-4));
					for(unsigned i=midpos+1; i<endpos-1; i++)
					{
						if(data[i] < '0' || data[i] > '9')
						{
							corrupt = true;
							break;
						}
					}
					if(!corrupt)
					{
						second = std::stoi(data.substr(midpos+1,endpos-1-midpos-1));
						result.first += first*second;
						result.second += (doMul) ? first*second : 0;
					}
				}
			}
		}
		
		pos = data.find_first_of("md",pos+1);
	}
	
	return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::string data;
	
  parseInput(data);

	result = mul(data);
	
  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

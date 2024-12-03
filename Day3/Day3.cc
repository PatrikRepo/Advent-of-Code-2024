#include <iostream>
#include <fstream>

#include <chrono>

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

uint64_t mul(const std::string &data)
{
	uint64_t result = 0;
	
	size_t pos = data.find_first_of("m");
	
	while(pos < data.size())
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
					result += first*second;
				}
			}
		}
		pos = data.find_first_of("m",pos+1);
	}
	
	return result;
}

uint64_t doMul(const std::string &data)
{
	uint64_t result = 0;
	
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
		else if(doMul)
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
						result += first*second;
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
	uint64_t resultA = 0;
	uint64_t resultB = 0;

	std::string data;
	
  parseInput(data);

	resultA = mul(data);
	resultB = doMul(data);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

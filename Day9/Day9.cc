#include <iostream>
#include <fstream>
#include <vector>

void parseInput(std::vector<int64_t> &files)
{
	std::string line;
  std::ifstream input("bigboy.txt");

  if(input.is_open())
  {	
  	getline(input,line);
  	unsigned pos = 0;
  	unsigned id = 0;
  	while(pos < line.size())
  	{
 			if(pos % 2 == 0)
			{
				for(int i=0; i<line[pos] - '0'; i++)
				{
					files.push_back(id);
				}
				id++;
			}	
			else
			{
				for(int i=0; i<line[pos] - '0'; i++)
				{
					files.push_back(-1);
				}
			}
	  	pos++;
  	}
  }
  input.close();
}

uint64_t moveBlocks(std::vector<int64_t> files)
{
	uint64_t result = 0;
	
	unsigned start = 0;
	unsigned end = files.size()-1;
	
	while(start < end)
	{
		while(files[end] == -1)
		{
			end--;
		}
		while(files[start] != -1)
		{
			start++;
		}
		if(start >= end)
		{
			break;
		}
		files[start] = files[end];
		files[end] = -1;
		start++;
		end--;
	}
	
	for(unsigned i=0; files[i] != -1; i++)
	{
		result += files[i]*i;
	}
	
	return result;
}

uint64_t moveFiles(std::vector<int64_t> &files)
{
	uint64_t result = 0;
	
	int64_t id = INT64_MAX;
	uint64_t idPos = files.size()-1;
	uint64_t firstZero = 0;
	
	while(idPos > 0)
	{
		uint64_t pos = 0;
		unsigned size = 0;
		
		for(; idPos+1>0; idPos--)
		{
			if(files[idPos] != -1 && files[idPos] < id)
			{
				pos = idPos;
				id = files[idPos];
				break;
			}
		}
		for(uint64_t i=pos; i+1>0 && files[i] == id; i--)
		{
			size++;
		}
		idPos = (pos - size + 1 > 0) ? pos - size : 0;
		pos = (idPos > 0) ? idPos+1 : 0;
		
		uint64_t to = firstZero;
		bool newFirst = false;
		
		while(to < pos)
		{
			for(unsigned i=to; i<files.size(); i++)
			{
				if(files[i] == -1)
				{
					to = i;
					if(!newFirst)
					{
						newFirst = true;
						firstZero = to;
					}
					break;
				}
			}
			unsigned toSize = 0;
			for(uint64_t i=to; i<files.size(); i++)
			{
				if(files[i] != -1 && files[i])
				{
					break;
				}
				toSize++;
			}
			
			if(toSize >= size)
			{
				break;
			}
			to += toSize;
		}
		
		if(to < pos)
		{
			for(uint64_t i=0; i<size; i++)
			{
				files[to+i] = files[pos+i];
				files[pos+i] = -1;
			}
		}
	}
	
	for(uint64_t i=0; i<files.size(); i++)
	{
		if(files[i] != -1)
		{	
			result += i*files[i];
		}
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	uint64_t resultB = 0;
	
	std::vector<int64_t> files;
	
  parseInput(files);

	resultA = moveBlocks(files);
	resultB = moveFiles(files);
	
  std::cout << "resultA: " << resultA << '\n';
  std::cout << "resultB: " << resultB << std::endl;

  return 0;
}

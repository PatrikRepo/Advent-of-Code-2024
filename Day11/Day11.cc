#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <utility>

void parseInput(std::vector<uint64_t> &stones)
{
	std::string line;
  std::ifstream input("input.txt");

  if(input.is_open())
  {
		getline(input,line);
		size_t pos = 0;
		while(pos < line.size())
		{
			 size_t endpos = line.find_first_of(" ",pos);
			 endpos = (endpos == std::string::npos) ? line.size() : endpos;
			 stones.push_back(std::stoi(line.substr(pos,endpos-pos)));
			 pos = endpos+1;
		}
  }
  input.close();
}

uint64_t blink(uint64_t stone, unsigned blinksLeft, std::unordered_map<uint64_t,std::array<uint64_t,75>> &cache)
{
  uint64_t result = 0;
 
  if(blinksLeft == 0)
  {
    return 1;
  }
 
  bool split = false;
  uint64_t splitStone = 0;
 
  if(stone == 0)
  {
    stone = 1;
  }
  else
  {
    std::string s = std::to_string(stone);
    if(s.size()%2 == 0)
    {
      split = true;
      stone = std::stoll(s.substr(0,s.size()/2));
      splitStone = std::stoll(s.substr(s.size()/2));
    }
    else
    {
      stone *= 2024;
    }
  }
  blinksLeft--;

  if(blinksLeft > 0)
  {
    if(cache.count(stone) == 0)
    {
      std::array<uint64_t,75> arr{0};
      cache[stone] = arr;
    }
    if(cache[stone][blinksLeft] == 0)
    {
      cache[stone][blinksLeft] = blink(stone, blinksLeft, cache);
    }
    result += cache[stone][blinksLeft];
    if(split)
    {
      if(cache.count(splitStone) == 0)
      {
        std::array<uint64_t,75> arr{0};
        cache[splitStone] = arr;
      }
      if(cache[splitStone][blinksLeft] == 0)
      {
        cache[splitStone][blinksLeft] = blink(splitStone, blinksLeft, cache);
      }
     
      result += cache[splitStone][blinksLeft];
    }
  }
  else
  {
    result = (split) ? 2 : 1;
  }
 
  return result;
}

std::pair<uint64_t,uint64_t> blink(const std::vector<uint64_t> &stones)
{
  std::pair<uint64_t,uint64_t> result = {0,0};
 
  std::unordered_map<uint64_t,std::array<uint64_t,75>> cache;
  for(unsigned i=0; i<stones.size(); i++)
  {
    result.first += blink(stones[i], 25, cache);
  }
 
  for(unsigned i=0; i<stones.size(); i++)
  {
    result.second += blink(stones[i], 75, cache);
  }
 
  return result;
}

int main()
{
	std::pair<uint64_t,uint64_t> result;

	std::vector<uint64_t> stones;

  parseInput(stones);

	result = blink(stones);

  std::cout << "resultA: " << result.first << '\n';
  std::cout << "resultB: " << result.second << std::endl;

  return 0;
}

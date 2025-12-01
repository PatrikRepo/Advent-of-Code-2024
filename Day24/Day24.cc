#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <algorithm>

struct Wire
{
	int val = -1;
	int op = -1;
	int input = -1;
	std::vector<std::string> children;
};

void parseInput(std::vector<std::pair<std::string,int>> &values, std::unordered_map<std::string, Wire> &wires)
{
	std::string line;
	std::ifstream input("input.txt");

	if(input.is_open())
	{
		getline(input,line);
		while(line != "")
		{
			values.emplace_back(line.substr(0,3), line.back()-'0');
			getline(input,line);
		}
		
		while(getline(input,line))
		{
			std::string inWire1 = line.substr(0,3);
			unsigned pos = line.find_first_of(" ", 5);
			std::string op = line.substr(4,pos-4);
			std::string inWire2 = line.substr(pos+1,3);
			std::string outWire = line.substr(line.size()-3);
			
			wires[inWire1].children.push_back(outWire);
			wires[inWire2].children.push_back(outWire);
			wires[outWire].op = (op == "AND") ? 0 : (op == "OR") ? 1 : 2;
		}
	}
	
	input.close();
}

uint64_t findNumber(std::vector<std::pair<std::string,int>> values, std::unordered_map<std::string, Wire> wires)
{
	uint64_t result = 0;

	for(unsigned i=0; i<values.size(); i++)
	{
		Wire& parent = wires.at(values[i].first);
		parent.val = values[i].second;
		for(auto &name:parent.children)
		{
			Wire& child = wires.at(name);
			if(child.input == -1)
			{
				child.input = parent.val;
			}
			else
			{
				switch(child.op)
				{
					case 0: values.emplace_back(name, child.input & parent.val); break;
					case 1: values.emplace_back(name, child.input | parent.val); break;
					case 2: values.emplace_back(name, child.input ^ parent.val); break;
				}
				child.input = -1;
			}
		}
	}
	
	std::string binz;
	for(int i=45; i>=0; i--)
	{
		std::string name = "z";
		if(i>9)
		{
			name += std::to_string(i);
		}
		else
		{
			name += "0" + std::to_string(i);
		}
		binz += ('0' + wires.at(name).val);
	}
	
	std::bitset<46> bits{binz};
	result = bits.to_ulong();
	
	return result;
}

std::string findSwaps(std::unordered_map<std::string, Wire> &wires)
{
	std::string result;
	
	std::vector<std::string> swapped;
	
	for(unsigned i=2; i<45; i++)
	{
		std::string x = "x";
		std::string y = "y";
		std::string z = "z";
		std::string num;
		if(i < 10) num = "0" + std::to_string(i);
		else num = std::to_string(i);
		x += num;
		y += num;
		z += num;
		std::string xyxor;
		std::string xyand;
		for(auto &wire:wires[x].children)
		{
			if(wires[wire].op == 2) xyxor = wire;
			if(wires[wire].op == 0) xyand = wire;
		}
		bool found = false;
		std::vector<std::string> xyxorC;
		for(auto &wire:wires[xyxor].children)
		{
			xyxorC.push_back(wire);
			if(wire == z && wires[wire].op == 2) found = true;
		}
		if(!found)
		{
			if(wires[z].op != 2)
			{
				swapped.push_back(z);
				for(auto &wire:xyxorC)
				{
					if(wires[wire].op == 2) swapped.push_back(wire);
				}	
			}
			else
			{
				for(auto &wire:wires[xyand].children)
				{
					if(wire == z && wires[wire].op == 2) 
					{
						swapped.push_back(xyand);
						swapped.push_back(xyxor);
					}
				}
			}
		}
	}
	
	std::sort(swapped.begin(), swapped.end());
	for(unsigned i=0; i<swapped.size(); i++)
	{
		result += swapped[i];
		if(i+1 < swapped.size()) result += ",";
	}
	
	return result;
}

int main()
{
	uint64_t resultA = 0;
	std::string resultB;

	std::vector<std::pair<std::string,int>> values;
	std::unordered_map<std::string, Wire> wires;

	parseInput(values, wires);

	resultA = findNumber(values, wires);
	resultB = findSwaps(wires);
	
	std::cout << "resultA: " << resultA << '\n';
	std::cout << "resultB: " << resultB << std::endl;

	return 0;
}


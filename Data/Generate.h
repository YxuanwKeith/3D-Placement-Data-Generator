#pragma once

#include "Global/Global.h"
#include "Common/Math.h"
#include "Common/Random.h"
#include "Common/StringUtil.h"

namespace WorkSpace {
namespace Placement {

class InstanceGenerator {
private:
	// path
	Str outputPath = "Instance";
	Str outputName = "instance";

	// output info
	Num instanceCount, netCount, technologyCount, cellCount;
	Insts insts;
	Area die;
	Arr<DieInfo, Die::SizeDie> dies;
	Terminal terminal;
	//Vec<Str> instName, cellName;
	Nets nets;
	Techs techs;

	// config
	ScaleRand numCells;
	ScaleRand numInstance;
	ScaleRand numNetPins;
	ScaleRand numCellPins;
	ScaleRand blockCellSize;
	ScaleRand blockCrossPinSize;  
	Arr<ScaleRand, Die::SizeDie> widthCells;
	Arr<ScaleRand, Die::SizeDie> utils;

	// support
	Vec<Num> cellPinCount;
	Vec<Num> instPinCount, netPinCount;
	Vec<ID> pinFromInst;
	Vec<Int> instStartId;
	Vec<Int> instPinsOrder;
	Int instPinsSumCount;

	RandomGen randomGen;
	StringUtil stringUtil;

	void make_net_data();
	void make_instance_data();
	void make_tech_data(Tech& tech, Coor cellHeight, ScaleRand cellWidth);
	void make_techs_data();
	void make_terminal_data();

	AreaSum calc_tech_sum_area(Tech& tech);
	void calc_die_info();

	void load_config(const Str& path);
	void clear_info();
	void make_data();
	void output_data();
public:
	void run(const Str& path = "config.txt");
};

} //Placement
} //WorkSpace
#include "Data/Generate.h"

namespace WorkSpace {
namespace Placement {

void InstanceGenerator::load_config(const Str& path) {
	ifstream file;
	file.open(path.c_str(), ios::in);
	if (!file.is_open()) return;

	string inputStr;
	Vec<Str> strs;
	while (getline(file, inputStr)) {
		stringUtil.string_split_by_delimiter(inputStr, strs);
		if (strs.size() < 2) continue;
		if (strs[0] == "OutputPath") {
			outputPath = strs[1];
			continue;
		}
		if (strs[0] == "OutputName") {
			outputName = strs[1];
			continue;
		}
		if (strs[0] == "NumCells") {
			numCells.load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "NumInstances") {
			numInstance.load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "NumNetPins") {
			numNetPins.load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "NumTechnologies") {
			technologyCount = stoi(strs[1]);
			continue;
		}
		if (strs[0] == "NumLibCellPins") {
			numCellPins.load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "TopBottomHeight") {
			dies[Die::Top].rowHeight = stoi(strs[1]);
			dies[Die::Bottom].rowHeight = stoi(strs[2]);
			continue;
		}
		if (strs[0] == "TopCellWidth") {
			widthCells[Die::Top].load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "BottomCellWidth") {
			widthCells[Die::Bottom].load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "TopDieMaxUitl") {
			utils[Die::Top].load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "BottomDieMaxUitl") {
			utils[Die::Bottom].load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "TerminalSize") {
			terminal.vol.width = stoi(strs[1]);
			terminal.vol.height = stoi(strs[2]);
			continue;
		}
		if (strs[0] == "TerminalSpacing") {
			terminal.spacing = stoi(strs[1]);
			continue;
		}
		if (strs[0] == "BlockCellSize") {
			blockCellSize.load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
		if (strs[0] == "BlockCrossPinSize") {
			blockCrossPinSize.load(stoi(strs[1]), stoi(strs[2]));
			continue;
		}
	}
}

void InstanceGenerator::clear_info() {
	Nets().swap(nets);
	Techs().swap(techs);
	Insts().swap(insts);
	//Vec<Str>().swap(instName);
	//Vec<Str>().swap(cellName);
}

void InstanceGenerator::make_tech_data(Tech& tech, Coor cellHeight, ScaleRand cellWidth) {
	HashMap<Int, ID> locMap;
	Int hashId;

	tech.cellCount = cellCount;
	for (ID cid = 0; cid < tech.cellCount; cid++) {
		Cell cell;
		cell.cellName = "MC" + to_string(cid);
		cell.vol.width = cellWidth.rand(randomGen);
		cell.vol.height = cellHeight;
		cell.pinCount = cellPinCount[cid];
		for (ID pid = 0; pid < cell.pinCount; pid++) {
			PinLoc pin;
			pin.pinName = "P" + to_string(pid);
			do {
				pin.loc.x = randomGen.rand(cell.vol.width + 1);
				pin.loc.y = randomGen.rand(cell.vol.height + 1);
				hashId = pin.loc.x * (cell.vol.height + 1) + pin.loc.y;
			} while (locMap.find(hashId) != locMap.end());
			locMap[hashId] = cid;
			cell.pinLocs.push_back(move(pin));
		}
		tech.cells.push_back(cell);
		locMap.clear();
	}
}

void InstanceGenerator::make_techs_data() {
	cellCount = numCells.rand(randomGen);
	techs.resize(technologyCount);

	cellPinCount.resize(cellCount);
	for (ID cid = 0; cid < cellCount; cid++) {
		cellPinCount[cid] = numCellPins.rand(randomGen);
	}

	for (ID tid = 0; tid < technologyCount; tid++) {
		ID cellHeight = (tid == 0) ? dies[Die::Top].rowHeight : dies[Die::Bottom].rowHeight;
		ScaleRand cellWidth = (tid == 0) ? widthCells[Die::Top] : widthCells[Die::Bottom];

		techs[tid].techName = "TC" + to_string(tid);
		make_tech_data(techs[tid], cellHeight, cellWidth);
	}
}

void InstanceGenerator::make_instance_data() {
	instanceCount = numInstance.rand(randomGen);
	instStartId.resize(instanceCount);
	insts.resize(instanceCount);

	Num pinsNum;
	for (ID iid = 0; iid < instanceCount; iid++) {
		ID cid = randomGen.rand(cellCount);

		insts[iid].instName = "I" + to_string(iid);
		insts[iid].cellName = "MC" + to_string(cid);
		insts[iid].cid[Die::Top] = cid;
		insts[iid].cid[Die::Bottom] = cid;
		pinsNum = cellPinCount[cid];
		instPinCount.push_back(pinsNum);

		instStartId[iid] = instPinsSumCount;
		instPinsSumCount += pinsNum;
	}

	instPinsOrder.resize(instPinsSumCount);
	pinFromInst.resize(instPinsSumCount);

	ID nowInst = 0;
	for (Int iid = 0; iid < instPinsSumCount; iid++) {
		if (nowInst < instanceCount - 1 && iid >= instStartId[nowInst + 1])
			nowInst++;
		instPinsOrder[iid] = iid;
		pinFromInst[iid] = nowInst;
	}
	//random_shuffle(instPinsOrder.begin(), instPinsOrder.end());
}

void InstanceGenerator::make_net_data() {
	Num resPinsCount = instPinsSumCount;
	Num pinsCount;
	while (resPinsCount) {
		if (resPinsCount < numNetPins.l) break;
		pinsCount = min(resPinsCount, numNetPins.rand(randomGen));
		resPinsCount -= pinsCount;
		netPinCount.push_back(pinsCount);
	}
	netCount = netPinCount.size();
	nets.resize(netCount);

	// divide cell into block
	Num pinsUsed = 0;
	Num blockNum = 0;
	while (pinsUsed < instPinsSumCount) {
		Num cellBlockNum = blockCellSize.rand(randomGen);
		Num pinBlockNum = cellBlockNum * (numCellPins.l + numCellPins.r) / 2;
		pinBlockNum = min(pinBlockNum, instPinsSumCount - pinsUsed);
		random_shuffle(instPinsOrder.begin() + pinsUsed, instPinsOrder.begin() + pinsUsed + pinBlockNum);

		// cross block
		if (blockNum > 0) {
			Num crossNum = blockCrossPinSize.rand(randomGen);
			Num crossL = max(sCast<Num>(0) , pinsUsed - crossNum / 2);
			Num crossR = min(pinsUsed + crossNum / 2, pinsUsed + pinBlockNum);
			random_shuffle(instPinsOrder.begin() + crossL, instPinsOrder.begin() + crossR);
		}
	
		pinsUsed += pinBlockNum;
		blockNum++;
	}

	pinsUsed = 0;
	Int pinsOrder;
	ID instId, pinId;
	for (ID nid = 0; nid < netCount; nid++) {
		nets[nid].netName = "N" + to_string(nid);
		nets[nid].pinNum = netPinCount[nid];
		for (ID pid = 0; pid < nets[nid].pinNum; pid++) {
			Pin pin;
			pinsOrder = instPinsOrder[pinsUsed];
			pinsUsed++;
			instId = pinFromInst[pinsOrder];
			pinId = pinsOrder - instStartId[instId];
			pin.instName = "I" + to_string(instId);
			pin.pinName = "P" + to_string(pinId);
			nets[nid].pins.push_back(move(pin));
		}
	}
}

void InstanceGenerator::make_terminal_data() {
}

void InstanceGenerator::calc_die_info() {
	Arr<AreaSum, Die::SizeDie> dieSumArea;
	for (ID did = 0; did < Die::SizeDie; did++) {
		dieSumArea[did] = calc_tech_sum_area(techs[did]);
		dies[did].utilization = utils[did].rand(randomGen);
		dieSumArea[did] = dieSumArea[did] * 100 / dies[did].utilization;
		
		dieSumArea[did] = (AreaSum)((double)dieSumArea[did] * 1.5);
		
		dies[did].techName = techs[did].techName;
	}

	AreaSum area = dieSumArea[Die::Top] * dieSumArea[Die::Bottom] / (dieSumArea[Die::Top] + dieSumArea[Die::Bottom]);
	Coor heightLcm = calc_lcm<Coor>(dies[Die::Top].rowHeight, dies[Die::Bottom].rowHeight);

	die.height = ceil(sqrt(area) / heightLcm) * heightLcm;
	die.width = area / die.height;
}

AreaSum InstanceGenerator::calc_tech_sum_area(Tech& tech) {
	AreaSum areaSum = 0;
	for (ID iid = 0; iid < instanceCount; iid++) {
		ID cid = insts[iid].cid[Die::Top];
		areaSum += (AreaSum)tech.cells[cid].vol.width * tech.cells[cid].vol.height;
	}
	return areaSum;
}

void InstanceGenerator::make_data() {
	clear_info();

	make_techs_data();
	make_instance_data();
	make_net_data();
	make_terminal_data();

	calc_die_info();

}

void InstanceGenerator::output_data() {
	string path = outputPath + "/" + outputName + ".txt";
	ofstream file;
	file.open(path.c_str(), ios::out);

	//instance
	file << "NumInstances " << instanceCount << endl;
	for (ID i = 0; i < instanceCount; i ++) {
		file << "Inst " << insts[i].instName << " " << insts[i].cellName << endl;
	}

	//net
	file << "NumNets " << netCount << endl;
	for (ID nid = 0; nid < netCount; nid ++) {
		file << "Net " << nets[nid].netName << " " << nets[nid].pinNum << endl;
		for (ID pid = 0; pid < nets[nid].pinNum; pid ++) {
			file << "Pin " << nets[nid].pins[pid].instName << "/" << nets[nid].pins[pid].pinName << endl;
		}
	}

	// die
	file << "DieSize 0 0 " << die.width << " " << die.height << endl;

	// util
	file << "TopDieMaxUtil " << dies[Die::Top].utilization << endl;
	file << "BottomDieMaxUtil " << dies[Die::Bottom].utilization << endl;

	//row
	file << "TopDieRows 0 0 " << die.width << " " << dies[Die::Top].rowHeight << " " << die.height / dies[Die::Top].rowHeight << endl;
	file << "BottomDieRows 0 0 " << die.width << " " << dies[Die::Bottom].rowHeight << " " << die.height / dies[Die::Bottom].rowHeight << endl;

	//tech
	file << "NumTechnologies " << technologyCount << endl;
	for (ID tid = 0; tid < technologyCount; tid++) {
		file << "Tech " << techs[tid].techName << " " << techs[tid].cellCount << endl;
		for (ID cid = 0; cid < techs[tid].cellCount; cid++) {
			Cell& tCell = techs[tid].cells[cid];
			file << "LibCell " << tCell.cellName << " " << tCell.vol.width << " " << tCell.vol.height << " " << tCell.pinCount << endl;
			for (ID pid = 0; pid < tCell.pinCount; pid++) {
				PinLoc& cPin = tCell.pinLocs[pid];
				file << "Pin " << cPin.pinName << " " << cPin.loc.x << " " << cPin.loc.y << endl;
			}
		}
	}

	//die tech
	file << "TopDieTech " << dies[Die::Top].techName << endl;
	file << "BottomDieTech " << dies[Die::Bottom].techName << endl;

	//terminal
	file << "TerminalSize " << terminal.vol.width << " " << terminal.vol.height << endl;
	file << "TerminalSpacing " << terminal.spacing << endl;
}

void InstanceGenerator::run(const Str& path) {
	load_config(path);
	make_data();
	output_data();
}

} //WorkSpace
} //Placement
#pragma once

#include "Common/TypeDef.h"
#include "Common/StringUtil.h"

namespace WorkSpace {

struct Config {
	
	StringUtil stringUtil;

	enum ConfigOptions {
		InstancePath,
		SolutionPath,
		LogPath,
		Runtime,
		RandomSeed,

		Size,
	};

	static constexpr char delim = '=';

	const HashMap<Str, ConfigOptions> ConfigOptionKeys = {
		{"-i", InstancePath},
		{"-o", SolutionPath},
		{"-l", LogPath},
		{"-t", Runtime},
		{"-s", RandomSeed}
	};

	const Arr<Str, ConfigOptions::Size> DefaultConfigOptions = {
		"Instance/instance.txt",
		"Solution/solution.txt",
		"Log/log.txt",
		"300",
		""
	};

	Arr<Str, ConfigOptions::Size> configOptions;
	

	void parse(int argc, char* argv[]) {
		Vec<Str> strs;
		for (int i = 1; i < argc; i++) {
			stringUtil.string_split_by_delimiter(argv[i], strs, delim);
			if (strs.size() != 2) continue;
			auto index = ConfigOptionKeys.find(strs[0]);
			if (index == ConfigOptionKeys.end()) continue;
			configOptions[index->second] = strs[1];
		}
		for (int i = 0; i < ConfigOptions::Size; i++) {
			if (configOptions[i].empty()) {
				configOptions[i] = DefaultConfigOptions[i];
			}
		}
	}
};

}

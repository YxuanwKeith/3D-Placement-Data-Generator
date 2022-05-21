#pragma once

#include "Common/TypeDef.h"

namespace WorkSpace {

class StringUtil {
public:
	void refine_str(Str& str) {
		while (!str.empty() && (str.back() == '\n' || str.back() == '\r'))
			str.pop_back();
	}

	void string_split_by_delimiter(const Str& _str, Vec<Str>& arr, const char delimiter = ' ') {
		string str = _str;
		Vec<Str>().swap(arr);
		
		refine_str(str);
		int len = sCast<int>(str.length());
		int pos = 0;
		for (int i = 0; i < len; i++) {
			if (str[i] == delimiter) {
				arr.push_back(str.substr(pos, i - pos));
				pos = i + 1;
			}
		}
		if (len > pos) arr.push_back(str.substr(pos, len - pos));
	}
};

}

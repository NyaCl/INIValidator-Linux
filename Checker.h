﻿#pragma once
#include "IniFile.h"
#include "TypeChecker.h"
#include "ListChecker.h"
#include "LimitChecker.h"
#include "NumberChecker.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <stack>
#include <set>

class DictData {
public:
	std::vector<std::string> types;
	std::string defaultValue;
	std::string file;
};

class Dict {
public:
	auto begin() { return section.begin(); }
	auto begin() const { return section.begin(); }
	auto end() { return section.end(); }
	auto end() const { return section.end(); }
	void insert(const Section& other) { return section.insert(other.begin(), other.end()); }
	size_t count(const std::string& key) const { return section.count(key); }
	DictData at(const std::string& key) const { return section.at(key); }
	DictData& at(const std::string& key) { return section.at(key); }
	DictData& operator[](const std::string& key) { return section[key]; }

	std::vector<std::string> dynamicKeys;					// 存储所有需要动态生成的key
	std::unordered_map<std::string, DictData> section;			// key <-> 该key对应的自定义类型
};

class Checker {
public:
    Checker(IniFile& configFile, IniFile& targetIni);
    void loadConfig(IniFile& configFile);
    void checkFile();

private:
	friend ListChecker;
	friend TypeChecker;
	using Sections = std::unordered_map<std::string, Dict>;
	using Limits = std::unordered_map<std::string, LimitChecker>;
	using Lists = std::unordered_map<std::string, ListChecker>;
	using Numbers = std::unordered_map<std::string, NumberChecker>;
	
	Section registryMap;	// 注册表名字映射: 配置ini的Type名字 <-> 注册ini中注册表名字(注册表可能不存在,则value="")
	Numbers numberLimits;	// 特殊类型限制: 类型名 <-> 特殊限制类型section
	Limits limits;			// 特殊类型限制: 类型名 <-> 特殊限制类型section
	Lists lists;			// 特殊类型限制: 类型名 <-> 特殊限制类型section
	Sections sections;		// 常规类型限制: 类型名 <-> 自定义类型section
	IniFile* targetIni;		// 检查的ini

	DictData parseTypeValue(const std::string& str);
    void validateSection(const std::string& sectionName, const Section& object, const std::string& type = "");
	void validate(const Section& section, const std::string& key, const Value& value, const std::string& type);
	std::vector<std::string> generateKey(const std::string& dynamicKey, const Section& object) const;
	double evaluateExpression(const std::string& expr, const Section& object) const;
	double parseValue(size_t& i, const std::string& expr, const Section& object) const;
	void applyOperation(std::stack<double>& values, std::stack<char>& operators) const;
	
	int isInteger(const Value& str);
	float isFloat(const Value& str);
	double isDouble(const Value& str);
	std::string isString(const Value& str);
};

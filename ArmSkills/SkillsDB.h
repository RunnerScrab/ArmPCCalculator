/*
Copyright (c) 2018 RunnerScrab

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <string>
#include <functional>
#include <map>

#include "Skill.h"
#include "PC_Class.h"

struct sqlite3;
struct sqlite3_stmt;
class Character;

class SkillsDB
{	
public:
	SkillsDB(const char* dbfilename = "armskills.db");
	~SkillsDB();

	bool IsMainGuild(const char* str);
	bool IsSubGuild(const char* str);

	std::map<std::string, PC_Class>& GetClassMap()
	{
		return m_classmap;
	}

	std::shared_ptr<Character> CreateCharacter(const char* mg, const char* sg);

private:
	int LoadRows(const char * tablename, unsigned int expected_columns,
		std::function<void(sqlite3_stmt *)> fpRowCallback);
	int LoadSkills(std::map<std::string, Skill>& skill_map);
	int LoadPCClasses(std::map<std::string, PC_Class>& pc_class_map);
	int PopulatePCClassSkillList(std::map<std::string, Skill>& base_skills, PC_Class * pc_class);
	int LoadDatabase();

	std::string m_dbfilename;
	sqlite3* m_pDb;

	std::map<std::string, Skill> m_skillmap;
	std::map<std::string, PC_Class> m_classmap;
};

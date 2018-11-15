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

#include "pch.h"
#include "SkillsDB.h"
#include "Skill.h"
#include "../sqlite3lib/sqlite3.h"
#include "Character.h"
#include <memory>

SkillsDB::SkillsDB(const char * dbfilename) :
	m_dbfilename(dbfilename)
{
	int rc = sqlite3_open_v2(dbfilename, &m_pDb, SQLITE_OPEN_READONLY, 0);
	if (SQLITE_OK != rc)
	{
		std::string errmsg = "Failed to open db file: ";
		errmsg += sqlite3_errmsg(m_pDb);
		sqlite3_close(m_pDb);
		m_pDb = nullptr;
		throw std::exception(errmsg.c_str());
	}

	if (LoadDatabase() < 0)
	{
		throw std::exception("Failed to load data from database.");
	}
}

SkillsDB::~SkillsDB()
{
	sqlite3_close(m_pDb);
}

inline bool SkillsDB::IsMainGuild(const char * str)
{
	auto founditer = m_classmap.find(str);
	return founditer != m_classmap.end() && !founditer->second.m_bIsSubclass;
}

inline bool SkillsDB::IsSubGuild(const char * str)
{
	auto founditer = m_classmap.find(str);
	return founditer != m_classmap.end() && founditer->second.m_bIsSubclass;
}

std::shared_ptr<Character> SkillsDB::CreateCharacter(const char * mg, const char * sg)
{
	if (mg && !IsMainGuild(mg))
	{
		//Allow a null main guild, to display subguild data
		return nullptr;
	}
	if (sg && !IsSubGuild(sg))
	{
		//Allow a null subguild, to display main guild data
		return nullptr;
	}

	unsigned int mglen = strlen(mg);
	unsigned int sglen = strlen(sg);

	std::shared_ptr<Character> pc_out = std::make_shared<Character>(mglen ? &m_classmap[mg] : nullptr,
		sglen ? &m_classmap[sg] : nullptr);
	return pc_out;
}

int SkillsDB::LoadRows(const char * tablename, unsigned int expected_columns,
	std::function<void(sqlite3_stmt *)> fpRowCallback)
{
	sqlite3_stmt *res;
	std::string query = "select * from " + std::string(tablename) + ";";
	int rc = sqlite3_prepare_v2(m_pDb, query.c_str(), -1, &res, 0);

	if (SQLITE_OK != rc)
	{
		return -1;
	}

	unsigned int columns = sqlite3_column_count(res);

	if (expected_columns != columns)
	{
		return -1;
	}

	do
	{
		rc = sqlite3_step(res);
		if (rc != SQLITE_ROW)
		{
			break;
		}

		fpRowCallback(res);

	} while (SQLITE_ROW == rc);

	sqlite3_finalize(res);

	return 0;
}

int SkillsDB::LoadSkills(std::map<std::string, Skill>& skill_map)
{
	int result = LoadRows("skills", 2,
		[&skill_map](sqlite3_stmt* res)
		{
			std::string name = reinterpret_cast<const char*>(sqlite3_column_text(res, 0));
			std::string type = reinterpret_cast<const char*>(sqlite3_column_text(res, 1));
			skill_map[name].m_name = name;
			skill_map[name].m_type = Skill::SkillStrToType(type);
		}
	);

	return result;
}

int SkillsDB::LoadPCClasses(std::map<std::string, PC_Class>& pc_class_map)
{
	int result = LoadRows("classes", 11,
		[&pc_class_map](sqlite3_stmt* res)
	{
		std::string class_name = reinterpret_cast<const char*>(sqlite3_column_text(res, 0));
		PC_Class& pc_class = pc_class_map[class_name];
		pc_class.m_strName = class_name;
		pc_class.m_bIsSubclass = sqlite3_column_int(res, 1);
		pc_class.m_udwTKP_cost = sqlite3_column_int(res, 2);
		pc_class.m_environment = PC_Class::EnvStringToEnvType(reinterpret_cast<const char*>(sqlite3_column_text(res, 3)));
		pc_class.m_ForageFoodAbility = static_cast<PC_Class_Skill::SkillEnvironmentType>(sqlite3_column_int(res, 4));
		pc_class.m_bCanCamp = sqlite3_column_int(res, 5);
		pc_class.m_ExertionRecovery = PC_Class::ExertionRecStringToType(reinterpret_cast<const char*>(sqlite3_column_text(res, 6)));
		pc_class.m_bCanTame = sqlite3_column_int(res, 7);
		pc_class.m_udwMaxMounts = sqlite3_column_int(res, 8);
		pc_class.m_PainTolerance = PC_Class::ToleranceStringToType(reinterpret_cast<const char*>(sqlite3_column_text(res, 9)));
		pc_class.m_AlcoholTolerance = PC_Class::ToleranceStringToType(reinterpret_cast<const char*>(sqlite3_column_text(res, 10)));
	}
	);

	return result;
}

int SkillsDB::PopulatePCClassSkillList(std::map<std::string, Skill>& base_skills, PC_Class * pc_class)
{
	//HACK: oh well
	std::string tablename = "class_skills where class_name = '" + pc_class->m_strName + "'";

	std::vector<PC_Class_Skill>& sk_list = pc_class->m_class_skill_list;
	int result = LoadRows(tablename.c_str(), 5,
		[&base_skills, &sk_list](sqlite3_stmt* res)
	{
		sk_list.push_back(PC_Class_Skill());
		std::string skill_name = reinterpret_cast<const char*>(sqlite3_column_text(res, 1));
		std::string max_level = reinterpret_cast<const char*>(sqlite3_column_text(res, 2));
		const char* thirdcol = reinterpret_cast<const char*>(sqlite3_column_text(res, 3));
		std::string branches_from = thirdcol ? thirdcol : "";
		const char* fourcol = reinterpret_cast<const char*>(sqlite3_column_text(res, 4));
		std::string environment = fourcol ? fourcol : "";

		sk_list.back().m_baseSkill = &base_skills[skill_name];
		sk_list.back().m_maxlevel = Skill::LevelStrToEnum(max_level);
		sk_list.back().m_pBranchesFrom = "" == branches_from ? nullptr : &base_skills[branches_from];
		sk_list.back().m_environment = PC_Class_Skill::EnvStringToEnvType(environment);
	}
	);

	return result;
}

int SkillsDB::LoadDatabase()
{
	if (LoadSkills(m_skillmap) < 0)
	{
		return -1;
	}

	if (LoadPCClasses(m_classmap) < 0)
	{
		return -1;
	}

	for (auto it = m_classmap.begin(); it != m_classmap.end();
		++it)
	{
		PopulatePCClassSkillList(m_skillmap, &(it->second));
	}

	return 0;
}


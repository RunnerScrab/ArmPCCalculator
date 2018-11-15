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
#include "Skill.h"

struct PC_Class_Skill
{
	typedef enum { NONE = 0, WILDERNESS = 1, CITY = 2, BOTH = 3 } SkillEnvironmentType;

	static PC_Class_Skill::SkillEnvironmentType EnvStringToEnvType(const std::string & str);
	static const char* EnvTypeToString(SkillEnvironmentType type);

	const char* GetMaxLevelStr() const
	{
		return Skill::LevelEnumTypeToStr(m_maxlevel);
	}

	const char* GetEnvironmentStr() const
	{
		return EnvTypeToString(m_environment);
	}

	const char* GetEnvironmentCode() const
	{
		//As strings rather than chars, for convenience
		const char* codes[] = { "N", "W", "C", "B" };
		return codes[m_environment];
	}

	PC_Class_Skill()
	{

	}

	PC_Class_Skill& operator=(const PC_Class_Skill& other)
	{
		if (this != &other)
		{
			m_baseSkill = other.m_baseSkill;
			m_maxlevel = other.m_maxlevel;
			m_pBranchesFrom = other.m_pBranchesFrom;
			m_environment = other.m_environment;
			m_combinationeffects = other.m_combinationeffects;
		}

		return *this;
	}

	PC_Class_Skill(PC_Class_Skill&& other)
	{
		m_baseSkill = other.m_baseSkill;
		m_maxlevel = other.m_maxlevel;
		m_pBranchesFrom = other.m_pBranchesFrom;
		m_environment = other.m_environment;
		m_combinationeffects = other.m_combinationeffects;
	}

	static PC_Class_Skill CombineSkills(PC_Class_Skill* main, PC_Class_Skill* sub);

	struct Skill* m_baseSkill;
	Skill::Level m_maxlevel;
	struct Skill* m_pBranchesFrom;
	SkillEnvironmentType m_environment;
	std::vector<std::string> m_combinationeffects;
};
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
#include "PC_Class_Skill.h"

PC_Class_Skill::SkillEnvironmentType PC_Class_Skill::EnvStringToEnvType(const std::string & str)
{
	if ("city" == str)
	{
		return CITY;
	}
	else if ("wilderness" == str)
	{
		return WILDERNESS;
	}
	else if ("both" == str)
	{
		return BOTH;
	}
	else
	{
		return NONE;
	}
}

const char * PC_Class_Skill::EnvTypeToString(SkillEnvironmentType type)
{
	const char* environments[] = {"", "wilderness", "city",
		"city and wilderness"};
	return environments[static_cast<int>(type)];
}

PC_Class_Skill PC_Class_Skill::CombineSkills(PC_Class_Skill * main, PC_Class_Skill * sub)
{
	if (main->m_baseSkill != sub->m_baseSkill)
	{
		throw std::exception("Cannot combine two different skills.\n");
	}


	PC_Class_Skill newskill;
	std::vector<std::string>& combeffects = newskill.m_combinationeffects;

	newskill.m_baseSkill = main->m_baseSkill;
	newskill.m_maxlevel = main->m_maxlevel > sub->m_maxlevel ? main->m_maxlevel : sub->m_maxlevel;

	if (main->m_maxlevel < sub->m_maxlevel)
	{
		combeffects.emplace_back("raises cap");
	}


	if (!main->m_pBranchesFrom || !sub->m_pBranchesFrom)
	{
		//Either main or sub starts with this, so no need to branch it
		newskill.m_pBranchesFrom = nullptr;
	}
	else
	{
		newskill.m_pBranchesFrom = main->m_pBranchesFrom;
	}

	if (!sub->m_pBranchesFrom && main->m_pBranchesFrom)
	{
		combeffects.emplace_back("skips branching it from " + main->m_pBranchesFrom->m_name);
	}
	else if(main->m_pBranchesFrom && sub->m_pBranchesFrom)
	{
		if (sub->m_pBranchesFrom == main->m_pBranchesFrom)
		{
			combeffects.emplace_back(" also branches from " + sub->m_pBranchesFrom->m_name);
		}
		else
		{
			combeffects.emplace_back(" may allow branching from " + sub->m_pBranchesFrom->m_name);
		}
	}

	newskill.m_environment = static_cast<SkillEnvironmentType>(main->m_environment | sub->m_environment);

	if (main->m_environment != sub->m_environment)
	{
		union
		{
			struct
			{
				unsigned char hb : 1;
				unsigned char lb : 1;
			} bits;
			PC_Class_Skill::SkillEnvironmentType env = PC_Class_Skill::SkillEnvironmentType::NONE;
		} A, B, Q;

		A.env = main->m_environment;
		B.env = sub->m_environment;
		Q.bits.hb = (~A.bits.hb) & B.bits.hb;
		Q.bits.lb = (~A.bits.lb) & B.bits.lb;

		PC_Class_Skill::SkillEnvironmentType envflag = Q.env;
		combeffects.emplace_back("improves " + std::string(PC_Class_Skill::EnvTypeToString(envflag)) + " use");
	}

	if (combeffects.empty())
	{
		combeffects.emplace_back("no improvement");
	}

	return std::move(newskill);
}

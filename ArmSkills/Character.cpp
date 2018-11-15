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
#include "Character.h"

#include <string>
#include <sstream>
#include <set>

void Character::ProcessClassSkills()
{
	if (!this->m_pMainClass || !this->m_pSubClass)
	{
		return;
	}
	std::ostringstream skill_report;
	std::set<std::string> skillset;

	skill_report << "[] Codes: C = City, W = Wilderness, B = Both City and Wilderness\r\nA skill in [] is what branches the marked skill.\r\n\r\n";

	for (const PC_Class_Skill& sk : m_pMainClass->m_class_skill_list)
	{
		if (skillset.insert(sk.m_baseSkill->m_name).second == false)
		{

		}
		else
		{
			m_combinedskills[sk.m_baseSkill->m_name] = sk;
		}

	}

	std::vector<PC_Class_Skill*> overlappingskills;

	for (PC_Class_Skill& sk : m_pSubClass->m_class_skill_list)
	{
		if (skillset.insert(sk.m_baseSkill->m_name).second == false)
		{
			PC_Class_Skill combskill = PC_Class_Skill::CombineSkills(&m_combinedskills[sk.m_baseSkill->m_name],
				&sk);
			//Subguild skill overlaps main guild skill
			m_combinedskills[sk.m_baseSkill->m_name] = combskill;
			overlappingskills.push_back(&m_combinedskills[sk.m_baseSkill->m_name]);
		}
		else
		{
			m_combinedskills[sk.m_baseSkill->m_name] = sk;
		}

	}
	for (unsigned int i = 0; i < Skill::SkillType::NUM_SKILLS; ++i)
	{

		std::ostringstream skillclass_report;
		bool bSecondOnLine = false;
		unsigned int foundskills = 0;
		for (auto it = m_combinedskills.begin(); it != m_combinedskills.end(); ++it)
		{
			const PC_Class_Skill& sk = it->second;
			if (sk.m_baseSkill->m_type == i)
			{
				++foundskills;
				std::string skillname = sk.m_baseSkill->m_name;
				if (sk.m_environment || sk.m_pBranchesFrom)
				{
					skillname += "[";
					if (sk.m_environment)
					{
						skillname += std::string(sk.GetEnvironmentCode());
					}
					if (sk.m_pBranchesFrom)
					{
						if (sk.m_environment)
							skillname += ",";
						skillname += sk.m_pBranchesFrom->m_name;
					}
					skillname += "]";
				}
				std::string level = "(" + std::string(Skill::LevelEnumTypeToStr(sk.m_maxlevel)) + ")";
				int numspaces = max(((38 - skillname.length()) - level.length()), static_cast<unsigned int>(0));

				skillclass_report << skillname;

				for (int j = 0; j < numspaces; ++j)
				{
					//Spaces between skill name and its max level
					skillclass_report << " ";
				}
				skillclass_report << level;

				skillclass_report << (!bSecondOnLine ? " " : "\r\n");
				bSecondOnLine = !bSecondOnLine;

			}
		}

		if (foundskills)
		{
			skill_report << Skill::SkillTypeToStr(static_cast<Skill::SkillType>(i)) <<
				(!i ? " powers" : " skills") <<
				"\r\n-----------------------------------------------------------------------------\r\n";

			skill_report << skillclass_report.str();

			skill_report << "\r\n";
			if (bSecondOnLine)
				skill_report << "\r\n";
		}
	}

	if (overlappingskills.size())
	{
		skill_report << overlappingskills.size() << "/" << m_pSubClass->m_class_skill_list.size()<<" subguild skills overlapping main guild skills:\r\n";
		for (PC_Class_Skill* pSkill : overlappingskills)
		{
			skill_report << pSkill->m_baseSkill->m_name << ": ";
			for (int i = 0, z = pSkill->m_combinationeffects.size(); i < z; ++i)
			{
				skill_report << pSkill->m_combinationeffects[i];
				if (i < (z - 1))
				{
					skill_report << "; ";
				}
			}

			skill_report << "\r\n";
		}
	}

	m_SkillReport = skill_report.str();
}

Character::Character(PC_Class * main, PC_Class * sub) :
	m_pMainClass(main), m_pSubClass(sub)
{
	ProcessClassAbilities();
	ProcessClassSkills();
}

int Character::ProcessClassAbilities()
{
	if (!this->m_pMainClass || !this->m_pSubClass)
	{
		return -1;
	}

	unsigned int tkp_cost = max(m_pMainClass->m_udwTKP_cost, m_pSubClass->m_udwTKP_cost);
	unsigned int max_mounts = max(m_pMainClass->m_udwMaxMounts, m_pSubClass->m_udwMaxMounts);
	PC_Class::EnvironmentType environment = static_cast<PC_Class::EnvironmentType>(m_pMainClass->m_environment | m_pSubClass->m_environment);
	int forage_food_abil = m_pMainClass->m_ForageFoodAbility | m_pSubClass->m_ForageFoodAbility;
	bool can_camp = max(m_pMainClass->m_bCanCamp, m_pSubClass->m_bCanCamp);
	bool can_tame = max(m_pMainClass->m_bCanTame, m_pSubClass->m_bCanTame);

	PC_Class::ExertionRecoveryType exertion_recovery = static_cast<PC_Class::ExertionRecoveryType>(
		max(m_pMainClass->m_ExertionRecovery, m_pSubClass->m_ExertionRecovery)
		);
	PC_Class::ToleranceType alcohol_tolerance =
		static_cast<PC_Class::ToleranceType>(max(m_pMainClass->m_AlcoholTolerance,
			m_pSubClass->m_AlcoholTolerance));

	PC_Class::ToleranceType pain_tolerance =
		static_cast<PC_Class::ToleranceType>(max(m_pMainClass->m_PainTolerance,
			m_pSubClass->m_PainTolerance));

	std::string environment_rep_str = ((environment == PC_Class::EnvironmentType::CITY) ? "the city" :
		(environment == PC_Class::EnvironmentType::WILDERNESS) ? "the wilderness" :
		(environment == PC_Class::EnvironmentType::BOTH) ? "both the city and wilderness" : "neither city or wilderness");
	std::ostringstream report_strstream;
	report_strstream << "This character will cost " << tkp_cost << " temporary karma points.\r\n";

	/*
	if (environment > PC_Class::EnvironmentType::NEUTRAL)
	{
		report_strstream << "Any stealth, hunting, perception, or food foraging skills will be specialized for " <<
			environment_rep_str << ".\n";
	}
	*/
	bool bHasAbility = false;
	report_strstream << "It will:\r\n";


	if (can_camp)
	{
		report_strstream << "Be able to camp (quit out in wilderness)\r\n";
		bHasAbility = true;
	}

	if (PC_Class_Skill::SkillEnvironmentType::NONE != forage_food_abil)
	{
		report_strstream << "Be able to forage for food in the " << PC_Class_Skill::EnvTypeToString(
			static_cast<PC_Class_Skill::SkillEnvironmentType>(forage_food_abil)) << ".\r\n";
		bHasAbility = true;
	}

	if (can_tame)
	{
		report_strstream << "Be able to tame mounts.\r\n";
		bHasAbility = true;
	}

	if (max_mounts > 1)
	{
		report_strstream << "Be able to hitch " << max_mounts << " mounts at a time.\r\n";
		bHasAbility = true;
	}

	if (exertion_recovery > PC_Class::ExertionRecoveryType::NORMAL_REC)
	{
		report_strstream << "Have " << PC_Class::ExertionRecTypeToStr(exertion_recovery) << " stamina recovery out of doors.\r\n";
		bHasAbility = true;
	}
	if (pain_tolerance > PC_Class::ToleranceType::NORMAL_TOL)
	{
		report_strstream << "Possess " << PC_Class::ToleranceTypeToStr(pain_tolerance) << " tolerance for pain\r\n";
		bHasAbility = true;
	}
	if (alcohol_tolerance > PC_Class::ToleranceType::NORMAL_TOL)
	{
		report_strstream << "Possess " << PC_Class::ToleranceTypeToStr(pain_tolerance) << " tolerance for alcohol.\r\n";
		bHasAbility = true;
	}

	if (!bHasAbility)
	{
		report_strstream << "Not possess any special class abilities.\r\n";
	}


	m_AbilityReport = report_strstream.str();

	return 0;
}

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
#include <vector>
#include "PC_Class_Skill.h"

struct PC_Class
{
	typedef enum {NONE = 0, WILDERNESS = 1, CITY = 2, BOTH = 3} EnvironmentType;
	typedef enum {NORMAL_REC = 0, FAIR_REC = 1, GOOD_REC = 2} ExertionRecoveryType;
	typedef enum {NORMAL_TOL = 0, INCREASED_TOL = 1 } ToleranceType;

	static EnvironmentType EnvStringToEnvType(const std::string& str);
	static ToleranceType ToleranceStringToType(const std::string& str);
	static ExertionRecoveryType ExertionRecStringToType(const std::string& str);

	static const char* ToleranceTypeToStr(PC_Class::ToleranceType type);
	static const char* ExertionRecTypeToStr(PC_Class::ExertionRecoveryType type);

	std::string m_strName;
	bool m_bIsSubclass;
	unsigned int m_udwTKP_cost;
	EnvironmentType m_environment; //Deprecated: General environment
	PC_Class_Skill::SkillEnvironmentType m_ForageFoodAbility;
	bool m_bCanCamp;
	ExertionRecoveryType m_ExertionRecovery;
	bool m_bCanTame;
	unsigned int m_udwMaxMounts;
	ToleranceType m_PainTolerance;
	ToleranceType m_AlcoholTolerance;
	std::vector<PC_Class_Skill> m_class_skill_list;

	bool IsSubclass() const
	{
		return m_bIsSubclass;
	}
};
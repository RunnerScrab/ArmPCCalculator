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
#include "PC_Class.h"
#include <map>

class Character
{
public:
	Character(PC_Class* main, PC_Class* sub);

	std::string* GetAbilityReport()
	{
		return &m_AbilityReport;
	}

	std::string* GetSkillReport()
	{
		return &m_SkillReport;
	}

	std::string GetCombinedReport()
	{
		std::string report = m_AbilityReport + "\r\n" + m_SkillReport;
		return std::move(report);
	}

private:
	int ProcessClassAbilities();
	void ProcessClassSkills();

	PC_Class* m_pMainClass, *m_pSubClass;
	std::map<std::string, PC_Class_Skill> m_combinedskills;
	std::string m_AbilityReport, m_SkillReport;
};
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
#include "Skill.h"

const char * Skill::SkillTypeToStr(Skill::SkillType type)
{
	static const char* skillnametable[9] = { "Psionic", "Combat", "Weapon",
		"Manipulation", "Perception", "Barter", "Language",
		"Craft", "Stealth" };
	return skillnametable[static_cast<int>(type)];
}

Skill::SkillType Skill::SkillStrToType(const std::string & type)
{
	if ("Combat" == type)
	{
		return SKTYPE_COMBAT;
	}
	else if ("Craft" == type)
	{
		return SKTYPE_CRAFT;
	}
	else if ("Weapon" == type)
	{
		return SKTYPE_WEAPON;
	}
	else if ("Stealth" == type)
	{
		return SKTYPE_STEALTH;
	}
	else if ("Perception" == type)
	{
		return SKTYPE_PERCEPTION;
	}
	else if ("Manipulation" == type)
	{
		return SKTYPE_MANIPULATION;
	}
	else if ("Barter" == type)
	{
		return SKTYPE_BARTER;
	}
	else if ("Language" == type)
	{
		return SKTYPE_LANGUAGE;
	}
	else
	{
		return SKTYPE_PSIONIC;
	}

}

Skill::Level Skill::LevelStrToEnum(const std::string & levelstr)
{
	if ("novice" == levelstr)
	{
		return NOVICE;
	}
	else if ("apprentice" == levelstr)
	{
		return APPRENTICE;
	}
	else if ("journeyman" == levelstr)
	{
		return JOURNEYMAN;
	}
	else if ("advanced" == levelstr)
	{
		return ADVANCED;
	}
	else
	{
		return MASTER;
	}
}

const char * Skill::LevelEnumTypeToStr(Level leveltype)
{
	static const char* levelnames[] = {
		"novice", "apprentice", "journeyman", "advanced", "master"
	};
	return levelnames[static_cast<int>(leveltype)];
}

Skill::Skill(const std::string name, const std::string type) :
	m_name(name)
{
	m_type = SkillStrToType(type);
}

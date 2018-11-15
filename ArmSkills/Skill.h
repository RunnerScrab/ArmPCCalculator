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

#ifndef SKILL_H_
#define SKILL_H_
#include <string>
#include <vector>

struct Skill
{
	typedef enum {
		SKTYPE_PSIONIC = 0, SKTYPE_COMBAT, SKTYPE_WEAPON, SKTYPE_STEALTH, SKTYPE_MANIPULATION,
		SKTYPE_PERCEPTION, SKTYPE_BARTER, SKTYPE_LANGUAGE, SKTYPE_CRAFT, NUM_SKILLS
	} SkillType;

	typedef enum {NOVICE = 0, APPRENTICE, JOURNEYMAN, ADVANCED, MASTER} Level;

	Skill() {}
	Skill(const std::string name, const std::string type);

	static const char* SkillTypeToStr(SkillType type);
	static SkillType SkillStrToType(const std::string& type);
	static Level LevelStrToEnum(const std::string& levelstr);
	static const char* LevelEnumTypeToStr(Level leveltype);

	std::string m_name;
	SkillType m_type;
};

#endif /* SKILL_H_ */
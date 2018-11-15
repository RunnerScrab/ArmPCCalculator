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
#include "PC_Class.h"

PC_Class::EnvironmentType PC_Class::EnvStringToEnvType(const std::string & str)
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

PC_Class::ToleranceType PC_Class::ToleranceStringToType(const std::string & str)
{
	if ("increased" == str)
	{
		return ToleranceType::INCREASED_TOL;
	}
	else
	{
		return ToleranceType::NORMAL_TOL;
	}
}

PC_Class::ExertionRecoveryType PC_Class::ExertionRecStringToType(const std::string & str)
{
	if ("fair" == str)
	{
		return FAIR_REC;
	}
	else if ("good" == str)
	{
		return GOOD_REC;
	}
	else
	{
		return NORMAL_REC;
	}
}

const char * PC_Class::ToleranceTypeToStr(PC_Class::ToleranceType type)
{
	static const char* tolerances[] = { "normal", "increased" };
	return tolerances[static_cast<int>(type)];
}

const char * PC_Class::ExertionRecTypeToStr(PC_Class::ExertionRecoveryType type)
{
	static const char* exertions[] = { "normal", "fair", "good" };
	return exertions[static_cast<int>(type)];
}

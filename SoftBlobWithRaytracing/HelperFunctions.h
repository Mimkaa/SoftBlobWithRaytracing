#pragma once
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

std::wstring ToWideString(std::string str_in);

std::string ToNarrowString(const std::wstring& wideStr);


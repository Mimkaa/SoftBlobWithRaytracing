#pragma once
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

std::wstring ToWideString(std::string str_in)
{
   

    // Create a wide string converter with the locale settings
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideString = converter.from_bytes(str_in);

    return wideString;
}


std::string ToNarrowString(const std::wstring& wideStr) {
    // Create a wide string converter with the locale settings
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Convert the wide string to a narrow string
    std::string narrowStr = converter.to_bytes(wideStr);

    // Return the narrow string
    return narrowStr;
}


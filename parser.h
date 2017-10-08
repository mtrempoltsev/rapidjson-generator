#pragma once

#include <memory>
#include <string>

class Item;

class Parser final
{
public:
    static std::unique_ptr<Item> parse(const char* text);

private:
    static void skipSpaces(const char*& text);
    static void moveToEndOfName(const char*& text);
    static std::string getName(const char*& text);
};

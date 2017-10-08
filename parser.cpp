#include "parser.h"

#include <cctype>
#include <cstring>
#include <stack>
#include <unordered_map>

const std::unordered_map<std::string, Item::Type> SimpleTypes =
{
    { "bool",           Item::Type::Bool },
    { "double",         Item::Type::Double },
    { "int8_t",         Item::Type::Int8 },
    { "int16_t",        Item::Type::Int16 },
    { "int32_t",        Item::Type::Int32 },
    { "int64_t",        Item::Type::Int64 },
    { "uint8_t",        Item::Type::UInt8 },
    { "uint16_t",       Item::Type::UInt16 },
    { "uint32_t",       Item::Type::UInt32 },
    { "uint64_t",       Item::Type::UInt64 },
    { "std::string",    Item::Type::String }
};

const char* toString(Item::Type type)
{
    switch (type)
    {
    case Item::Type::Root:
        return "{root}";
    case Item::Type::Namespace:
        return "namespace";
    case Item::Type::Struct:
        return "struct";
    case Item::Type::Bool:
        return "bool";
    case Item::Type::Double:
        return "double";
    case Item::Type::Int8:
        return "int8";
    case Item::Type::Int16:
        return "int16";
    case Item::Type::Int32:
        return "int32";
    case Item::Type::Int64:
        return "int64";
    case Item::Type::UInt8:
        return "uint8";
    case Item::Type::UInt16:
        return "uint16";
    case Item::Type::UInt32:
        return "uint32";
    case Item::Type::UInt64:
        return "uint64";
    case Item::Type::String:
        return "string";
    case Item::Type::Array:
        return "array";
    }
    return "{invalid}";
}

Item::Item()
    : type_(Type::Root)
    , parent_(nullptr)
{
}

Item::Item(std::string&& name, Type type, Item* parent)
    : name_(std::move(name))
    , type_(type)
    , parent_(parent)
{
}

std::unique_ptr<Item> Item::clone(Item* parent)
{
    auto root = std::make_unique<Item>(std::string(name_), type_, parent);
    for (auto& child : children_)
    {
        root->children_.push_back(child->clone(root.get()));
    }
    return root;
}

Item* Item::addChild(std::string&& name, Item::Type type)
{
    auto item = std::make_unique<Item>(std::move(name), type, this);
    auto result = item.get();
    children_.push_back(std::move(item));
    return result;
}

Item* Item::find(const std::string& name) const
{
    for (auto& item : children_)
    {
        if (item->name_ == name)
            return item.get();

        auto result = item->find(name);
        if (result != nullptr)
            return result;
    }

    return nullptr;
}

std::unique_ptr<Item> Parser::parse(const char* text)
{
    auto root = std::make_unique<Item>();
    auto current = root.get();

    int level = 0;
    std::stack<int> parentLevel_;

    while (auto c = *text++)
    {
        if (std::isspace(c))
            continue;

        switch (c)
        {
        case '{':
            ++level;
            break;
        case '}':
            --level;
            if (level == parentLevel_.top())
            {
                parentLevel_.pop();
                current = current->parent_;
            }
            break;
        case 'b':
            if (*text && *text++ == 'o' &&
                *text && *text++ == 'o' &&
                *text && *text++ == 'l' &&
                *text && std::isspace(*text))
            {
                current->addChild(getName(text), Item::Type::Bool);
            }
            break;
        case 'd':
            if (*text && *text++ == 'o' &&
                *text && *text++ == 'u' &&
                *text && *text++ == 'b' &&
                *text && *text++ == 'l' &&
                *text && *text++ == 'e' &&
                *text && std::isspace(*text))
            {
                current->addChild(getName(text), Item::Type::Double);
            }
            break;
        case 'i':
            if (*text && *text++ == 'n' &&
                *text && *text++ == 't' &&
                *text)
            {
                switch (*text++)
                {
                case '8':
                    if (*text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::Int8);
                    }
                    break;
                case '1':
                    if (*text && *text++ == '6' &&
                        *text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::Int16);
                    }
                    break;
                case '3':
                    if (*text && *text++ == '2' &&
                        *text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::Int32);
                    }
                    break;
                case '6':
                    if (*text && *text++ == '4' &&
                        *text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::Int64);
                    }
                    break;
                }
            }
            break;
        case 'n':
            if (*text && *text++ == 'a' &&
                *text && *text++ == 'm' &&
                *text && *text++ == 'e' &&
                *text && *text++ == 's' &&
                *text && *text++ == 'p' &&
                *text && *text++ == 'a' &&
                *text && *text++ == 'c' &&
                *text && *text++ == 'e' &&
                *text && std::isspace(*text))
            {
                current = current->addChild(getName(text), Item::Type::Namespace);
                parentLevel_.push(level);
            }
            break;
        case 's':
            if (*text && *text++ == 't' &&
                *text)
            {
                switch (*text++)
                {
                case 'd':
                    if (*text && *text++ == ':' &&
                        *text && *text++ == ':' &&
                        *text)
                    {
                        switch (*text++)
                        {
                        case 's':
                            if (*text && *text++ == 't' &&
                                *text && *text++ == 'r' &&
                                *text && *text++ == 'i' &&
                                *text && *text++ == 'n' &&
                                *text && *text++ == 'g' &&
                                *text && std::isspace(*text))
                            {
                                current->addChild(getName(text), Item::Type::String);
                            }
                            break;
                        case 'v':
                            if (*text && *text++ == 'e' &&
                                *text && *text++ == 'c' &&
                                *text && *text++ == 't' &&
                                *text && *text++ == 'o' &&
                                *text && *text++ == 'r' &&
                                *text && *text == '<')
                            {
                                const auto typeName = getName(text);

                                const auto array = current->addChild(getName(text), Item::Type::Array);

                                const auto type = SimpleTypes.find(typeName);
                                if (type != SimpleTypes.end())
                                {
                                    array->addChild(std::string(), type->second);
                                }
                                else
                                {
                                    const auto arrayType = root->find(typeName);
                                    if (arrayType != nullptr)
                                    {
                                        auto item = arrayType->clone(array);
                                        array->children_.push_back(std::move(item));
                                    }
                                }
                            }
                            break;
                        }
                    }
                    break;
                case 'r':
                    if (*text && *text++ == 'u' &&
                        *text && *text++ == 'c' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current = current->addChild(getName(text), Item::Type::Struct);
                        parentLevel_.push(level);
                    }
                    break;
                }
            }
            break;
        case 'u':
            if (*text && *text++ == 'i' &&
                *text && *text++ == 'n' &&
                *text && *text++ == 't' &&
                *text)
            {
                switch (*text++)
                {
                case '8':
                    if (*text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::UInt8);
                    }
                    break;
                case '1':
                    if (*text && *text++ == '6' &&
                        *text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::UInt16);
                    }
                    break;
                case '3':
                    if (*text && *text++ == '2' &&
                        *text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::UInt32);
                    }
                    break;
                case '6':
                    if (*text && *text++ == '4' &&
                        *text && *text++ == '_' &&
                        *text && *text++ == 't' &&
                        *text && std::isspace(*text))
                    {
                        current->addChild(getName(text), Item::Type::UInt64);
                    }
                    break;
                }
            }
            break;
        }
    }

    return root;
}

void Parser::skipSpaces(const char*& text)
{
    while (*text++ && std::isspace(*text));
}

void Parser::moveToEndOfName(const char*& text)
{
    while (*text++ && (!std::isspace(*text) && *text != ';' && *text != '>'));
}

std::string Parser::getName(const char*& text)
{
    skipSpaces(text);
    const auto begin = text;
    moveToEndOfName(text);
    return std::string(begin, text);
}

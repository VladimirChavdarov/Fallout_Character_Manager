#pragma once
#include <string>
#include <sstream>

using namespace std;

namespace util
{
    // if you want to get a substring from the beginning or to the end, put "" in the arguments instead of an actual string.
    static std::string GetSubstringBetween(const std::string& input, const std::string& start, const std::string& end)
    {
        size_t start_pos;
        size_t end_pos;
        if (start == "")
            start_pos = 0;
        else
        {
            start_pos = input.find(start);
            if (start_pos == string::npos) return ""; // start pos not found
        }
        if (end == "")
            end_pos = input.size();
        else
        {
            size_t end_pos = input.find(end);
            if (end_pos == string::npos) return ""; // end pos not found
        }

        string substr = input.substr(start_pos + start.length(), end_pos - (start_pos + start.length()));
        return substr;
    }

    static std::vector<std::string> SplitString(const std::string& input, char delimiter) {
        std::vector<std::string> tokens;
        std::istringstream tokenStream(input);
        std::string token;

        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }
}
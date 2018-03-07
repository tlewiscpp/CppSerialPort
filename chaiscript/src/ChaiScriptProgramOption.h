#ifndef CPPSERIALPORT_CHAISCRIPTPROGRAMOPTION_H
#define CPPSERIALPORT_CHAISCRIPTPROGRAMOPTION_H

#include <string>
#include <regex>
#include <algorithm>
#include <utility>
#include <sstream>

template <typename T> inline std::string toStdString(const T &t) { return dynamic_cast<std::ostringstream &>(std::ostringstream{} << t).str(); }

/*Base case to break recursion*/
inline std::string TStringFormat(const char *formatting) { return formatting; }

/*C# style String.Format()*/
template <typename First, typename ... Args>
std::string TStringFormat(const char *formatting, First &&first, Args&& ... args)
{
    /* Match exactly one opening brace, one or more numeric digit,
    * then exactly one closing brace, identifying a token */
    static const std::regex targetRegex{R"(\{[0-9]+\})"};
    std::smatch match;

    /* Copy the formatting string to a std::string, to
    * make for easier processing, which will eventually
    * be used (the .c_str() method) to pass the remainder
    * of the formatting recursively */
    std::string returnString{formatting};

    /* Copy the formatting string to another std::string, which
    * will get modified in the regex matching loop, to remove the
    * current match from the string and find the next match */
    std::string copyString{formatting};

    /* std::tuple to hold the current smallest valued brace token,
    * wrapped in a std::vector because there can be multiple brace
    * tokens with the same value. For example, in the following format string:
    * "There were {0} books found matching the title {1}, {0}/{2}",
    * this pass will save the locations of the first and second {0} */
    using TokenInformation = std::tuple<int, size_t, size_t>;
    std::vector<TokenInformation> smallestValueInformation{std::make_tuple(-1, 0, 0)};

    /*Iterate through string, finding position and lengths of all matches {x}*/
    while(std::regex_search(copyString, match, targetRegex)) {
        /*Get the absolute position of the match in the original return string*/
        size_t foundPosition{match.position() + (returnString.length() - copyString.length())};
        int regexMatchNumericValue{0};
        /*Convert the integer value between the opening and closing braces to an int to compare */
        regexMatchNumericValue = std::stoi(returnString.substr(foundPosition + 1, (foundPosition + match.str().length())));

        /*Do not allow negative numbers, although this should never get picked up the regex anyway*/
        if (regexMatchNumericValue < 0) {
            throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
        }
        /* If the numeric value in the curly brace token is smaller than
        * the current smallest (or if the smallest value has not yet been set,
        * ie it is the first match), set the corresponding smallestX variables
        * and wrap them up into a TokenInformation and add it to the std::vector */
        int smallestValue{std::get<0>(smallestValueInformation.at(0))};
        if ((smallestValue == -1) || (regexMatchNumericValue < smallestValue)) {
            smallestValueInformation.clear();
            smallestValueInformation.push_back(std::make_tuple(regexMatchNumericValue,
                                                               foundPosition,
                                                               match.str().length()));
        } else if (regexMatchNumericValue == smallestValue) {
            smallestValueInformation.push_back(std::make_tuple(regexMatchNumericValue,
                                                               foundPosition,
                                                               match.str().length()));
        }
        copyString = match.suffix();
    }
    int smallestValue{std::get<0>(smallestValueInformation.at(0))};
    if (smallestValue == -1) {
        throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
    }
    /* Set the returnString to be up to the brace token, then the string
    * representation of current argument in line (first), then the remainder
    * of the format string, effectively removing the token and replacing it
    * with the requested item in the final string, then pass it off recursively */

    std::string firstString{toStdString(first)};
    int index{0};
    for (const auto &it : smallestValueInformation) {
        size_t smallestValueLength{std::get<2>(it)};

        /* Since the original string will be modified, the adjusted position must be
        calculated for any repeated brace tokens, kept track of by index.
        The length of string representation of first mutiplied by which the iterationn count
        is added, and the length of the brace token multiplied by the iteration count is
        subtracted, resulting in the correct starting position of the current brace token */
        size_t lengthOfTokenBracesRemoved{index * smallestValueLength};
        size_t lengthOfStringAdded{index * firstString.length()};
        size_t smallestValueAdjustedPosition{std::get<1>(it) + lengthOfStringAdded - lengthOfTokenBracesRemoved};
        returnString = returnString.substr(0, smallestValueAdjustedPosition)
                       + firstString
                       + returnString.substr(smallestValueAdjustedPosition + smallestValueLength);
        index++;
    }
    return TStringFormat(returnString.c_str(), std::forward<Args>(args)...);
}

class ProgramOption {
public:
    inline ProgramOption(int shortOption, const std::string &longOption, int argumentSpecifier, const std::string &description) :
            m_shortOption{'\0'},
            m_longOption{""},
            m_argumentSpecifier{0},
            m_description{""}
    {
        this->setShortOption(shortOption);
        this->setLongOption(longOption);
        this->setArgumentSpecifier(argumentSpecifier);
        this->setDescription(description);
    }

    inline ProgramOption(option gnuOption, const std::string &description) :
            m_shortOption{'\0'},
            m_longOption{""},
            m_argumentSpecifier{0},
            m_description{""}
    {
        this->setShortOption(gnuOption.val);
        this->setLongOption(gnuOption.name);
        this->setArgumentSpecifier(gnuOption.has_arg);
        this->setDescription(description);
    }

    inline int shortOption() const { return this->m_shortOption; }
    inline std::string longOption() const { return this->m_longOption; }
    inline int argumentSpecifier() const { return this->m_argumentSpecifier; }
    inline std::string description() const { return this->m_description; }

    inline void setShortOption(int shortOption) {
        if (shortOption == 0) {
            throw std::runtime_error("ProgramOption::setShortOption(int shortOption): Short option must be a non-zero integer value");
        }
        this->m_shortOption = shortOption;
    }
    inline void setLongOption(const std::string &longOption) {
        if (longOption.length() == 0) {
            throw std::runtime_error("ProgramOption::setLongOption(const std::string &): Long option must be at least 1 intacters in length");
        }
        this->m_longOption = longOption;
    }
    inline void setArgumentSpecifier(int argumentSpecifier) {
        if ( (argumentSpecifier != no_argument) && (argumentSpecifier != optional_argument) && (argumentSpecifier != required_argument) ) {
            throw std::runtime_error(TStringFormat("ProgramOption::setArgumentSpecifier(int): Argument specifier must be either no_argument, optional_argument, or required_argument ({0}, {1}, {2})", no_argument, optional_argument, required_argument));
        }
        this->m_argumentSpecifier = argumentSpecifier;
    }
    inline void setDescription(const std::string &description) {
        if (description.length() == 0) {
            throw std::runtime_error("ProgramOption::setDescription(const std::string &str): Description must be at least 1 intacter long");
        }
        this->m_description = description;
    }
    inline option toPosixOption() const {
        if ( (this->m_longOption[0] == '-') && (this->m_longOption[1] == '-') ) {
            return option{this->m_longOption.substr(2).c_str(), this->m_argumentSpecifier, nullptr, this->m_shortOption};
        } else if (this->m_longOption[0] == '-') {
            return option{this->m_longOption.substr(1).c_str(), this->m_argumentSpecifier, nullptr, this->m_shortOption};
        } else {
            return option{this->m_longOption.c_str(), this->m_argumentSpecifier, nullptr, this->m_shortOption};
        }
    };

private:
    int m_shortOption;
    std::string m_longOption;
    int m_argumentSpecifier;
    std::string m_description;
};


#endif //CPPSERIALPORT_CHAISCRIPTPROGRAMOPTION_H

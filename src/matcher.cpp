#include "matcher.hpp"

namespace sdptransform
{
    namespace matcher
    {
        std::pair<std::string::const_iterator, std::string::const_iterator> Regex::getIteratorAfterPrefixMatch(const std::string &content) const
        {
            auto result = std::make_pair(content.cbegin(), content.cend());
            if (!prefix_.empty())
            {
                std::advance(result.first, prefix_.size());
            }
            return result;
        }

        bool Regex::doMatch(const std::string &content) const
        {
            if (!prefix_.empty() && content.find(prefix_) != 0U)
            {
                return false;
            }
            auto iterators = getIteratorAfterPrefixMatch(content);
            return std::regex_search(iterators.first, iterators.second, regex_);
        }

        Interface::Results Regex::getMatches(const std::string &content) const
        {
            auto iterators = getIteratorAfterPrefixMatch(content);
            std::smatch matches;
            std::regex_search(iterators.first, iterators.second, matches, regex_);
            Results results;
            for (unsigned i = 1U; i < matches.size(); i++)
            {
                results.push_back(convertMatchToView(matches[i]));
            }
            return results;
        }

    } // namespace matcher

} // namespace sdptransform

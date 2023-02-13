#pragma once

#include "string_view.hpp"
#include <vector>
#include <regex>
#include <memory>

namespace sdptransform
{
    namespace matcher
    {
        class Interface {
        public:
            virtual bool doMatch(const std::string& content) const = 0;
            using Results=std::vector<string_view>;
            virtual Results getMatches(const std::string& content) const = 0;
        };
        using Ptr=std::shared_ptr<Interface>;

        class Regex final : public Interface {
        public:
            Regex(std::string&& regex) : regex_(std::move(regex)) {}
            Regex(const char* prefix, std::string&& regex) : regex_(std::move(regex)), prefix_(prefix) {}
            bool doMatch(const std::string& content) const override;
            Results getMatches(const std::string& content) const override;
        private:
            std::pair<std::string::const_iterator,std::string::const_iterator> getIteratorAfterPrefixMatch(const std::string& content) const;
            std::regex  regex_;
            std::string prefix_={};
        };

        template<typename... T>
        inline std::shared_ptr<Regex> createRegex(T&&... vals) {
            return std::make_shared<Regex>(std::forward<T>(vals)...);
        }

        class WholeMatcher final : public Interface {
        public:
            bool doMatch(const std::string&) const override { return true; }
            Results getMatches(const std::string& content) const override { return {string_view(content)}; }
        };

        inline std::shared_ptr<WholeMatcher> createWholeMatcher() {
            return std::make_shared<WholeMatcher>();
        }
        
    } // namespace matcher
    
} // namespace sdptransform

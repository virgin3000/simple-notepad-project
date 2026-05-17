#include "spell_checker.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <vector>

bool spell_checker::load_dictionary(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::string word;
    while (std::getline(file, word)) {
        // Trim trailing whitespace/carriage returns
        while (!word.empty() && (word.back() == '\r' || word.back() == '\n' || word.back() == ' ')) {
            word.pop_back();
        }
        if (!word.empty()) {
            dictionary.insert(word);
        }
    }

    return !dictionary.empty();
}

std::string spell_checker::normalize(const std::string& word)
{
    std::string result;
    result.reserve(word.size());
    for (unsigned char ch : word) {
        if (std::isalpha(ch)) {
            result += static_cast<char>(std::tolower(ch));
        }
    }
    return result;
}

bool spell_checker::is_correct(const std::string& word) const
{
    const std::string normalized = normalize(word);
    if (normalized.empty()) {
        return true; // Punctuation-only tokens are not misspelled
    }
    return dictionary.count(normalized) > 0;
}

int spell_checker::edit_distance(const std::string& a, const std::string& b)
{
    const std::size_t m = a.size();
    const std::size_t n = b.size();

    // Quick length guard: skip words that are too different in length
    if (m > n + 4 || n > m + 4) {
        return std::numeric_limits<int>::max();
    }

    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (std::size_t i = 0; i <= m; ++i) {
        dp[i][0] = static_cast<int>(i);
    }
    for (std::size_t j = 0; j <= n; ++j) {
        dp[0][j] = static_cast<int>(j);
    }

    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
            }
        }
    }

    return dp[m][n];
}

std::vector<std::string> spell_checker::suggestions(const std::string& word, int max_suggestions) const
{
    const std::string normalized = normalize(word);
    if (normalized.empty()) {
        return {};
    }

    // Collect candidates within edit distance 2
    std::vector<std::pair<int, std::string>> candidates;

    for (const auto& entry : dictionary) {
        // Only consider words of similar length to keep it fast
        const std::size_t len_diff = (entry.size() > normalized.size())
            ? entry.size() - normalized.size()
            : normalized.size() - entry.size();

        if (len_diff > 3) {
            continue;
        }

        const int dist = edit_distance(normalized, entry);
        if (dist <= 2) {
            candidates.emplace_back(dist, entry);
        }

        // Stop early if we have plenty of good candidates
        if (candidates.size() > 200) {
            break;
        }
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    std::vector<std::string> result;
    result.reserve(static_cast<std::size_t>(max_suggestions));

    for (int i = 0; i < max_suggestions && i < static_cast<int>(candidates.size()); ++i) {
        result.push_back(candidates[static_cast<std::size_t>(i)].second);
    }

    return result;
}
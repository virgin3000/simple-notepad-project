#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <set>
#include <string>
#include <vector>

class spell_checker {
public:
    // Loads word list from the given file path.
    // Returns true on success, false if the file could not be opened.
    bool load_dictionary(const std::string& path);

    // Returns true if the word is spelled correctly.
    // The word is lowercased and stripped of non-alphabetic characters before lookup.
    [[nodiscard]] bool is_correct(const std::string& word) const;

    // Returns up to max_suggestions suggestions for a misspelled word.
    [[nodiscard]] std::vector<std::string> suggestions(const std::string& word,
                                                        int max_suggestions = 5) const;

    [[nodiscard]] bool is_loaded() const { return !dictionary.empty(); }

private:
    std::set<std::string> dictionary;

    // Normalize: lowercase + strip non-alpha characters
    [[nodiscard]] static std::string normalize(const std::string& word);

    // Simple edit-distance based suggestion scoring
    [[nodiscard]] static int edit_distance(const std::string& a, const std::string& b);
};

#endif // SPELL_CHECKER_H
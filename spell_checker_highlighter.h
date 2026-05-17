#ifndef SPELL_CHECKER_HIGHLIGHTER_H
#define SPELL_CHECKER_HIGHLIGHTER_H

#include "spell_checker.h"

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

class spell_checker_highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit spell_checker_highlighter(spell_checker* checker, QTextDocument* parent = nullptr);

    // Enable or disable highlighting
    void set_enabled(bool enabled);
    [[nodiscard]] bool is_enabled() const { return highlighting_enabled; }

    // Re-run highlight pass over the whole document
    void recheck_document();

protected:
    void highlightBlock(const QString& text) override;

private:
    spell_checker* checker{ nullptr };
    QTextCharFormat misspelled_format;
    bool highlighting_enabled{ true };
};

#endif // SPELL_CHECKER_HIGHLIGHTER_H
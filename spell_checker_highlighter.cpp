#include "spell_checker_highlighter.h"

#include <QColor>
#include <QRegularExpression>

spell_checker_highlighter::spell_checker_highlighter(spell_checker* checker, QTextDocument* parent)
    : QSyntaxHighlighter(parent)
    , checker(checker)
{
    misspelled_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    misspelled_format.setUnderlineColor(Qt::red);
}

void spell_checker_highlighter::set_enabled(bool enabled)
{
    highlighting_enabled = enabled;
    rehighlight();
}

void spell_checker_highlighter::recheck_document()
{
    rehighlight();
}

void spell_checker_highlighter::highlightBlock(const QString& text)
{
    if (!highlighting_enabled || !checker || !checker->is_loaded()) {
        return;
    }

    // Match sequences of alphabetic characters (words)
    static const QRegularExpression word_re(R"(\b[A-Za-z']+\b)");

    auto it = word_re.globalMatch(text);
    while (it.hasNext()) {
        const auto match = it.next();
        const std::string word = match.captured().toStdString();

        if (!checker->is_correct(word)) {
            setFormat(match.capturedStart(), match.capturedLength(), misspelled_format);
        }
    }
}

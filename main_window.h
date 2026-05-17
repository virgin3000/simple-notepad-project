#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "notepad_exception.h"
#include "spell_checker.h"
#include "spell_checker_highlighter.h"
#include "text_transform.h"

#include <QDialog>
#include <QLabel>
#include <QMainWindow>
#include <QString>
#include <QTextDocument>
#include <QTextEdit>
#include <memory>
#include <vector>

namespace Ui {
class find_replace_dialog;
class word_frequency_dialog;
}

class main_window : public QMainWindow {
    Q_OBJECT

public:
    main_window();
    ~main_window() override;

private:
    // ── Menu / toolbar setup ──────────────────────────────────────
    void setup_file_menu();
    void setup_edit_menu();
    void setup_format_menu();
    void setup_format_toolbar();
    void setup_search_menu();
    void setup_tools_menu();
    void setup_view_menu();
    void setup_status_bar();

    // ── File operations ───────────────────────────────────────────
    void open_file();
    void save_file();
    void save_file_as();
    void update_title();

    // ── Text transform ────────────────────────────────────────────
    void apply_transform(const text_transform& transform) const;

    // ── Find / Replace ────────────────────────────────────────────
    void show_find_replace_dialog();
    void find_next(const QString& term, QTextDocument::FindFlags flags = {}) const;
    void replace_current(const QString& term, const QString& replacement,
                         QTextDocument::FindFlags flags = {}) const;
    void replace_all(const QString& term, const QString& replacement,
                     QTextDocument::FindFlags flags = {}) const;

    // ── Word frequency ────────────────────────────────────────────
    void show_word_frequency();

    // ── Spell checker ─────────────────────────────────────────────
    void setup_spell_checker();
    void show_suggestions_menu(const QPoint& pos);

    // ── Status bar updates ────────────────────────────────────────
    void update_status_bar();

    // ── Zoom ──────────────────────────────────────────────────────
    void zoom_in();
    void zoom_out();
    void zoom_reset();

    // ── Widgets ───────────────────────────────────────────────────
    QTextEdit* editor{ nullptr };
    QLabel* status_words{ nullptr };
    QLabel* status_lines{ nullptr };
    QLabel* status_cursor{ nullptr }; // Optional feature 1: cursor position

    // ── State ─────────────────────────────────────────────────────
    QString current_file;
    int base_font_size{ 12 }; // For zoom reset

    // ── Text transforms ───────────────────────────────────────────
    std::vector<std::unique_ptr<text_transform>> transforms;

    // ── Find / Replace dialog ─────────────────────────────────────
    QDialog* find_replace_dlg{ nullptr };
    std::unique_ptr<Ui::find_replace_dialog> find_replace_ui;

    // ── Spell checker ─────────────────────────────────────────────
    spell_checker checker;
    spell_checker_highlighter* highlighter{ nullptr };
};

#endif // MAIN_WINDOW_H
#include "proto.h"

extern int r, c;
extern int nrows, ncols;

void print_msg(const message_t& msg) {
    c = 0;

    while (r >= nrows) {
        --r;
        scrl(1);
    }

    // Print heading
    for (const int& s : msg.header) {
        mvaddch(r, c++, (chtype)s);
    }

    c = 3;

    ++r;

    // Print description
    for (const std::vector<int>& word : msg.content) {
        while (r >= nrows) {
            --r;
            scrl(1);
        }

        if (c + word.size() >= (size_t)ncols) {
            ++r;

            while (r >= nrows) {
                --r;
                scrl(1);
            }

            c = 3;
        }

        ++c;

        for (const int& ch : word) {
            mvaddch(r, c++, ch);
        }
    }

    c = 1;

    ++r;

    // Print options
    int opt_num = 1;

    for (const std::list<std::vector<int> >& option : msg.options) {
        ++r;

        while (r >= nrows) {
            --r;
            scrl(1);
        }

        // Print option number in first column
        c = 1;

        for (const char ch : std::to_string(opt_num++)) {
            mvaddch(r, c++, ch);
        }

        // Move to second column
        c = 4;

        for (const std::vector<int>& word : option) {
            if (c + word.size() >= (size_t)ncols) {
                ++r;

                while (r >= nrows) {
                    --r;
                    scrl(1);
                }

                c = 4;
            }

            ++c;

            for (const int& ch : word) {
                mvaddch(r, c++, ch);
            }
        }

        ++r;
    }

    ++r;  // add blank line between messages
}

std::vector<int> vec_fmt(const std::wstring& str, int fmt_flags) {
    std::vector<int> vec;

    for (const wchar_t& ch : str) {
        vec.push_back((int)(ch) | fmt_flags);
    }

    return vec;
}

std::list<std::vector<int> > word_fmt(const std::wstring& str, int fmt_flags) {
    if(str.empty() || str.front() == ' ' || str.back() == ' ') {
        throw std::logic_error("text::word_fmt(): whitespace on string edge(s).");
    }
    
    std::list<std::wstring> _words;         // List of words from input string
    std::list<std::vector<int> > words;  // Converted strings for printing

    size_t p = 0;
    size_t q = 0;

    while (p < str.size()) {
        while (q < str.size() && str.at(q) != ' ') {
            ++q;
        }

        if (q - p != 0) {
            _words.push_back(str.substr(p, q - p));
        }

        p = ++q;
    }

    for (const std::wstring& w : _words) {
        std::vector<int> tmp;

        for (const wchar_t& ch : w) {
            tmp.push_back((int)(ch) | fmt_flags);
        }

        words.push_back(tmp);
    }

    return words;
}

std::wstring vec_str(std::vector<int> word) {
    std::wstring str = L"";

    for (const int& ch : word) {
        str += (char)ch;
    }

    return str;
}

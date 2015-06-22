/**
 * cstrip, by Sam Nipps (c) 2015
 * Released under the GNU GPL.
 *
 * Strip whitespace and comments from C family source files.
 */

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <functional>

typedef std::function<char (void)> current_t;
typedef std::function<void (void)> next_t;
typedef std::function<bool (void)> end_t;
typedef std::function<void (char)> out_t;

bool skip (current_t current, next_t next, end_t end, out_t out) {
    auto skipComment = [&]() {
        /*C comment*/
        if (current() == '*') {
            next();

            while (!end()) {
                if (current() == '*') {
                    next();

                    if (current() == '/') {
                        next();
                        break;
                    }

                } else
                    next();
            }

        /*C++ comment*/
        } else if (current() == '/') {
            while (current() != '\n')
                next();

        /*Not a comment*/
        } else
            out('/');
    };

    bool newline = false;

    while (true) {
        if (current() == '\n') {
            next();
            newline = true;

        } else if (isspace(current())) {
            next();

        } else if (current() == '/') {
            next();
            skipComment();

        } else
            break;
    }

    return newline;
}

bool isalnumscore (char c) {
    return isalnum(c) || c == '_';
}

bool unsafe (char prev, char next) {
    return isalnumscore(prev) && isalnumscore(next);
}

char* cstrip (const char* input, size_t length = -1) {
    if (length < 0)
        length = strlen(input);

    size_t pos = 0;

    auto current = [&]() {
        return input[pos];
    };

    auto end = [&]() {
        return pos >= length;
    };

    auto next = [&]() {
        pos++;
    };

    char prev = 0;

    auto out = [&](char c) {
        putchar(c);
        prev = c;
    };

    auto repeat = [&]() {
        out(current());
        next();
    };

    bool newline = false;

    while (!end()) {
        if (unsafe(prev, current()))
            out(newline ? '\n' : ' ');

        else if (newline && prev != 0)
            out('\n');

        /*Wait until a skippable character*/
        while (!isspace(current()) && current() != '/' && !end()) {
            /*Being inside a string makes skippable characters unskippable*/
            if (current() == '"' || current() == '\'') {
                char delim = current();
                repeat();

                while (current() != delim && !end()) {
                    if (current() == '\\')
                        repeat();

                    repeat();
                }

                repeat();

            /*Preprocessor directive*/
            } else if (current() == '#') {
                while (current() != '\n')
                    repeat();

            } else
                repeat();
        }

        /*Skip until meaningful characters*/
        newline = skip(current, next, end, out);
    }
}

std::string getFile (FILE* file, size_t* length_o) {
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file)+1;
    rewind(file);

    if (length_o)
        *length_o = length;

    char* contents = new char[length+1]();
    fread(contents, 1, length, file);

    return std::string(contents);
}

int main (int argc, char** argv) {
    if (argc != 2) {
        puts("cstrip <file>");
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");

    if (!file) {
        printf("No file named '%s'\n", argv[1]);
        return 2;
    }

    size_t length;
    std::string contents = getFile(file, &length);
    fclose(file);

    cstrip(contents.c_str(), length);
    return 0;
}

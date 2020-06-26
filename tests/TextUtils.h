#ifndef TEXTUTILS_H
#define TEXTUTILS_H

class TextEdit;

class QString;

/**
 * Takes a TextEdit and a string and setup the TextEdit content, selection and cursor position.
 *
 * The text parameter contains the text to use, and some special characters:
 *
 * - It *must* contain a '|' to indicate the cursor position.
 * - It *can* contain a '*' to indicate the selection start.
 *
 * '|' can appear before '*' in the case of an upward selection.
 */
void setupTextEditContent(TextEdit* edit, const QString& text);

/**
 * Dumps the TextEdit content in the format described in setupTextEditContent() doc. It makes it
 * easy to write tests to verify the state of the TextEdit matches expectations.
 */
QString dumpTextEditContent(TextEdit* edit);

#endif // TEXTUTILS_H

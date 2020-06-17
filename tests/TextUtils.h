#ifndef TEXTUTILS_H
#define TEXTUTILS_H

class TextEdit;

class QString;

/**
 * Take a TextEdit and a text and setup the TextEdit text and selection
 *
 * The text parameter contains the text to use, but it *must* contain also a '{' to indicate the
 * selection start. It *can* also contain a '}' to indicate the selection end.
 *
 * '{' can appear *after* '}' in case of an upward selection.
 */
void setupTextEditContent(TextEdit* edit, const QString& text);

#endif // TEXTUTILS_H

#ifndef MOVELINEEXTENSION_H
#define MOVELINEEXTENSION_H

#include "TextEdit.h"

class MoveLinesExtension : public TextEditExtension {
    Q_OBJECT
public:
    explicit MoveLinesExtension(TextEdit* textEdit);

    bool keyPress(QKeyEvent* event) override;

private:
    void moveSelectedLines(int delta);
};

#endif // MOVELINEEXTENSION_H

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QSpinBox>

#include "Settings.h"

static const char PROJECT_URL[] = "https://github.com/agateau/nanonote";

SettingsDialog::SettingsDialog(Settings* settings, QWidget* parent)
        : QDialog(parent), ui(new Ui::SettingsDialog), mSettings(settings) {
    ui->setupUi(this);
    setupAboutTab();
    ui->tabWidget->setCurrentIndex(0);

    updateFontFromSettings();

    connect(mSettings, &Settings::fontChanged, this, &SettingsDialog::updateFontFromSettings);

    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, mSettings, &Settings::setFont);
    connect(ui->fontSizeSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            [this](int value) {
                auto font = mSettings->font();
                font.setPointSize(value);
                mSettings->setFont(font);
            });
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}

void SettingsDialog::setupAboutTab() {
    auto projectLink = QString("<a href='%1'>%1</a>").arg(PROJECT_URL);
    auto noteLink = QString("<a href='file:%1'>%1</a>").arg(Settings::notePath());
    auto text = tr("<h2>Nanonote %1</h2>"
                   "<p>A minimalist note taking application.<br>"
                   "%2</p>"
                   "<p>Your notes are stored in %3.</p>",
                   "%1=version %2=projectLink %3=noteLink")
                    .arg(qApp->applicationVersion(), projectLink, noteLink);

    ui->aboutLabel->setText(text);
}

void SettingsDialog::updateFontFromSettings() {
    ui->fontComboBox->setCurrentFont(mSettings->font());
    ui->fontSizeSpinBox->setValue(mSettings->font().pointSize());
}

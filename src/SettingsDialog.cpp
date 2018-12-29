#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QSpinBox>

#include "Settings.h"

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , mSettings(settings)
{
    ui->setupUi(this);
    setupAboutTab();
    ui->tabWidget->setCurrentIndex(0);

    updateFontFromSettings();

    connect(mSettings, &Settings::fontChanged, this, &SettingsDialog::updateFontFromSettings);

    connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, mSettings, &Settings::setFont);
    connect(ui->fontSizeSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [this](int value) {
       auto font = mSettings->font();
       font.setPointSize(value);
       mSettings->setFont(font);
    });
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setupAboutTab()
{
    QString text = tr(R"(
                   <h2>Nanonote %1</h2>
                   <p>A minimalist note taking application.<br>
                   <a href="https://github.com/agateau/nanonote">https://github.com/agateau/nanonote</a></p>
                   <p>Your notes are stored in <a href="file:%2">%2</a>.</p>
                   )").arg(qApp->applicationVersion(), Settings::notePath());

    ui->aboutLabel->setText(text);
}

void SettingsDialog::updateFontFromSettings()
{
    ui->fontComboBox->setCurrentFont(mSettings->font());
    ui->fontSizeSpinBox->setValue(mSettings->font().pointSize());
}

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QSpinBox>

#include "Settings.h"

static constexpr char PROJECT_URL[] = "https://github.com/agateau/nanonote/";
static constexpr char SUPPORT_URL[] = "https://agateau.com/support/";

SettingsDialog::SettingsDialog(Settings* settings, QWidget* parent)
        : QDialog(parent), ui(new Ui::SettingsDialog), mSettings(settings) {
    ui->setupUi(this);
    setupConfigTab();
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

void SettingsDialog::setupConfigTab() {
    auto noteLink = QString("<html><a href='file:%1'>%1</a></html>").arg(Settings::notePath());
    ui->noteLocationLabel->setText(noteLink);
}

void SettingsDialog::setupAboutTab() {
    // Do not use C++ raw strings here, the lupdate shipped with Ubuntu 18.04 does not understand
    // them
    auto text = tr("<h2>Nanonote %1</h2>\n"
                   "<p>A minimalist note taking application.<br>\n"
                   "<a href='%2'>%2</a></p>",
                   "%1: version, %2: project url")
                    .arg(qApp->applicationVersion(), PROJECT_URL);
    ui->aboutLabel->setText(text);

    text = tr("<p>Hi,</p>\n"
              "<p>I hope you enjoy Nanonote!</p>\n"
              "<p>If you do, it would be lovely if you could <a href='%1'>support my work</a> on "
              "free and open source software.</p>\n"
              "<p align=\"right\">― Aurélien</p>",
              "%1: support url")
               .arg(SUPPORT_URL);
    auto font = ui->supportLabel->font();
    font.setItalic(true);
    ui->supportLabel->setFont(font);
    ui->supportLabel->setText(text);
}

void SettingsDialog::updateFontFromSettings() {
    ui->fontComboBox->setCurrentFont(mSettings->font());
    ui->fontSizeSpinBox->setValue(mSettings->font().pointSize());
}

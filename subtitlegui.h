#pragma once

#include <QtWidgets/QWidget>
#include "ui_subtitlegui.h"
#include "SettingClass.h"
#include "displayText.h"
#include "subtitleUpdater.h"
#include "historyPage.h"

class SubtitleGUI : public QWidget
{
    Q_OBJECT

public:
    SubtitleGUI(QWidget *parent = nullptr);
    ~SubtitleGUI();

private:
    Ui::SubtitleGUIClass ui;
    SettingClass* settingPage = nullptr;
    historyPage* recordPage = nullptr;
    displayText* textOutput;
    subtitleUpdater* textUpdater;
    bool recognitionIsRunning = false;

private slots:
    void on_historyButton_clicked();
    void on_settingButton_clicked();
    void on_startButton_clicked();
    void on_modeSwitch_clicked(bool);
    void stop_recognition();

public slots:
    void saveNewAPIConfig(std::string key, std::string region, std::string recognitionLanguage, std::string translationLanguage, bool recordState, int engineMode, bool showSingleLanguage);
};

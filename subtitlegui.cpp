#include "stdafx.h"
#include "subtitlegui.h"
#include "AzureSpeechAPI.h"

SubtitleGUI::SubtitleGUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.historyButton->setIcon(QIcon("icon/speech-to-text-icon.png"));
    ui.historyButton->setToolTip("History");
    ui.settingButton->setIcon(QIcon("icon/setting-icon.png"));
    ui.settingButton->setToolTip("Setting");
    ui.startButton->setText("");
    ui.startButton->setIcon(QIcon("icon/shutdown-icon.png"));
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setColor(QColor(0, 0, 0, 255 * 0.3));
    effect->setBlurRadius(5);
    effect->setOffset(0);
    ui.startButton->setGraphicsEffect(effect);
    AzureSpeechAPI* speech = AzureSpeechAPI::getInstance();
    bool engineMode;
    if (speech->getEngineMode() == 1) {
        ui.modeSwitch->setText("Translation Mode");
        engineMode = true;
    }
    else {
        ui.modeSwitch->setText("Recognition Mode");
        engineMode = false;
    }
    ui.modeSwitch->setChecked(engineMode);
}

SubtitleGUI::~SubtitleGUI()
{}

void SubtitleGUI::on_historyButton_clicked() {
    recordPage = new historyPage();
    recordPage->show();
}

void SubtitleGUI::on_settingButton_clicked() {
    if (settingPage == nullptr) {
        settingPage = new SettingClass();
        connect(settingPage, SIGNAL(updateAPIConfig(std::string, std::string, std::string, std::string, bool, int, bool))
            , this, SLOT(saveNewAPIConfig(std::string, std::string, std::string, std::string, bool, int, bool)));
    }
    settingPage->show();
}

void SubtitleGUI::stop_recognition() {
    textUpdater->close();
}

void SubtitleGUI::saveNewAPIConfig(std::string key, std::string region, std::string recognitionLanguage, std::string translationLanguage, bool recordState, int engineMode, bool showSingleLanguage) {
    bool mode;
    if (engineMode == 1) {
        ui.modeSwitch->setText("Translation Mode");
        mode = true;
    }
    else {
        ui.modeSwitch->setText("Recognition Mode");
        mode = false;
    }
    ui.modeSwitch->setChecked(mode);
    textUpdater->saveConfig(key, region, recognitionLanguage, translationLanguage, recordState, engineMode, showSingleLanguage);
    if (recognitionIsRunning) {
        int rowNo = (engineMode == 0) ? 1 : 2;
        if (rowNo == 2)rowNo = (showSingleLanguage) ? 1 : 2;
        textOutput->updateLabel(rowNo);
        textUpdater->close();
        QThread* thread = new QThread;
        textUpdater = new subtitleUpdater();
        textUpdater->moveToThread(thread);
        connect(textUpdater, SIGNAL(updateRecognition(QString)), textOutput, SLOT(setRecognition(QString)));
        connect(textUpdater, SIGNAL(updateTranslation(QString)), textOutput, SLOT(setTranslation(QString)));
        connect(thread, SIGNAL(started()), textUpdater, SLOT(process()));
        connect(textUpdater, SIGNAL(finished()), thread, SLOT(quit()));
        connect(textUpdater, SIGNAL(finished()), textUpdater, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}

void SubtitleGUI::on_startButton_clicked() {
    if (recognitionIsRunning) {
        ui.startButton->setIcon(QIcon("icon/shutdown-icon.png"));
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
        effect->setColor(QColor(0, 0, 0, 255 * 0.3));
        effect->setBlurRadius(5);
        effect->setOffset(0);
        ui.startButton->setGraphicsEffect(effect);
        emit stop_recognition();
        delete textOutput;
        recognitionIsRunning = !recognitionIsRunning;
        return;
    }
    recognitionIsRunning = !recognitionIsRunning;
    ui.startButton->setIcon(QIcon("icon/on-green.png"));
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setColor(QColor(0, 0, 0, 255 * 0.4));
    effect->setBlurRadius(20);
    effect->setOffset(0);
    ui.startButton->setGraphicsEffect(effect);

    QSize size = qApp->screens()[0]->size();
    int x = size.width() * 0.05;
    int y = size.height() * 0.85;
    int width = size.width() * 0.9;
    int height = size.height() * 0.5;

    AzureSpeechAPI* speech = AzureSpeechAPI::getInstance();
    int rowNo = (speech->getEngineMode() == 0) ? 1 : 2;
    if (rowNo == 2)rowNo = (speech->getShowSingleLanguage()) ? 1 : 2;
    textOutput = new displayText(rowNo);
    textOutput->setGeometry(x, y, width, height);
    textOutput->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    textOutput->setAttribute(Qt::WA_TranslucentBackground, true);
    textOutput->show();

    QThread* thread = new QThread;
    textUpdater = new subtitleUpdater();
    textUpdater->moveToThread(thread);
    connect(textUpdater, SIGNAL(updateRecognition(QString)), textOutput, SLOT(setRecognition(QString)));
    connect(textUpdater, SIGNAL(updateTranslation(QString)), textOutput, SLOT(setTranslation(QString)));
    connect(thread, SIGNAL(started()), textUpdater, SLOT(process()));
    connect(textUpdater, SIGNAL(finished()), thread, SLOT(quit()));
    connect(textUpdater, SIGNAL(finished()), textUpdater, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void SubtitleGUI::on_modeSwitch_clicked(bool state) {
    int mode;
    if (state) {
        ui.modeSwitch->setText("Translation Mode");
        mode = 1;
    }
    else {
        ui.modeSwitch->setText("Recognition Mode");
        mode = 0;
    }
    textUpdater->updateEngineMode(mode);
    if (settingPage != nullptr)settingPage->updateEngineMode(mode);
    if (recognitionIsRunning) {
        int rowNo = (mode == 0) ? 1 : 2;
        textOutput->updateLabel(rowNo);
        textUpdater->close();
        QThread* thread = new QThread;
        textUpdater = new subtitleUpdater();
        textUpdater->moveToThread(thread);
        connect(textUpdater, SIGNAL(updateRecognition(QString)), textOutput, SLOT(setRecognition(QString)));
        connect(textUpdater, SIGNAL(updateTranslation(QString)), textOutput, SLOT(setTranslation(QString)));
        connect(thread, SIGNAL(started()), textUpdater, SLOT(process()));
        connect(textUpdater, SIGNAL(finished()), thread, SLOT(quit()));
        connect(textUpdater, SIGNAL(finished()), textUpdater, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}
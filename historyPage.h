#pragma once
#include <QWidget>
#include "ui_historyPage.h"
#include "DatabaseAPIClass.h"
#include "deleteRecordPage.h"

class historyPage : public QWidget
{
	Q_OBJECT

public:
	historyPage(QWidget *parent = nullptr);
	~historyPage();
	void closeEvent(QCloseEvent* event);

private:
	Ui::historyPageClass ui;
	std::vector<std::vector<std::string>> translationQueryResult;
	std::vector<std::vector<std::string>> recognitionQueryResult;
	std::string recognition;
	std::string fromLanguage;
	std::string toLanguage;
	int startTime;
	int endTime;
	deleteRecordPage* deletionPage;
	void loadRecognitionRecord();
	void loadTranslationRecord();

signals:
	void refreshTextBrowser();

private slots:
	void on_languageComboBox_currentIndexChanged(QString);
	void on_recognitionCheckBox_stateChanged(int);
	void on_translationCheckBox_stateChanged(int);
	void on_startTimeComboBox_currentIndexChanged(int);
	void on_endTimeComboBox_currentIndexChanged(int);
	void update_textBrowser();
	void on_downloadButton_clicked();
	void on_deleteButton_clicked();
};

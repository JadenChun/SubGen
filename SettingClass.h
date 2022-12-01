#pragma once

#include <QWidget>
#include "ui_SettingClass.h"

class SettingClass : public QWidget
{
	Q_OBJECT

public:
	SettingClass(QWidget *parent = nullptr);
	~SettingClass();

	void closeEvent(QCloseEvent* event);
	void updateEngineMode(int mode);

private:
	Ui::SettingClassClass ui;
	QString key;
	QString region;
	QString detectionLanguage;
	QString translationLanguage;
	bool recordState;
	int engineMode;
	bool showSingleLanguage;

private slots:
	void on_saveButton_clicked();
	void checkInputChanges();
	void on_engineModeComboBox_currentIndexChanged(int);

signals:
	void updateAPIConfig(std::string key, std::string region, std::string recognitionLanguage, std::string translationLanguage, bool recordState, int engineMode, bool showSingleLanguage);
	
};

#include "stdafx.h"
#include "SettingClass.h"
#include "AzureSpeechAPI.h"
#include <QCloseEvent>
#include "DatabaseAPIClass.h"

SettingClass::SettingClass(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	AzureSpeechAPI *API = API->getInstance();
	key = QString::fromStdString(API->getKey());
	region = QString::fromStdString(API->getRegion());
	std::map<std::string, language> supportedLanguages = API->getSupportedLanguage();
	ui.APIKeyInput->setText(key);
	ui.APIRegionInput->setText(region);
	std::vector<std::string> vec;
	for (auto& index : supportedLanguages) {
		QString languageName = QString::fromStdString(index.first);
		ui.detectionLanguageComboBox->addItem(languageName);
		if (std::find(vec.begin(), vec.end(), index.second.name) == vec.end()){
			vec.push_back(index.second.name);
			ui.translationLanguageComboBox->addItem(QString::fromStdString(index.second.name));
		}
		detectionLanguage = (index.second.recognitionCode == API->getDetectionLanguageCode()) ? QString::fromStdString(index.first) : detectionLanguage;
		translationLanguage = (index.second.translationCode == API->getTranslationLanguageCode()) ? QString::fromStdString(index.second.name) : translationLanguage;
	}
	ui.detectionLanguageComboBox->setCurrentText(detectionLanguage);
	ui.translationLanguageComboBox->setCurrentText(translationLanguage);
	recordState = API->getSavingResultState();
	ui.saveRecordCheckBox->setChecked(recordState);
	engineMode = API->getEngineMode();
	ui.engineModeComboBox->setCurrentIndex(engineMode);
	if (engineMode == 0) {
		ui.translationLanguageComboBox->setEnabled(false);
		ui.showSingleLanguageCheckBox->setEnabled(false);
	}
	showSingleLanguage = API->getShowSingleLanguage();
	ui.showSingleLanguageCheckBox->setChecked(showSingleLanguage);
	connect(ui.APIKeyInput, SIGNAL(textChanged(QString)), this, SLOT(checkInputChanges()));
	connect(ui.APIRegionInput, SIGNAL(textChanged(QString)), this, SLOT(checkInputChanges()));
	connect(ui.detectionLanguageComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(checkInputChanges()));
	connect(ui.translationLanguageComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(checkInputChanges()));
	connect(ui.saveRecordCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkInputChanges()));
	connect(ui.engineModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkInputChanges()));
	connect(ui.showSingleLanguageCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkInputChanges()));
}

SettingClass::~SettingClass()
{}

void SettingClass::on_saveButton_clicked() {
	key = ui.APIKeyInput->text();
	region = ui.APIRegionInput->text();
	AzureSpeechAPI* API = API->getInstance();
	std::map<std::string, language> supportedLanguages = API->getSupportedLanguage();
	std::string recognitionLanguageCode;
	std::string translationLanguageCode;
	for (auto& index : supportedLanguages) {
		detectionLanguage = ui.detectionLanguageComboBox->currentText();
		translationLanguage = ui.translationLanguageComboBox->currentText();
		recognitionLanguageCode = (index.first == detectionLanguage.toStdString()) ? index.second.recognitionCode : recognitionLanguageCode;
		translationLanguageCode = (index.second.name == translationLanguage.toStdString()) ? index.second.translationCode : translationLanguageCode;
	}
	recordState = ui.saveRecordCheckBox->isChecked();
	std::string recordStateStr = (recordState) ? "1" : "0";
	engineMode = ui.engineModeComboBox->currentIndex();
	showSingleLanguage = ui.showSingleLanguageCheckBox->isChecked();
	std::string singleLanguageStateStr = (showSingleLanguage) ? "1" : "0";
	DatabaseAPI* dbAPI = new DatabaseAPI();
	dbAPI->save_engine_state(key.toStdString(), region.toStdString(), recognitionLanguageCode, translationLanguageCode, recordStateStr,to_string(engineMode),singleLanguageStateStr);
	emit updateAPIConfig(key.toStdString(), region.toStdString(),recognitionLanguageCode,translationLanguageCode, recordState, engineMode, showSingleLanguage);
	QMessageBox::information(this, "Save Setting", "The setting has been saved.");
	this->hide();
}

void SettingClass::closeEvent(QCloseEvent* event) {
	this->hide();
}

void SettingClass::updateEngineMode(int mode)
{
	ui.engineModeComboBox->setCurrentIndex(mode);
}

void SettingClass::checkInputChanges()
{
	if (ui.APIKeyInput->text() != key || 
		ui.APIRegionInput->text() != region ||
		ui.detectionLanguageComboBox->currentText() != detectionLanguage || 
		ui.translationLanguageComboBox->currentText() != translationLanguage || 
		ui.saveRecordCheckBox->isChecked() != recordState ||
		ui.engineModeComboBox->currentIndex() != engineMode ||
		ui.showSingleLanguageCheckBox->isChecked() != showSingleLanguage
		) 
	{
		ui.saveButton -> setEnabled(true);
	}
	else {
		ui.saveButton->setEnabled(false);
	}
}

void SettingClass::on_engineModeComboBox_currentIndexChanged(int mode)
{
	if (mode == 0) {
		ui.translationLanguageComboBox->setEnabled(false);
		ui.showSingleLanguageCheckBox->setEnabled(false);
	}
	if (mode == 1) {
		ui.translationLanguageComboBox->setEnabled(true);
		ui.showSingleLanguageCheckBox->setEnabled(true);
	}
}

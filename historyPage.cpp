#include "stdafx.h"
#include "historyPage.h"

historyPage::historyPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	loadRecognitionRecord();
	loadTranslationRecord();
	ui.startTimeComboBox->setCurrentIndex(0);
	int lastIndex = ui.endTimeComboBox->count() - 1;
	ui.endTimeComboBox->setCurrentIndex(lastIndex);
	connect(this, SIGNAL(refreshTextBrowser()), this, SLOT(update_textBrowser()));
	ui.deleteButton->setText("");
	ui.deleteButton->setIcon(QIcon("icon/trash-icon.png"));
}

historyPage::~historyPage()
{}

void historyPage::loadRecognitionRecord() {
	DatabaseAPI* dbAPI = new DatabaseAPI();
	dbAPI->read_recognition_data();
	dbMemory* query = dbMemory::getInstance();
	recognitionQueryResult = query->queryResult;
	std::vector<QString> temp;
	for (int x = 0; x < recognitionQueryResult.size(); x++) {
		std::vector row = recognitionQueryResult.at(x);
		QString language = QString::fromStdString(row.at(2));
		if (std::find(temp.begin(), temp.end(), language) == temp.end()) {
			ui.languageComboBox->addItem(language, QVariant(0));
			temp.push_back(language);
		}
		time_t time = stoi(row.at(1));
		tm* gmtm = gmtime(&time);
		std::string dt = asctime(gmtm);
		std::string display_time = dt.substr(0, dt.find("\n"));
		ui.startTimeComboBox->addItem(QString::fromStdString(display_time), QVariant(time));
		ui.endTimeComboBox->addItem(QString::fromStdString(display_time), QVariant(time));
	}
}

void historyPage::loadTranslationRecord() {
	DatabaseAPI* dbAPI = new DatabaseAPI();
	dbAPI->read_translation_data();
	dbMemory* query = dbMemory::getInstance();
	translationQueryResult = query->queryResult;
	std::vector<QString> temp;
	for (int x = 0; x < translationQueryResult.size(); x++) {
		std::vector row = translationQueryResult.at(x);
		QString language = QString::fromStdString(row.at(2)) + "->" + QString::fromStdString(row.at(3));
		if (std::find(temp.begin(), temp.end(), language) == temp.end()) {
			ui.languageComboBox->addItem(language, QVariant(1));
			temp.push_back(language);
		}
		time_t time = stoi(row.at(1));
		tm* gmtm = gmtime(&time);
		std::string dt = asctime(gmtm);
		std::string display_time = dt.substr(0, dt.find("\n"));
		ui.startTimeComboBox->addItem(QString::fromStdString(display_time), QVariant(time));
		ui.endTimeComboBox->addItem(QString::fromStdString(display_time), QVariant(time));
	}
}

void historyPage::on_languageComboBox_currentIndexChanged(QString selected) {
	if (ui.languageComboBox->itemData(ui.languageComboBox->currentIndex()) == 0) {
		recognition = selected.toStdString();
	}
	if (ui.languageComboBox->itemData(ui.languageComboBox->currentIndex()) == 1) {
		std::string languages = selected.toStdString();
		fromLanguage = languages.substr(0, languages.find("->"));
		toLanguage = languages.substr(languages.find("->") + 2, languages.length());
	}
	emit update_textBrowser();
}

void historyPage::on_recognitionCheckBox_stateChanged(int checkState)
{
	if (checkState == 2 || ui.translationCheckBox->isChecked()) {
		ui.downloadButton->setEnabled(true);
	}
	else {
		ui.downloadButton->setEnabled(false);
	}
}

void historyPage::on_translationCheckBox_stateChanged(int checkState)
{
	if (checkState == 2 || ui.recognitionCheckBox->isChecked()) {
		ui.downloadButton->setEnabled(true);
	}
	else {
		ui.downloadButton->setEnabled(false);
	}
}

void historyPage::on_startTimeComboBox_currentIndexChanged(int index)
{
	startTime = ui.startTimeComboBox->currentData().toInt();
	emit update_textBrowser();
}

void historyPage::on_endTimeComboBox_currentIndexChanged(int index)
{
	endTime = ui.endTimeComboBox->currentData().toInt();
	emit update_textBrowser();
}

void historyPage::update_textBrowser() {
	ui.recognitionTextBrowser->clear();
	ui.translationTextBrowser->clear();
	if (ui.languageComboBox->itemData(ui.languageComboBox->currentIndex()) == 0) {
		for (int x = 0; x < recognitionQueryResult.size(); x++) {
			std::vector row = recognitionQueryResult.at(x);
			if (row.at(2) == recognition) {
				int time = stoi(row.at(1));
				if (time >= startTime && time <= endTime) {
					ui.recognitionCheckBox->setEnabled(true);
					ui.recognitionTextBrowser->insertPlainText(" " + QString::fromStdString(row.at(3)));
					ui.translationCheckBox->setEnabled(false);
				}
			}
		}
	}
	if (ui.languageComboBox->itemData(ui.languageComboBox->currentIndex()) == 1) {
		for (int x = 0; x < translationQueryResult.size(); x++) {
			std::vector row = translationQueryResult.at(x);
			if (row.at(2) == fromLanguage && row.at(3) == toLanguage) {
				int time = stoi(row.at(1));
				if (time >= startTime && time <= endTime) {
					ui.recognitionCheckBox->setEnabled(true);
					ui.recognitionTextBrowser->insertPlainText(" "+QString::fromStdString(row.at(4)));
					ui.translationCheckBox->setEnabled(true);
					ui.translationTextBrowser->insertPlainText(" "+QString::fromStdString(row.at(5)));
				}
			}
		}
	}
}

void historyPage::on_downloadButton_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, "Select a Directory","/Downloads", QFileDialog::ShowDirsOnly);
	if (ui.recognitionCheckBox->isChecked()) {
		QFile file(path + "/Recognition Record.txt");
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, "Error", "File open failed!");
		}
		QTextStream out(&file);
		QString text = ui.recognitionTextBrowser->toPlainText();
		out << text;
		file.flush();
		file.close();
	}
	if (ui.translationCheckBox->isChecked()) {
		QFile file(path + "/Translation Record.txt");
		if (!file.open(QFile::WriteOnly | QFile::Text)) {
			QMessageBox::warning(this, "Error", "File open failed!");
		}
		QTextStream out(&file);
		QString text = ui.translationTextBrowser->toPlainText();
		out << text;
		file.flush();
		file.close();
	}
	QMessageBox::information(this, "Done", "The record has been downloaded.");
}

void historyPage::on_deleteButton_clicked() {
	deletionPage = new deleteRecordPage();
	deletionPage->show();
	this->close();
}

void historyPage::closeEvent(QCloseEvent* event) {
	delete this;
}
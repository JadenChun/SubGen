#include "deleteRecordPage.h"

deleteRecordPage::deleteRecordPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	emit loadDatabase();
}

deleteRecordPage::~deleteRecordPage()
{}

void deleteRecordPage::loadDatabase() {
	DatabaseAPI* dbAPI = new DatabaseAPI();
	dbAPI->read_translation_data();
	dbMemory *translationQuery = dbMemory::getInstance();
	queryCopy = translationQuery->queryResult;
	int rowNum = queryCopy.size();
	for (int row = 0; row < rowNum;row++) {
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		QString recognitionText = QString::fromStdString(queryCopy.at(row).at(4));
		QString translationText = QString::fromStdString(queryCopy.at(row).at(5));
		QTableWidgetItem *recognitionTableItem = new QTableWidgetItem(recognitionText);
		QTableWidgetItem *translationTableItem = new QTableWidgetItem(translationText);
		ui.tableWidget->setCellWidget(row, 0, generateCheckBox(1));
		ui.tableWidget->setItem(row, 1, recognitionTableItem);
		ui.tableWidget->setItem(row, 2, translationTableItem);
	}
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	QHeaderView* headerView = ui.tableWidget->horizontalHeader();
	headerView->setSectionResizeMode(0, QHeaderView::ResizeToContents);

	dbAPI->read_recognition_data();
	dbMemory* recognitionQuery = dbMemory::getInstance();
	queryCopy = recognitionQuery->queryResult;
	rowNum = queryCopy.size();
	for (int row = 0; row < rowNum; row++) {
		ui.tableWidget_2->insertRow(ui.tableWidget_2->rowCount());
		QString recognitionText = QString::fromStdString(queryCopy.at(row).at(3));
		QTableWidgetItem* recognitionTableItem = new QTableWidgetItem(recognitionText);
		ui.tableWidget_2->setCellWidget(row, 0, generateCheckBox(0));
		ui.tableWidget_2->setItem(row, 1, recognitionTableItem);
	}
	ui.tableWidget_2->horizontalHeader()->setStretchLastSection(true);
	headerView = ui.tableWidget->horizontalHeader();
	headerView->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void deleteRecordPage::on_deleteButton_clicked()
{
	if (ui.tabWidget->currentIndex() == 0) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Delete selected record", "Are you sure you want to delete the data?",
			QMessageBox::Yes | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			std::vector<int> IDs;
			for (int x = 0; x < recognitionCheckBoxList.size(); x++) {
				if (recognitionCheckBoxList.at(x)->isChecked()) {
					IDs.push_back(x + 1);
				}
			}
			DatabaseAPI* db = new DatabaseAPI();
			db->delete_recognition_data(IDs);
			this->close();
		}
	}
	if (ui.tabWidget->currentIndex() == 1) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Delete selected record", "Are you sure you want to delete the data?",
			QMessageBox::Yes | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			std::vector<int> IDs;
			for (int x = 0; x < translationCheckBoxList.size(); x++) {
				if (translationCheckBoxList.at(x)->isChecked()) {
					IDs.push_back(x + 1);
				}
			}
			DatabaseAPI* db = new DatabaseAPI();
			db->delete_translation_data(IDs);
			this->close();
		}
	}
	
}

void deleteRecordPage::on_deleteAllButton_clicked()
{
	if (ui.tabWidget->currentIndex() == 0) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Delete all record", "Are you sure you want to delete all data?",
			QMessageBox::Yes | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			std::vector<int> IDs;
			for (int x = 0; x < recognitionCheckBoxList.size(); x++) {
				IDs.push_back(x + 1);
			}
			DatabaseAPI* db = new DatabaseAPI();
			db->delete_recognition_data(IDs);
			this->close();
		}
	}
	if (ui.tabWidget->currentIndex() == 1) {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Delete all record", "Are you sure you want to delete all data?",
			QMessageBox::Yes | QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			std::vector<int> IDs;
			for (int x = 0; x < translationCheckBoxList.size(); x++) {
				IDs.push_back(x + 1);
			}
			DatabaseAPI* db = new DatabaseAPI();
			db->delete_translation_data(IDs);
			this->close();
		}
	}
}

void deleteRecordPage::checkBoxClicked()
{
	bool flag = false;
	for (int x = 0; x < recognitionCheckBoxList.size(); x++) {
		if (recognitionCheckBoxList.at(x)->isChecked()) {
			flag = true;
		}
	}
	for (int x = 0; x < translationCheckBoxList.size(); x++) {
		if (translationCheckBoxList.at(x)->isChecked()) {
			flag = true;
		}
	}
	if (flag) {
		ui.deleteButton->setEnabled(true);
	}
	else {
		ui.deleteButton->setEnabled(false);
	}
}

QCheckBox* deleteRecordPage::generateCheckBox(int mode)
{
	if (mode == 0) {
		QCheckBox* checkBox = new QCheckBox();
		connect(checkBox, SIGNAL(clicked()), this, SLOT(checkBoxClicked()));
		recognitionCheckBoxList.push_back(checkBox);
		return checkBox;
	}
	else {
		QCheckBox* checkBox = new QCheckBox();
		connect(checkBox, SIGNAL(clicked()), this, SLOT(checkBoxClicked()));
		translationCheckBoxList.push_back(checkBox);
		return checkBox;
	}
}
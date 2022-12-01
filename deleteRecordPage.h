#pragma once

#include <QWidget>
#include "ui_deleteRecordPage.h"
#include "DatabaseAPIClass.h"
#include <QtWidgets>

class deleteRecordPage : public QWidget
{
	Q_OBJECT

public:
	deleteRecordPage(QWidget *parent = nullptr);
	~deleteRecordPage();

private:
	Ui::deleteRecordPageClass ui;
	std::vector<std::vector<std::string>> queryCopy;
	QCheckBox* generateCheckBox(int mode);
	std::vector<QCheckBox*> translationCheckBoxList;
	std::vector<QCheckBox*> recognitionCheckBoxList;

private slots:
	void loadDatabase();
	void on_deleteButton_clicked();
	void on_deleteAllButton_clicked();
	void checkBoxClicked();
};

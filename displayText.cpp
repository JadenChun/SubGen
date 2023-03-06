#pragma once
#include "stdafx.h"
#include "displayText.h"

displayText::displayText(int row, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Subtitle");
	QSize size = qApp->screens()[0]->size();
	QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.widget->layout());
	for (int i = 0; i < row; i++) {
		QLabel* label = new QLabel(ui.widget);
		label->setWordWrap(true);
		label->setStyleSheet("background-color: rgba(0,0,0,0); color: white; font-size: 30px;");
		layout->insertWidget(-1, label);
		labelList.push_back(label);
	}
}

displayText::~displayText()
{}

void displayText::updateLabel(int row)
{
	if (row > labelList.size()) {
		QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui.widget->layout());
		QLabel* label = new QLabel(ui.widget);
		label->setWordWrap(true);
		label->setStyleSheet("background-color: rgba(0,0,0,0); color: white; font-size: 30px;");
		layout->insertWidget(-1, label);
		labelList.push_back(label);
	}
	if (row < labelList.size()) {
		delete labelList.back();
		labelList.pop_back();
	}
}

void displayText::setTranslation(QString string)
{
	labelList.back()->setText(string);
}

void displayText::setRecognition(QString string) {
	labelList.front()->setText(string);
}

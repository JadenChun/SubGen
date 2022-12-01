#pragma once

#include <QWidget>
#include "ui_displayText.h"

class displayText : public QWidget
{
	Q_OBJECT

public:
	displayText(int row, QWidget *parent = nullptr);
	~displayText();

	Ui::displayTextClass ui;
    void updateLabel(int row);

private:
    QPoint oldPos;
    std::vector<QLabel*> labelList;

protected:
    void mousePressEvent(QMouseEvent* evt)
    {
        oldPos = evt->globalPos();
    }

    void mouseMoveEvent(QMouseEvent* evt)
    {
        const QPoint delta = evt->globalPos() - oldPos;
        move(x() + delta.x(), y() + delta.y());
        oldPos = evt->globalPos();
    }
    
public slots:
    void setRecognition(QString string);
    void setTranslation(QString string);
};

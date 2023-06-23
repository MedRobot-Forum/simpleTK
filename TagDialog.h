#pragma once
#include <QObject>
#include "ui_TagDialog.h"

class TagDialog final : public QDialog
{
	Q_OBJECT

public:
	explicit TagDialog(QDialog* parent, QString filePath);
	~TagDialog();


private:
	Ui::Dialog ui;
	QString path;



private:
	

private slots:
	//void okPressed();
	//void cancelPressed();
	//void findText() const;
};



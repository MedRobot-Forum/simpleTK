#pragma once
#include <QObject>
#include "ui_TagDialog.h"
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmdata/dcvr.h"
#include "dcmtk/dcmdata/dctag.h"
#include <dcmtk/dcmdata/dcpixseq.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcpxitem.h>
#include "DcmWidgetElement.h"
#include "DcmWidgetElement.h"
class TagDialog final : public QDialog
{
	Q_OBJECT

public:
	explicit TagDialog(QDialog* parent, QString filePath);
	~TagDialog();
	DcmWidgetElement getElement() const;
	//void populate() const;
	void initTag();

private:
	Ui::Dialog ui;
	QString path;
	DcmFileFormat file;
	std::vector<DcmWidgetElement> elements;
	std::vector<DcmWidgetElement> nestedElements;
	unsigned long globalIndex = 0;
	int depthRE = 0;
	QModelIndex scrollPosition;


private:
	DcmWidgetElement element;
	void clearTable() const;
	void extractData(DcmFileFormat file);
	void insertInTable(DcmElement* element);
	static void alertFailed(const std::string& message);
	static void indent(DcmWidgetElement& element, int depth);
	void getNestedSequences(const DcmTagKey& tag, DcmSequenceOfItems* sequence);
	void iterateItem(DcmItem *item, int& depth);
	DcmWidgetElement createElement(DcmElement* element = nullptr, DcmSequenceOfItems* sequence = nullptr, DcmItem* item = nullptr) const;
	//static double getFileSize(const std::string& fileName);
	void insert(DcmWidgetElement element, unsigned long &index) const;
	friend void replace(std::string& str, const std::string& from, const std::string& to);

private slots:
	//void okPressed();
	//void cancelPressed();
	//void findText() const;
};



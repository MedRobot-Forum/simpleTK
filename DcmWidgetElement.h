#pragma once

#include <QtWidgets>
#include <dcmtk/dcmdata/dcdeftag.h>

class DcmWidgetElement
{
public:
	DcmWidgetElement() { }
	DcmWidgetElement(const QString &itemTag, const QString &itemVR, const QString &itemVM, const QString &itemLength, const QString &itemDescription, const QString &itemValue);
	~DcmWidgetElement() = default;

	QString getItemTag() const;
	QString getItemVR() const;
	QString getItemVM() const;
	QString getItemLength() const;
	QString getItemDescription() const;
	QString getItemValue() const;
	int getTableIndex() const;
	int getDepth() const;
	void calculateTableIndex(const int& current, const std::vector<DcmWidgetElement>& elements);
	std::string toString() const;
	void setDepth(const int& depth);
	void setTableIndex(const int& index);
	void incrementDepth();
	void setItemTag(const QString& final);
	bool checkIfContains(const QString& str) const;
	DcmTagKey extractTagKey() const;
	void calculateDepthFromTag();
	void setVR(const QString& str);
	void setValue(const  QString& str);
	static int hexToDecimal(const char* hex);
	bool operator==(DcmWidgetElement &element) const;
	bool operator>(DcmWidgetElement &element) const;
	bool operator<(DcmWidgetElement &element) const;
	int compareTagKey(DcmWidgetElement &element) const;

private:
	QString itemTag;
	QString itemVR;
	QString itemVM;
	QString itemLength;
	QString itemDescription;
	QString itemValue;
	int depth = -1;
	int tableIndex = -1;
};

#include "TagDialog.h"

#include <QFileDialog>

TagDialog::TagDialog(QDialog * parent, QString filePath)
{
	ui.setupUi(this);
	this->path = filePath;

	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(15);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView { font-weight: 2000; }");
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	QHeaderView *verticalHeader = ui.tableWidget->verticalHeader();
	verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader->setDefaultSectionSize(10);

	initTag();
}
void TagDialog::initTag()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr(""), tr("DICOM File (*.dcm)"));

	if (!fileName.isEmpty())
	{
		if (file.loadFile(fileName.toStdString().c_str()).good())
		{
			ui.tableWidget->scrollToTop();
			this->clearTable();
			this->extractData(file);
			ui.tableWidget->resizeColumnsToContents();
			this->setWindowTitle("PixelData DICOM Editor - " + fileName);
			//ui.buttonInsert->setEnabled(true);
			//ui.buttonClose->setEnabled(true);
			//std::string nr = std::to_string(getFileSize(fileName.toStdString()));
			//precision(nr, 2);
			//ui.label->setText("Size: " + QString::fromStdString(nr) + " MB");
		}
		else
		{
			alertFailed("Failed to open file!");
		}
	}

}
//========================================================================================================================
TagDialog::~TagDialog()
{
	clearTable();
}
void replace(std::string& str, const std::string& from, const std::string& to)
{
	if (from.empty())
		return;

	size_t start_pos = 0;

	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void TagDialog::iterateItem(DcmItem * item, int& depth)
{
	depth++;

	for (unsigned long i = 0; i < item->getNumberOfValues(); i++)
	{
		DcmWidgetElement widgetElement = createElement(item->getElement(i), nullptr, nullptr);
		widgetElement.setItemTag(widgetElement.getItemTag().toUpper());

		if (widgetElement.getItemVR() != "SQ")
		{
			widgetElement.setDepth(depth);
			this->nestedElements.push_back(widgetElement);
		}

		const DcmTagKey NULLkey;
		DcmSequenceOfItems* sequence;
		item->getElement(i)->getParentItem()->findAndGetSequence(item->getElement(i)->getTag().getBaseTag(), sequence, true);
		this->getNestedSequences(NULLkey, sequence);
	}
}
DcmWidgetElement TagDialog::createElement(DcmElement* element, DcmSequenceOfItems* sequence, DcmItem* item) const
{
	if (element)
	{
		DcmTagKey tagKey = DcmTagKey(
			OFstatic_cast(Uint16, element->getGTag()),
			OFstatic_cast(Uint16, element->getETag()));
		DcmTag tagName = DcmTag(tagKey);
		DcmVR vr = DcmVR(element->getVR());
		std::string finalString;

		if (tagKey != DCM_PixelData && element->getLengthField() <= 50)
		{
			OFString value;
			element->getOFStringArray(value, true);
			finalString = value.c_str();
			replace(finalString, "\\", " ");
		}

		else
		{
			for (int i = 0; i < 10; i++)
			{
				OFString value;
				element->getOFString(value, i, false);
				finalString.append(value.c_str());
				finalString.append(" ");
			}
		}


		if (strcmp(vr.getVRName(), "??") == 0)
		{
			DcmWidgetElement widgetElement = DcmWidgetElement(
				QString::fromStdString(tagKey.toString().c_str()),
				QString::fromStdString(""),
				QString::fromStdString(std::to_string(element->getVM())),
				QString::fromStdString(std::to_string(element->getLength())),
				"",
				QString::fromStdString(finalString));

			return widgetElement;
		}

		DcmWidgetElement widgetElement = DcmWidgetElement(
			QString::fromStdString(tagKey.toString().c_str()),
			QString::fromStdString(vr.getVRName()),
			QString::fromStdString(std::to_string(element->getVM())),
			QString::fromStdString(std::to_string(element->getLength())),
			tagName.getTagName(),
			QString::fromStdString(finalString));


		return widgetElement;
	}

	if (sequence)
	{
		DcmTagKey tagKey = DcmTagKey(
			OFstatic_cast(Uint16, sequence->getGTag()),
			OFstatic_cast(Uint16, sequence->getETag()));
		DcmTag tagName = DcmTag(tagKey);
		DcmVR vr = DcmVR(sequence->getVR());
		std::string finalString;

		if (tagKey != DCM_PixelData && sequence->getLengthField() <= 50)
		{
			OFString value;
			sequence->getOFStringArray(value, true);
			finalString = value.c_str();
			replace(finalString, "\\", " ");
		}


		else
		{
			for (int i = 0; i < 10; i++)
			{
				OFString value;
				sequence->getOFString(value, i, false);
				finalString.append(value.c_str());
				finalString.append(" ");
			}

		}

		if (strcmp(vr.getVRName(), "??") == 0)
		{
			DcmWidgetElement widgetElement = DcmWidgetElement(
				QString::fromStdString(tagKey.toString().c_str()),
				QString::fromStdString(""),
				QString::fromStdString(std::to_string(sequence->getVM())),
				QString::fromStdString(std::to_string(sequence->getLength())),
				"",
				QString::fromStdString(finalString));

			return widgetElement;
		}

		DcmWidgetElement widgetElement = DcmWidgetElement(
			QString::fromStdString(tagKey.toString().c_str()),
			QString::fromStdString(vr.getVRName()),
			QString::fromStdString(std::to_string(sequence->getVM())),
			QString::fromStdString(std::to_string(sequence->getLength())),
			tagName.getTagName(),
			QString::fromStdString(finalString));


		return widgetElement;
	}

	if (item)
	{
		DcmTagKey tagKey = DcmTagKey(
			OFstatic_cast(Uint16, item->getGTag()),
			OFstatic_cast(Uint16, item->getETag()));
		DcmTag tagName = DcmTag(tagKey);
		DcmVR vr = DcmVR(item->getVR());


		if (strcmp(vr.getVRName(), "??") == 0)
		{
			DcmWidgetElement widgetElement = DcmWidgetElement(
				QString::fromStdString(tagKey.toString().c_str()),
				QString::fromStdString(""),
				QString::fromStdString(std::to_string(item->getVM())),
				QString::fromStdString(std::to_string(item->getLength())),
				"",
				QString::fromStdString(""));

			return widgetElement;
		}

		DcmWidgetElement widgetElement = DcmWidgetElement(
			QString::fromStdString(tagKey.toString().c_str()),
			QString::fromStdString(vr.getVRName()),
			QString::fromStdString(std::to_string(item->getVM())),
			QString::fromStdString(std::to_string(item->getLength())),
			tagName.getTagName(),
			QString::fromStdString(""));


		return widgetElement;
	}


	return DcmWidgetElement();

}


//========================================================================================================================
void TagDialog::extractData(DcmFileFormat file)
{
	DcmMetaInfo* metaInfo = file.getMetaInfo();
	DcmDataset* dataSet = file.getDataset();

	for (unsigned long i = 0; i < metaInfo->card(); i++)
	{
		this->insertInTable(metaInfo->getElement(i));
	}

	for (unsigned long i = 0; i < dataSet->card(); i++)
	{
		this->insertInTable(dataSet->getElement(i));
	}

}
void TagDialog::alertFailed(const std::string& message)
{
	auto* messageBox = new QMessageBox();
	messageBox->setIcon(QMessageBox::Warning);
	messageBox->setText(QString::fromStdString(message));
	messageBox->exec();
	delete messageBox;
}
//========================================================================================================================
void TagDialog::indent(DcmWidgetElement & element, int depth)
{
	if (depth == -1)
		return;

	QString str = element.getItemTag();

	while (depth--)
	{
		str.insert(0, " ");
	}

	element.setItemTag(str);
}

void TagDialog::getNestedSequences(const DcmTagKey& tag, DcmSequenceOfItems* sequence)
{
	OFCondition cond = OFCondition(EC_CorruptedData);

	if (tag.hasValidGroup() && tag.getGroup() != 0x7FE0)
	{
		cond = file.getDataset()->findAndGetSequence(tag, sequence, true);
	}

	else
	{
		DcmElement *elem;

		if (file.getDataset()->findAndGetElement(DCM_PixelData, elem, OFTrue).good())
		{
			DcmPixelData *dpix = OFstatic_cast(DcmPixelData*, elem);
			E_TransferSyntax xfer = EXS_Unknown;
			const DcmRepresentationParameter *param = nullptr;
			dpix->getOriginalRepresentationKey(xfer, param);
			DcmPixelSequence *pixSeq = nullptr;

			if (dpix->getEncapsulatedRepresentation(xfer, param, pixSeq).good() && (pixSeq != nullptr))
			{
				DcmWidgetElement widgetElement = createElement(elem, nullptr, nullptr);
				widgetElement.setItemTag(widgetElement.getItemTag().toUpper());
				this->nestedElements.push_back(widgetElement);

				for (unsigned long i = 0; i < pixSeq->card(); ++i)
				{
					DcmPixelItem* item;
					pixSeq->getItem(item, i);
					auto* newItem = reinterpret_cast<DcmItem*>(item);
					DcmWidgetElement widgetElemen = createElement(nullptr, nullptr, newItem);
					widgetElemen.setItemTag(widgetElemen.getItemTag().toUpper());
					widgetElemen.setDepth(1);
					widgetElemen.setVR(widgetElement.getItemVR().toUpper());
					widgetElemen.setValue("Not Loaded");
					this->nestedElements.push_back(widgetElemen);

				}

				DcmWidgetElement widgetElementDelim = DcmWidgetElement(
					QString("(FFFE,E00D)"),
					QString(""), QString("0"),
					QString("0"),
					QString("SequenceDelimitationItem"),
					QString(""));
				widgetElementDelim.setDepth(0);
				this->nestedElements.push_back(widgetElementDelim);

				return;
			}
		}
	}

	if (cond.good() || (sequence != nullptr &&  sequence->card()))
	{
		DcmWidgetElement widgetElement1 = createElement(nullptr, sequence, nullptr);
		widgetElement1.setDepth(this->depthRE);
		widgetElement1.setItemTag(widgetElement1.getItemTag().toUpper());
		this->nestedElements.push_back(widgetElement1);
		this->depthRE++;

		for (unsigned long i = 0; i < sequence->card(); i++)
		{
			DcmWidgetElement widgetElement2 = createElement(nullptr, nullptr, sequence->getItem(i));
			widgetElement2.setDepth(this->depthRE);
			widgetElement2.setItemTag(widgetElement2.getItemTag().toUpper());
			this->nestedElements.push_back(widgetElement2);
			this->iterateItem(sequence->getItem(i), this->depthRE);
			DcmWidgetElement widgetElementDelim = DcmWidgetElement(
				QString("(FFFE,E00D)"),
				QString(""), QString("0"),
				QString("0"),
				QString("ItemDelimitationItem"),
				QString(""));
			this->depthRE--;
			widgetElementDelim.setDepth(this->depthRE);
			this->nestedElements.push_back(widgetElementDelim);
		}

		DcmWidgetElement widgetElementDelim = DcmWidgetElement(
			QString("(FFFE,E0DD)"),
			QString(""),
			QString("0"),
			QString("0"),
			QString("SequenceDelimitationItem"),
			QString(""));
		this->depthRE--;
		widgetElementDelim.setDepth(this->depthRE);
		this->nestedElements.push_back(widgetElementDelim);
	}
}
void TagDialog::insertInTable(DcmElement* element)
{
	this->depthRE = 0;
	this->getNestedSequences(DcmTagKey(
		OFstatic_cast(Uint16, element->getGTag()),
		OFstatic_cast(Uint16, element->getETag())), nullptr);

	if (!this->nestedElements.empty())
	{
		for (auto widget_element : this->nestedElements)
		{
			indent(widget_element, widget_element.getDepth());
			this->insert(widget_element, globalIndex);
			DcmWidgetElement copyElement = DcmWidgetElement(widget_element);
			copyElement.setTableIndex(globalIndex);
			this->elements.push_back(copyElement);
			this->globalIndex++;

		}

		this->nestedElements.clear();
	}

	else
	{
		DcmWidgetElement widgetElement = createElement(element, nullptr, nullptr);
		widgetElement.setItemTag(widgetElement.getItemTag().toUpper());
		this->insert(widgetElement, globalIndex);
		DcmWidgetElement copyElement = DcmWidgetElement(widgetElement);
		copyElement.setTableIndex(globalIndex);
		this->elements.push_back(copyElement);
		this->globalIndex++;
	}
}
//========================================================================================================================
 DcmWidgetElement TagDialog::getElement() const
 {
 	return this->element;
 }

//========================================================================================================================
void TagDialog::clearTable() const
{
	for (int i = ui.tableWidget->rowCount() - 1; i >= 0; i--)
	{
		ui.tableWidget->removeRow(i);
	}
}

void TagDialog::insert(DcmWidgetElement element, unsigned long &index) const
{
	ui.tableWidget->insertRow(index);
	ui.tableWidget->setItem(index, 0, new QTableWidgetItem(element.getItemTag()));
	ui.tableWidget->setItem(index, 1, new QTableWidgetItem(element.getItemVR()));
	ui.tableWidget->setItem(index, 2, new QTableWidgetItem(element.getItemVM()));
	ui.tableWidget->setItem(index, 3, new QTableWidgetItem(element.getItemLength()));
	ui.tableWidget->setItem(index, 4, new QTableWidgetItem(element.getItemDescription()));
	ui.tableWidget->setItem(index, 5, new QTableWidgetItem(element.getItemValue()));
}
//
// //========================================================================================================================
// void TagSelectDialog::populate() const
// {
// 	auto* dictionary = new DcmDataDictionary(true, false);
// 	DcmHashDictIterator iterStart = dictionary->normalBegin();
// 	const DcmHashDictIterator iterEnd = dictionary->normalEnd();
// 	int count = 0;
//
// 	while (iterStart != iterEnd)
// 	{
// 		const DcmDictEntry* item = *iterStart;
// 		QString key = item->getBaseTag().toString().c_str();
// 		QString description = item->getTagName();
// 		QString vr = item->getVR().getVRName();
//
// 		if (key != "(fffe,e00d)" && key != "(fffe,00dd)")
// 		{
// 			ui.tableWidget->insertRow(count);
// 			ui.tableWidget->setItem(count, 0, new QTableWidgetItem(key));
// 			ui.tableWidget->setItem(count, 1, new QTableWidgetItem(description));
// 			ui.tableWidget->setItem(count, 2, new QTableWidgetItem(vr));
// 			count++;
// 		}
//
// 		++iterStart;
// 	}
//
// 	ui.tableWidget->resizeColumnsToContents();
// 	delete dictionary;
// }
//
// //========================================================================================================================
// void TagSelectDialog::okPressed()
// {
// 	QList<QTableWidgetItem*> items = ui.tableWidget->selectedItems();
//
// 	if (!items.empty())
// 	{
// 		const DcmWidgetElement element = DcmWidgetElement(items[0]->text(), items[2]->text(), "", "", items[1]->text(), ui.lineEdit->text());
//
// 		if (element.getItemValue().isEmpty() && element.getItemVR() != "na" && element.getItemVR() != "SQ")
// 		{
// 			auto* box = new QMessageBox();
// 			box->setText("No value entered!");
// 			box->setIcon(QMessageBox::Warning);
// 			box->exec();
// 			delete box;
// 			this->reject();
// 		}
//
// 		this->element = element;
// 		this->accept();
// 	}
// }
//
// //========================================================================================================================
// void TagSelectDialog::cancelPressed()
// {
// 	this->reject();
// }
//
// //========================================================================================================================
// void TagSelectDialog::findText() const
// {
// 	const QString text = ui.lineEditSearch->text();
// 	std::vector<DcmWidgetElement> result;
//
// 	if (!text.isEmpty())
// 	{
// 		std::unique_ptr <DcmDataDictionary> dictionary = std::make_unique<DcmDataDictionary>(true, false);
// 		DcmHashDictIterator iterStart = dictionary->normalBegin();
// 		const DcmHashDictIterator iterEnd = dictionary->normalEnd();
//
// 		while (iterStart != iterEnd)
// 		{
//
//
// 			const DcmDictEntry* item = *iterStart;
// 			QString key = item->getBaseTag().toString().c_str();
// 			QString description = item->getTagName();
// 			QString vr = item->getVR().getVRName();
//
// 			DcmWidgetElement element = DcmWidgetElement(key, vr, "", "", description, "");
// 			if (element.checkIfContains(text))
// 			{
// 				result.push_back(element);
// 			}
//
// 			++iterStart;
// 		}
//
// 		if (!result.empty())
// 		{
// 			clearTable();
//
// 			for (unsigned long i = 0; i < result.size(); i++)
// 			{
// 				if (result[i].getItemTag() != "(fffe,e00d)" &&result[i].getItemTag() != "(fffe,00dd)")
// 				{
// 					ui.tableWidget->insertRow(i);
// 					ui.tableWidget->setItem(i, 0, new QTableWidgetItem(result[i].getItemTag()));
// 					ui.tableWidget->setItem(i, 1, new QTableWidgetItem(result[i].getItemDescription()));
// 					ui.tableWidget->setItem(i, 2, new QTableWidgetItem(result[i].getItemVR()));
// 				}
// 			}
// 		}
// 	}
//
// 	else
// 	{
// 		clearTable();
// 		populate();
// 	}
// }

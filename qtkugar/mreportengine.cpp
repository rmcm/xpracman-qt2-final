/***************************************************************************
              mreportengine.cpp  -  Kugar report engine
              -------------------
    begin     : Sun Aug 15 1999                                           
    copyright : (C) 1999 by Mutiny Bay Software                         
    email     : info@mutinybaysoftware.com                                     
 ***************************************************************************/

#include <qfile.h>

#include "mreportengine.h"
#include "mutil.h"


/** Constructor */
MReportEngine::MReportEngine() : QObject(){
  // Set page params
  pageSize = MReportEngine::Letter;
  pageOrientation = MReportEngine::Portrait;
  topMargin = 0;
  bottomMargin = 0;
  leftMargin = 0;
  rightMargin = 0;

	// Set cancel flag
	cancelRender = false;

  // Set the grand total list to AutoDelete
  grandTotal.setAutoDelete( true );

  rHeader.setPrintFrequency(MReportSection::FirstPage);
  pHeader.setPrintFrequency(MReportSection::EveryPage);
  pFooter.setPrintFrequency(MReportSection::EveryPage);
  rFooter.setPrintFrequency(MReportSection::LastPage);

  // Set the default page metrics
  QSize ps = getPageMetrics(pageSize, pageOrientation);
  pageWidth = ps.width();
  pageHeight = ps.height();
}

/** Copy constructor */
MReportEngine::MReportEngine(const MReportEngine& mReportEngine) /*: QObject((QObject &) mReportEngine)*/{
  copy(&mReportEngine);
}

/** Assignment operator */
MReportEngine MReportEngine::operator=(const MReportEngine& mReportEngine){
  if(&mReportEngine == this) return *this;

  // Copy the derived class's data
  copy(&mReportEngine);

  // Copy the base class's data
  //((QObject &) *this) = mReportEngine;

	return *this;
}

/** Destructor */
MReportEngine::~MReportEngine(){
	// Clear the grand total data
  grandTotal.clear();
}

/** Clears report formatting */
void MReportEngine::clearFormatting(){
	rHeader.clear();
	pHeader.clear();
	detail.clear();
	pFooter.clear();
	rFooter.clear();
}


// Set the report's data from an inline string.  Return true if it was valid
// data.

bool MReportEngine::setReportData(const QString &data)
{
	if (!rd.setContent(data))
	{
		qWarning("Unable to parse report data");
		return false;
	}

	initData();

	return true;
}


// Set the report's data from an i/o device.  Return true if it was valid data.

bool MReportEngine::setReportData(QIODevice *dev)
{
	if (!rd.setContent(dev))
	{
		qWarning("Unable to parse report data");
		return false;
	}

	initData();

	return true;
}


// Initialise after reading a report's data.

void MReportEngine::initData()
{
	// Get the record set (we assume there is only one).

	for (QDomNode n = rd.firstChild(); !n.isNull(); n = n.nextSibling())
		if (n.nodeName() == "KugarData")
		{
			// Get the records.

			records = n.childNodes();

			// See if there is a prefered template given.

			QDomNamedNodeMap attr = n.attributes();
			QDomNode tempattr = attr.namedItem("Template");
			QString tempname = tempattr.nodeValue();

			if (!tempname.isNull())
				emit preferedTemplate(tempname);

			break;
		}
}


// Set the report's template from an inline string.  Return true if it was a
// valid template.

bool MReportEngine::setReportTemplate(const QString &tpl)
{
	clearFormatting();

	if (!rt.setContent(tpl))
	{
		qWarning("Unable to parse report template");
		return false;
	}

	initTemplate();

	return true;
}


// Set the report's template from an i/o device.  Return true if it was a valid
// template.

bool MReportEngine::setReportTemplate(QIODevice *dev)
{
	clearFormatting();

	if (!rt.setContent(dev))
	{
		qWarning("Unable to parse report template");
		return false;
	}

	initTemplate();

	return true;
}


/** Cancels rendering of a report */
void MReportEngine::slotCancelRendering(){
	cancelRender = true;
}

/** Renders the report as a page collection - the caller
  * is responsible for de-allocating the returned
  * collection
  */
MPageCollection* MReportEngine::renderReport(){
  unsigned int j;
  unsigned int i;

  // Set cancel flag
  cancelRender = false;

  // Create the page collection
  MPageCollection* pages = new MPageCollection;

  // Initialize the basic page data
  currHeight = pageHeight - (topMargin + bottomMargin + pFooter.getHeight());
  currPage = 0;
  currDate = QDate::currentDate();

  // Initialize the grand total array
  grandTotal.clear();
  for(int i = 0; i < rFooter.getCalcFieldCount(); i++)
    grandTotal.append(new QArray<double>);

  // Create the first page
  startPage(pages);

  // Draw the detail section by looping through all the records
  unsigned int rowCount = records.length();
  unsigned int fieldCount = detail.getFieldCount();
  int chkRow = 0;

  for(j = 0; j < rowCount; j++){
    QString detailValue;
    QDomNode record = records.item(j);

    if(record.nodeType() == QDomNode::ElementNode){
	// Update status event
	if((chkRow = (j / 2) % 20) == 0)
		emit signalRenderStatus(j / 2);

	// Check for cancel action
	if(cancelRender){
    		p.end();
		delete pages;
		return 0;
	}

	// Process the record ...
      QDomNamedNodeMap fields = record.attributes();
          // look for ACTION attributes
      QDomNode action_test = fields.namedItem("ACTION");
      if ( !action_test.isNull() )
      {
          if ( action_test.nodeValue() == "FF" )
          {
              newPage(pages);
              continue;
          }
      }
    

      for(i = 0; i < fieldCount; i++){
	// Get the field value
	detailValue =  fields.namedItem(detail.getFieldName(i)).nodeValue();
        // Set the field
        detail.setFieldData(i, detailValue);
        // Set the grand total data
        int calcIdx = rFooter.getCalcFieldIndex(detail.getFieldName(i));
        if(calcIdx != -1){
          int vsize = grandTotal.at(calcIdx)->size();
          grandTotal.at(calcIdx)->resize(vsize + 1);
          grandTotal.at(calcIdx)->at(vsize) = detailValue.toDouble();
        }
      }

			if(currY > currHeight)
     		newPage(pages);

     	detail.setPageNumber(currPage);
     	detail.setReportDate(currDate);
     	detail.draw(&p, leftMargin, currY);
    	currY += detail.getHeight();
    }
  }

  // Finish the last page of the report
  endPage(pages);

  // Destroy the page painter
  p.end();

  // Set the page collection attributes
  pages->setPageDimensions(QSize(pageWidth, pageHeight));
  pages->setPageSize(pageSize);
  pages->setPageOrientation(pageOrientation);

	// Send final status
	emit signalRenderStatus(rowCount / 2);

  return pages;
}

/** Starts a new page of the report */
void MReportEngine::startPage(MPageCollection* pages){
  // Set the current y pos
  currY = topMargin;

  // Create a new page
  pages->appendPage();
  // Increment the page count
  currPage++;

  // Set the page painter to the current page
  p.begin(pages->getCurrentPage());

  // Draw the report header
  drawReportHeader(pages);
  // Draw the page header
  drawPageHeader(pages);
}

/** Finishes the current page of the report */
void MReportEngine::endPage(MPageCollection* pages){
  // Draw the report footer
  drawReportFooter(pages);
  // Draw the page footer
  drawPageFooter(pages);
}

/** Finishes the current page and adds a new page */
void MReportEngine::newPage(MPageCollection* pages){
  // Draw the page footer
  drawPageFooter(pages);

  // Stop the painter
  p.end();

  // Start a new page
  startPage(pages);
}

/** Draws the report header object to the current page */
void MReportEngine::drawReportHeader(MPageCollection* pages){
	if(rHeader.getHeight() == 0) return;

  if((rHeader.printFrequency() == MReportSection::FirstPage && currPage == 1)
     ||(rHeader.printFrequency() == MReportSection::EveryPage)){

    rHeader.setPageNumber(currPage);
    rHeader.setReportDate(currDate);
    rHeader.draw(&p, leftMargin, currY);
    currY += rHeader.getHeight();
  }
}

/** Draws the page header to the current page */
void MReportEngine::drawPageHeader(MPageCollection* pages){
	if(pHeader.getHeight() == 0) return;

  if((currY + pHeader.getHeight()) > currHeight)
    newPage(pages);

  if((pHeader.printFrequency() == MReportSection::FirstPage && currPage == 1)
     ||(pHeader.printFrequency() == MReportSection::EveryPage)){

    pHeader.setPageNumber(currPage);
    pHeader.setReportDate(currDate);
    pHeader.draw(&p, leftMargin, currY);
    currY += pHeader.getHeight();
  }
}

/** Draws the page footer to the current page */
void MReportEngine::drawPageFooter(MPageCollection* pages){
	if(pFooter.getHeight() == 0) return;

  if((pFooter.printFrequency() == MReportSection::FirstPage && currPage == 1)
     ||(pFooter.printFrequency() == MReportSection::EveryPage)){

    pFooter.setPageNumber(currPage);
    pFooter.setReportDate(currDate);
    pFooter.draw(&p, leftMargin, (pageHeight - bottomMargin) - pFooter.getHeight());
    currY += pFooter.getHeight();
  }
}

/** Draws the report footer to the current page */
void MReportEngine::drawReportFooter(MPageCollection* pages){
	if(rFooter.getHeight() == 0) return;

  if((currY + rFooter.getHeight()) > currHeight)
    newPage(pages);

  if((rFooter.printFrequency() == MReportSection::EveryPage)
     ||(rFooter.printFrequency() == MReportSection::LastPage)){

    rFooter.setCalcFieldData(&grandTotal);

    rFooter.setPageNumber(currPage);
    rFooter.setReportDate(currDate);
    rFooter.draw(&p, leftMargin, currY);
    currY += rFooter.getHeight();
  }
}

/** Gets the metrics for the selected page size & orientation */
QSize MReportEngine::getPageMetrics(int size, int orientation){
  QPrinter* printer;
  QSize ps;

  // Set the page size
  printer = new QPrinter();
  printer->setFullPage(true);
  printer->setPageSize((QPrinter::PageSize)size);
  printer->setOrientation((QPrinter::Orientation)orientation);

  // Get the page metrics
  QPaintDeviceMetrics pdm(printer);

  // Display the first page of the report
  ps.setWidth(pdm.width());
  ps.setHeight(pdm.height());

  delete printer;

  return ps;
}

/** Walks the document tree, setting the report layout */
void MReportEngine::initTemplate(){
  QDomNode report;
  QDomNode child;

	// Get the report - assume there is only one.

	for (report = rt.firstChild(); !report.isNull(); report = report.nextSibling())
		if (report.nodeName() == "KugarTemplate")
			break;

  setReportAttributes(&report);

  // Get all the child report elements
  QDomNodeList children = report.childNodes();
  int childCount = children.length();

  for(int j = 0; j < childCount; j++){
    child = children.item(j);

    if(child.nodeType() == QDomNode::ElementNode){
      // Report Header
      if(child.nodeName() == "ReportHeader")
        setSectionAttributes(&rHeader, &child);
      else if(child.nodeName() == "PageHeader")
        setSectionAttributes(&pHeader, &child);
      else if(child.nodeName() == "Detail")
        setDetailAttributes(&child);
      else if(child.nodeName() == "PageFooter")
        setSectionAttributes(&pFooter, &child);
      else if(child.nodeName() == "ReportFooter")
        setSectionAttributes(&rFooter, &child);
    }
  }
}

/** Sets the main layout attributes for the report */
void MReportEngine::setReportAttributes(QDomNode* report){
  // Get the attributes for the report
  QDomNamedNodeMap attributes = report->attributes();

  pageSize = attributes.namedItem("PageSize").nodeValue().toInt();
  pageOrientation = attributes.namedItem("PageOrientation").nodeValue().toInt();
  topMargin = attributes.namedItem("TopMargin").nodeValue().toInt();
  bottomMargin = attributes.namedItem("BottomMargin").nodeValue().toInt();
  leftMargin = attributes.namedItem("LeftMargin").nodeValue().toInt();
  rightMargin = attributes.namedItem("RightMargin").nodeValue().toInt();

  // Set the page metrics
  QSize ps = getPageMetrics(pageSize, pageOrientation);
  pageWidth = ps.width();
  pageHeight = ps.height();
}

/** Sets the layout attributes for the given report section */
void MReportEngine::setSectionAttributes(MReportSection* section, QDomNode* report){
  // Get the attributes for the section
  QDomNamedNodeMap attributes = report->attributes();

  // Get the section attributes
  section->setHeight(attributes.namedItem("Height").nodeValue().toInt());
  section->setPrintFrequency(attributes.namedItem("PrintFrequency").nodeValue().toInt());

  // Process the sections labels
  QDomNodeList children = report->childNodes();
  int childCount = children.length();

  // For each label, extract the attr list and add the new label
  // to the sections's label collection
  for(int j = 0; j < childCount; j++){
    QDomNode child = children.item(j);
    if(child.nodeType() == QDomNode::ElementNode){
	if(child.nodeName() == "Line"){
          QDomNamedNodeMap attributes = child.attributes();
          MLineObject* line = new MLineObject();
          setLineAttributes(line, &attributes);
          section->addLine(line);
        }
        else if(child.nodeName() == "Label"){
          QDomNamedNodeMap attributes = child.attributes();
          MLabelObject* label = new MLabelObject();
          setLabelAttributes(label, &attributes);
          section->addLabel(label);
        }
        else if(child.nodeName() == "Special"){
          QDomNamedNodeMap attributes = child.attributes();
          MSpecialObject* field = new MSpecialObject();
          setSpecialAttributes(field, &attributes);
          section->addSpecialField(field);
        }
        else if(child.nodeName() == "CalculatedField"){
          QDomNamedNodeMap attributes = child.attributes();
          MCalcObject* field = new MCalcObject();
          setCalculatedFieldAttributes(field, &attributes);
          section->addCalculatedField(field);
        }
    }
  }
}

/** Sets the layout attributes for the detail section */
void MReportEngine::setDetailAttributes(QDomNode* report){
  // Get the attributes for the detail section
  QDomNamedNodeMap attributes = report->attributes();

  // Get the report detail attributes
  detail.setHeight(attributes.namedItem("Height").nodeValue().toInt());

  // Process the report detail labels
  QDomNodeList children = report->childNodes();
  int childCount = children.length();

  for(int j = 0; j < childCount; j++){
    QDomNode child = children.item(j);
    if(child.nodeType() == QDomNode::ElementNode){
	if(child.nodeName() == "Line"){
          QDomNamedNodeMap attributes = child.attributes();
          MLineObject* line = new MLineObject();
          setLineAttributes(line, &attributes);
          detail.addLine(line);
        }
        else if(child.nodeName() == "Label"){
          QDomNamedNodeMap attributes = child.attributes();
          MLabelObject* label = new MLabelObject();
          setLabelAttributes(label, &attributes);
          detail.addLabel(label);
        }
        else if(child.nodeName() == "Special"){
          QDomNamedNodeMap attributes = child.attributes();
          MSpecialObject* field = new MSpecialObject();
          setSpecialAttributes(field, &attributes);
          detail.addSpecialField(field);
        }
        else if(child.nodeName() == "Field"){
          QDomNamedNodeMap attributes = child.attributes();
          MFieldObject* field = new MFieldObject();
          setFieldAttributes(field, &attributes);
          detail.addField(field);
        }
    }
  }
}

/** Sets a line's layout attributes */
void MReportEngine::setLineAttributes(MLineObject* line, QDomNamedNodeMap* attr){
  line->setLine(attr->namedItem("X1").nodeValue().toInt(),
  		attr->namedItem("Y1").nodeValue().toInt(),
                attr->namedItem("X2").nodeValue().toInt(),
                attr->namedItem("Y2").nodeValue().toInt());

  QString tmp = attr->namedItem("Color").nodeValue();

  line->setColor(tmp.left(tmp.find(",")).toInt(),
                 tmp.mid(tmp.find(",") + 1, (tmp.findRev(",") - tmp.find(",")) - 1).toInt(),
                 tmp.right(tmp.length() - tmp.findRev(",") - 1).toInt());

  line->setWidth(attr->namedItem("Width").nodeValue().toInt());
  line->setStyle(attr->namedItem("Style").nodeValue().toInt());
}

/** Sets a label's layout attributes */
void MReportEngine::setLabelAttributes(MLabelObject* label, QDomNamedNodeMap* attr){
  QString tmp;

  label->setText(attr->namedItem("Text").nodeValue());
  label->setGeometry(attr->namedItem("X").nodeValue().toInt(),
                     attr->namedItem("Y").nodeValue().toInt(),
                     attr->namedItem("Width").nodeValue().toInt(),
                     attr->namedItem("Height").nodeValue().toInt());

  tmp = attr->namedItem("BackgroundColor").nodeValue();
  label->setBackgroundColor(tmp.left(tmp.find(",")).toInt(),
                            tmp.mid(tmp.find(",") + 1, (tmp.findRev(",") - tmp.find(",")) - 1).toInt(),
                            tmp.right(tmp.length() - tmp.findRev(",") - 1).toInt());

  tmp = attr->namedItem("ForegroundColor").nodeValue();
  label->setForegroundColor(tmp.left(tmp.find(",")).toInt(),
                            tmp.mid(tmp.find(",") + 1, (tmp.findRev(",") - tmp.find(",")) - 1).toInt(),
                            tmp.right(tmp.length() - tmp.findRev(",") - 1).toInt());

  tmp = attr->namedItem("BorderColor").nodeValue();
  label->setBorderColor(tmp.left(tmp.find(",")).toInt(),
                            tmp.mid(tmp.find(",") + 1, (tmp.findRev(",") - tmp.find(",")) - 1).toInt(),
                            tmp.right(tmp.length() - tmp.findRev(",") - 1).toInt());

  label->setBorderWidth(attr->namedItem("BorderWidth").nodeValue().toInt());
  label->setBorderStyle(attr->namedItem("BorderStyle").nodeValue().toInt());
  label->setFont(attr->namedItem("FontFamily").nodeValue(),
                 attr->namedItem("FontSize").nodeValue().toInt(),
                 attr->namedItem("FontWeight").nodeValue().toInt(),
                 (attr->namedItem("FontItalic").nodeValue().toInt() == 0 ? false : true));
  label->setHorizontalAlignment(attr->namedItem("HAlignment").nodeValue().toInt());
  label->setVerticalAlignment(attr->namedItem("VAlignment").nodeValue().toInt());
  label->setWordWrap(attr->namedItem("WordWrap").nodeValue().toInt() == 0 ? false : true);
}

/** Sets a special field's layout attributes */
void MReportEngine::setSpecialAttributes(MSpecialObject* field, QDomNamedNodeMap* attr){
  field->setType(attr->namedItem("Type").nodeValue().toInt());
  field->setDateFormat(attr->namedItem("DateFormat").nodeValue().toInt());

  setLabelAttributes((MLabelObject *)field, attr);
}

/** Sets a field's layout attributes */
void MReportEngine::setFieldAttributes(MFieldObject* field, QDomNamedNodeMap* attr){
  field->setFieldName(attr->namedItem("Field").nodeValue());
  field->setDataType(attr->namedItem("DataType").nodeValue().toInt());
  field->setDateFormat(attr->namedItem("DateFormat").nodeValue().toInt());
  field->setPrecision(attr->namedItem("Precision").nodeValue().toInt());
  field->setCurrency(attr->namedItem("Currency").nodeValue().toInt());
  field->setCommaSeparator(attr->namedItem("CommaSeparator").nodeValue().toInt());

  QString tmp = attr->namedItem("NegValueColor").nodeValue();

  field->setNegValueColor(tmp.left(tmp.find(",")).toInt(),
                          tmp.mid(tmp.find(",") + 1, (tmp.findRev(",") - tmp.find(",")) - 1).toInt(),
                          tmp.right(tmp.length() - tmp.findRev(",") - 1).toInt());

  setLabelAttributes((MLabelObject *)field, attr);
}

/** Sets a calculated field's layout attributes */
void MReportEngine::setCalculatedFieldAttributes(MCalcObject* field, QDomNamedNodeMap* attr){
  field->setCalculationType(attr->namedItem("CalculationType").nodeValue().toInt());
  setFieldAttributes((MFieldObject *)field, attr);
}

/** Copies member data from one object to another.
  Used by the copy constructor and assignment operator */
void MReportEngine::copy(const MReportEngine* mReportEngine){
  // Copy document data
  rd = mReportEngine->rd;
  rt = mReportEngine->rt;

  // Copy page metrics
  pageSize = mReportEngine->pageSize;
  pageOrientation = mReportEngine->pageOrientation;
  topMargin = mReportEngine->topMargin;
  bottomMargin = mReportEngine->bottomMargin;
  leftMargin = mReportEngine->leftMargin;
  rightMargin = mReportEngine->rightMargin;
  pageWidth = mReportEngine->pageWidth;
  pageHeight = mReportEngine->pageHeight;

  // Copy the report header
  rHeader = mReportEngine->rHeader;
  // Copy the page header
  pHeader = mReportEngine->pHeader;
  // Copy the detail section
  detail = mReportEngine->detail;
  // Copy the page footer
  pFooter = mReportEngine->pFooter;
  // Copy the report footer
  rFooter = mReportEngine->rFooter;

  // Copy the rendering state
  currY = mReportEngine->currY;
  currHeight = mReportEngine->currHeight;
  currPage = mReportEngine->currPage;
  currDate = mReportEngine->currDate;
	cancelRender = mReportEngine->cancelRender;

	// Copy grand totals list
	grandTotal = mReportEngine->grandTotal;
}

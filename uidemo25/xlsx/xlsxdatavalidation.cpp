/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#include "xlsxdatavalidation.h"
#include "xlsxdatavalidation_p.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

QT_BEGIN_NAMESPACE_XLSX

DataValidationPrivate::DataValidationPrivate()
    :validationType(DataValidation::None), validationOperator(DataValidation::Between)
    , errorStyle(DataValidation::Stop), allowBlank(false), isPromptMessageVisible(true)
    , isErrorMessageVisible(true)
{

}

DataValidationPrivate::DataValidationPrivate(DataValidation::ValidationType type, DataValidation::ValidationOperator op, const QString &formula1, const QString &formula2, bool allowBlank)
    :validationType(type), validationOperator(op)
    , errorStyle(DataValidation::Stop), allowBlank(allowBlank), isPromptMessageVisible(true)
    , isErrorMessageVisible(true), formula1(formula1), formula2(formula2)
{

}

DataValidationPrivate::DataValidationPrivate(const DataValidationPrivate &other)
    :QSharedData(other)
    , validationType(DataValidation::None), validationOperator(DataValidation::Between)
    , errorStyle(DataValidation::Stop), allowBlank(false), isPromptMessageVisible(true)
    , isErrorMessageVisible(true)
{

}

DataValidationPrivate::~DataValidationPrivate()
{

}

/*!
 * \class DataValidation
 * \brief Data validation for single cell or a range
 * \inmodule QtXlsx
 *
 * The data validation can be applied to a single cell or a range of cells.
 */

/*!
 * \enum DataValidation::ValidationType
 *
 * The enum type defines the type of data that you wish to validate.
 *
 * \value None the type of data is unrestricted. This is the same as not applying a data validation.
 * \value Whole restricts the cell to integer values. Means "Whole number"?
 * \value Decimal restricts the cell to decimal values.
 * \value List restricts the cell to a set of user specified values.
 * \value Date restricts the cell to date values.
 * \value Time restricts the cell to time values.
 * \value TextLength restricts the cell data based on an integer string length.
 * \value Custom restricts the cell based on an external Excel formula that returns a true/false value.
 */

/*!
 * \enum DataValidation::ValidationOperator
 *
 *  The enum type defines the criteria by which the data in the
 *  cell is validated
 *
 * \value Between
 * \value NotBetween
 * \value Equal
 * \value NotEqual
 * \value LessThan
 * \value LessThanOrEqual
 * \value GreaterThan
 * \value GreaterThanOrEqual
 */

/*!
 * \enum DataValidation::ErrorStyle
 *
 *  The enum type defines the type of error dialog that
 *  is displayed.
 *
 * \value Stop
 * \value Warning
 * \value Information
 */

/*!
 * Construct a data validation object with the given \a type, \a op, \a formula1
 * \a formula2, and \a allowBlank.
 */
DataValidation::DataValidation(ValidationType type, ValidationOperator op, const QString &formula1, const QString &formula2, bool allowBlank)
    :d(new DataValidationPrivate(type, op, formula1, formula2, allowBlank))
{

}

/*!
    Construct a data validation object
*/
DataValidation::DataValidation()
    :d(new DataValidationPrivate())
{

}

/*!
    Constructs a copy of \a other.
*/
DataValidation::DataValidation(const DataValidation &other)
    :d(other.d)
{

}

/*!
    Assigns \a other to this validation and returns a reference to this validation.
 */
DataValidation &DataValidation::operator=(const DataValidation &other)
{
    this->d = other.d;
    return *this;
}


/*!
 * Destroy the object.
 */
DataValidation::~DataValidation()
{
}

/*!
    Returns the validation type.
 */
DataValidation::ValidationType DataValidation::validationType() const
{
    return d->validationType;
}

/*!
    Returns the validation operator.
 */
DataValidation::ValidationOperator DataValidation::validationOperator() const
{
    return d->validationOperator;
}

/*!
    Returns the validation error style.
 */
DataValidation::ErrorStyle DataValidation::errorStyle() const
{
    return d->errorStyle;
}

/*!
    Returns the formula1.
 */
QString DataValidation::formula1() const
{
    return d->formula1;
}

/*!
    Returns the formula2.
 */
QString DataValidation::formula2() const
{
    return d->formula2;
}

/*!
    Returns whether blank is allowed.
 */
bool DataValidation::allowBlank() const
{
    return d->allowBlank;
}

/*!
    Returns the error message.
 */
QString DataValidation::errorMessage() const
{
    return d->errorMessage;
}

/*!
    Returns the error message title.
 */
QString DataValidation::errorMessageTitle() const
{
    return d->errorMessageTitle;
}

/*!
    Returns the prompt message.
 */
QString DataValidation::promptMessage() const
{
    return d->promptMessage;
}

/*!
    Returns the prompt message title.
 */
QString DataValidation::promptMessageTitle() const
{
    return d->promptMessageTitle;
}

/*!
    Returns the whether prompt message is shown.
 */
bool DataValidation::isPromptMessageVisible() const
{
    return d->isPromptMessageVisible;
}

/*!
    Returns the whether error message is shown.
 */
bool DataValidation::isErrorMessageVisible() const
{
    return d->isErrorMessageVisible;
}

/*!
    Returns the ranges on which the validation will be applied.
 */
QList<CellRange> DataValidation::ranges() const
{
    return d->ranges;
}

/*!
    Sets the validation type to \a type.
 */
void DataValidation::setValidationType(DataValidation::ValidationType type)
{
    d->validationType = type;
}

/*!
    Sets the validation operator to \a op.
 */
void DataValidation::setValidationOperator(DataValidation::ValidationOperator op)
{
    d->validationOperator = op;
}

/*!
    Sets the error style to \a es.
 */
void DataValidation::setErrorStyle(DataValidation::ErrorStyle es)
{
    d->errorStyle = es;
}

/*!
    Sets the formula1 to \a formula.
 */
void DataValidation::setFormula1(const QString &formula)
{
    if (formula.startsWith(QLatin1Char('=')))
        d->formula1 = formula.mid(1);
    else
        d->formula1 = formula;
}

/*!
    Sets the formulas to \a formula.
 */
void DataValidation::setFormula2(const QString &formula)
{
    if (formula.startsWith(QLatin1Char('=')))
        d->formula2 = formula.mid(1);
    else
        d->formula2 = formula;
}

/*!
    Sets the error message to \a error with title \a title.
 */
void DataValidation::setErrorMessage(const QString &error, const QString &title)
{
    d->errorMessage = error;
    d->errorMessageTitle = title;
}

/*!
    Sets the prompt message to \a prompt with title \a title.
 */
void DataValidation::setPromptMessage(const QString &prompt, const QString &title)
{
    d->promptMessage = prompt;
    d->promptMessageTitle = title;
}

/*!
    Enable/disabe blank allow based on \a enable.
 */
void DataValidation::setAllowBlank(bool enable)
{
    d->allowBlank = enable;
}

/*!
    Enable/disabe prompt message visible based on \a visible.
 */
void DataValidation::setPromptMessageVisible(bool visible)
{
    d->isPromptMessageVisible = visible;
}

/*!
    Enable/disabe error message visible based on \a visible.
 */
void DataValidation::setErrorMessageVisible(bool visible)
{
    d->isErrorMessageVisible = visible;
}

/*!
    Add the \a cell on which the DataValidation will apply to.
 */
void DataValidation::addCell(const CellReference &cell)
{
    d->ranges.append(CellRange(cell, cell));
}

/*!
    \overload
    Add the cell(\a row, \a col) on which the DataValidation will apply to.
 */
void DataValidation::addCell(int row, int col)
{
    d->ranges.append(CellRange(row, col, row, col));
}

/*!
    \overload
    Add the range(\a firstRow, \a firstCol, \a lastRow, \a lastCol) on
    which the DataValidation will apply to.
 */
void DataValidation::addRange(int firstRow, int firstCol, int lastRow, int lastCol)
{
    d->ranges.append(CellRange(firstRow, firstCol, lastRow, lastCol));
}

/*!
    Add the \a range on which the DataValidation will apply to.
 */
void DataValidation::addRange(const CellRange &range)
{
    d->ranges.append(range);
}

/*!
 * \internal
 */
bool DataValidation::saveToXml(QXmlStreamWriter &writer) const
{
    static QMap<DataValidation::ValidationType, QString> typeMap;
    static QMap<DataValidation::ValidationOperator, QString> opMap;
    static QMap<DataValidation::ErrorStyle, QString> esMap;
    if (typeMap.isEmpty()) {
        typeMap.insert(DataValidation::None, QString("none"));
        typeMap.insert(DataValidation::Whole, QString("whole"));
        typeMap.insert(DataValidation::Decimal, QString("decimal"));
        typeMap.insert(DataValidation::List, QString("list"));
        typeMap.insert(DataValidation::Date, QString("date"));
        typeMap.insert(DataValidation::Time, QString("time"));
        typeMap.insert(DataValidation::TextLength, QString("textLength"));
        typeMap.insert(DataValidation::Custom, QString("custom"));

        opMap.insert(DataValidation::Between, QString("between"));
        opMap.insert(DataValidation::NotBetween, QString("notBetween"));
        opMap.insert(DataValidation::Equal, QString("equal"));
        opMap.insert(DataValidation::NotEqual, QString("notEqual"));
        opMap.insert(DataValidation::LessThan, QString("lessThan"));
        opMap.insert(DataValidation::LessThanOrEqual, QString("lessThanOrEqual"));
        opMap.insert(DataValidation::GreaterThan, QString("greaterThan"));
        opMap.insert(DataValidation::GreaterThanOrEqual, QString("greaterThanOrEqual"));

        esMap.insert(DataValidation::Stop, QString("stop"));
        esMap.insert(DataValidation::Warning, QString("warning"));
        esMap.insert(DataValidation::Information, QString("information"));
    }

    writer.writeStartElement(QString("dataValidation"));
    if (validationType() != DataValidation::None)
        writer.writeAttribute(QString("type"), typeMap[validationType()]);
    if (errorStyle() != DataValidation::Stop)
        writer.writeAttribute(QString("errorStyle"), esMap[errorStyle()]);
    if (validationOperator() != DataValidation::Between)
        writer.writeAttribute(QString("operator"), opMap[validationOperator()]);
    if (allowBlank())
        writer.writeAttribute(QString("allowBlank"), QString("1"));
    //        if (dropDownVisible())
    //            writer.writeAttribute(QString("showDropDown"), QString("1"));
    if (isPromptMessageVisible())
        writer.writeAttribute(QString("showInputMessage"), QString("1"));
    if (isErrorMessageVisible())
        writer.writeAttribute(QString("showErrorMessage"), QString("1"));
    if (!errorMessageTitle().isEmpty())
        writer.writeAttribute(QString("errorTitle"), errorMessageTitle());
    if (!errorMessage().isEmpty())
        writer.writeAttribute(QString("error"), errorMessage());
    if (!promptMessageTitle().isEmpty())
        writer.writeAttribute(QString("promptTitle"), promptMessageTitle());
    if (!promptMessage().isEmpty())
        writer.writeAttribute(QString("prompt"), promptMessage());

    QStringList sqref;
    foreach (CellRange range, ranges())
        sqref.append(range.toString());
    writer.writeAttribute(QString("sqref"), sqref.join(QLatin1Char(' ')));

    if (!formula1().isEmpty())
        writer.writeTextElement(QString("formula1"), formula1());
    if (!formula2().isEmpty())
        writer.writeTextElement(QString("formula2"), formula2());

    writer.writeEndElement(); //dataValidation

    return true;
}

/*!
 * \internal
 */
DataValidation DataValidation::loadFromXml(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("dataValidation"));

    static QMap<QString, DataValidation::ValidationType> typeMap;
    static QMap<QString, DataValidation::ValidationOperator> opMap;
    static QMap<QString, DataValidation::ErrorStyle> esMap;
    if (typeMap.isEmpty()) {
        typeMap.insert(QString("none"), DataValidation::None);
        typeMap.insert(QString("whole"), DataValidation::Whole);
        typeMap.insert(QString("decimal"), DataValidation::Decimal);
        typeMap.insert(QString("list"), DataValidation::List);
        typeMap.insert(QString("date"), DataValidation::Date);
        typeMap.insert(QString("time"), DataValidation::Time);
        typeMap.insert(QString("textLength"), DataValidation::TextLength);
        typeMap.insert(QString("custom"), DataValidation::Custom);

        opMap.insert(QString("between"), DataValidation::Between);
        opMap.insert(QString("notBetween"), DataValidation::NotBetween);
        opMap.insert(QString("equal"), DataValidation::Equal);
        opMap.insert(QString("notEqual"), DataValidation::NotEqual);
        opMap.insert(QString("lessThan"), DataValidation::LessThan);
        opMap.insert(QString("lessThanOrEqual"), DataValidation::LessThanOrEqual);
        opMap.insert(QString("greaterThan"), DataValidation::GreaterThan);
        opMap.insert(QString("greaterThanOrEqual"), DataValidation::GreaterThanOrEqual);

        esMap.insert(QString("stop"), DataValidation::Stop);
        esMap.insert(QString("warning"), DataValidation::Warning);
        esMap.insert(QString("information"), DataValidation::Information);
    }

    DataValidation validation;
    QXmlStreamAttributes attrs = reader.attributes();

    QString sqref = attrs.value(QLatin1String("sqref")).toString();
    foreach (QString range, sqref.split(QLatin1Char(' ')))
        validation.addRange(range);

    if (attrs.hasAttribute(QLatin1String("type"))) {
        QString t = attrs.value(QLatin1String("type")).toString();
        validation.setValidationType(typeMap.contains(t) ? typeMap[t] : DataValidation::None);
    }
    if (attrs.hasAttribute(QLatin1String("errorStyle"))) {
        QString es = attrs.value(QLatin1String("errorStyle")).toString();
        validation.setErrorStyle(esMap.contains(es) ? esMap[es] : DataValidation::Stop);
    }
    if (attrs.hasAttribute(QLatin1String("operator"))) {
        QString op = attrs.value(QLatin1String("operator")).toString();
        validation.setValidationOperator(opMap.contains(op) ? opMap[op] : DataValidation::Between);
    }
    if (attrs.hasAttribute(QLatin1String("allowBlank"))) {
        validation.setAllowBlank(true);
    } else {
        validation.setAllowBlank(false);
    }
    if (attrs.hasAttribute(QLatin1String("showInputMessage"))) {
        validation.setPromptMessageVisible(true);
    } else {
        validation.setPromptMessageVisible(false);
    }
    if (attrs.hasAttribute(QLatin1String("showErrorMessage"))) {
        validation.setErrorMessageVisible(true);
    } else {
        validation.setErrorMessageVisible(false);
    }

    QString et = attrs.value(QLatin1String("errorTitle")).toString();
    QString e = attrs.value(QLatin1String("error")).toString();
    if (!e.isEmpty() || !et.isEmpty())
        validation.setErrorMessage(e, et);

    QString pt = attrs.value(QLatin1String("promptTitle")).toString();
    QString p = attrs.value(QLatin1String("prompt")).toString();
    if (!p.isEmpty() || !pt.isEmpty())
        validation.setPromptMessage(p, pt);

    //find the end
    while(!(reader.name() == QLatin1String("dataValidation") && reader.tokenType() == QXmlStreamReader::EndElement)) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("formula1")) {
                validation.setFormula1(reader.readElementText());
            } else if (reader.name() == QLatin1String("formula2")) {
                validation.setFormula2(reader.readElementText());
            }
        }
    }
    return validation;
}

QT_END_NAMESPACE_XLSX

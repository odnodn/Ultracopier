#include "fileIsSameDialog.h"
#include "ui_fileIsSameDialog.h"

#include <QDebug>

fileIsSameDialog::fileIsSameDialog(QWidget *parent,QFileInfo fileInfo) :
	QDialog(parent),
	ui(new Ui::fileIsSameDialog)
{
	ui->setupUi(this);
	action=FileExists_Cancel;
	oldName=fileInfo.fileName();
	destinationInfo=fileInfo;
	ui->lineEditNewName->setText(oldName);
	ui->lineEditNewName->setPlaceholderText(oldName);
	ui->label_content_size->setText(QString::number(fileInfo.size()));
	ui->label_content_modified->setText(fileInfo.lastModified().toString());
	ui->label_content_file_name->setText(fileInfo.fileName());
	updateRenameButton();
}

fileIsSameDialog::~fileIsSameDialog()
{
	delete ui;
}

void fileIsSameDialog::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

QString fileIsSameDialog::getNewName()
{
	if(oldName==ui->lineEditNewName->text() || ui->checkBoxAlways->isChecked())
		qDebug() << "fileIsSameDialog, return the old name: "+oldName;
	else
		qDebug() << "fileIsSameDialog, return the new name: "+ui->lineEditNewName->text();
	if(oldName==ui->lineEditNewName->text() || ui->checkBoxAlways->isChecked())
		return oldName;
	else
		return ui->lineEditNewName->text();
}

void fileIsSameDialog::on_SuggestNewName_clicked()
{
	QFileInfo destinationInfo=this->destinationInfo;
	QString absolutePath=destinationInfo.absolutePath();
	QString fileName=destinationInfo.fileName();
	QString suffix="";
	QString destination;
	if(fileName.contains(QRegExp("^(.*)(\\.[a-z0-9]+)$")))
	{
		suffix=fileName;
		suffix.replace(QRegExp("^(.*)(\\.[a-z0-9]+)$"),"\\2");
		fileName.replace(QRegExp("^(.*)(\\.[a-z0-9]+)$"),"\\1");
	}
	do
	{
		if(!fileName.startsWith(tr("Copy of ")))
			fileName=tr("Copy of ")+fileName;
		else
		{
			if(fileName.contains(QRegExp("_[0-9]+$")))
			{
				QString number=fileName;
				number.replace(QRegExp("^.*_([0-9]+)$"),"\\1");
				int num=number.toInt()+1;
				fileName.remove(QRegExp("[0-9]+$"));
				fileName+=QString::number(num);
			}
			else
				fileName+="_2";
		}
		destination=absolutePath+QDir::separator()+fileName+suffix;
		destinationInfo.setFile(destination);
	}
	while(destinationInfo.exists());
	ui->lineEditNewName->setText(fileName+suffix);
}

void fileIsSameDialog::on_Rename_clicked()
{
	action=FileExists_Rename;
	this->close();
}

void fileIsSameDialog::on_Skip_clicked()
{
	action=FileExists_Skip;
	this->close();
}

void fileIsSameDialog::on_Cancel_clicked()
{
	action=FileExists_Cancel;
	this->close();
}

FileExistsAction fileIsSameDialog::getAction()
{
	return action;
}

bool fileIsSameDialog::getAlways()
{
	return ui->checkBoxAlways->isChecked();
}

void fileIsSameDialog::updateRenameButton()
{
	ui->Rename->setEnabled(ui->checkBoxAlways->isChecked() || (oldName!=ui->lineEditNewName->text() && !ui->lineEditNewName->text().isEmpty()));
}

void fileIsSameDialog::on_lineEditNewName_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	updateRenameButton();
}

void fileIsSameDialog::on_checkBoxAlways_toggled(bool checked)
{
	Q_UNUSED(checked);
	updateRenameButton();
}
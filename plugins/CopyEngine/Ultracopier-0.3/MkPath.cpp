#include "MkPath.h"

MkPath::MkPath()
{
	stopIt=false;
	waitAction=false;
	setObjectName("MkPath");
	start();
}

MkPath::~MkPath()
{
	stopIt=true;
	quit();
	wait();
}

void MkPath::addPath(QString path)
{
	if(stopIt)
		return;
	emit internalStartAddPath(path);
}

void MkPath::skip()
{
	emit internalStartSkip();
}

void MkPath::retry()
{
	emit internalStartRetry();
}

void MkPath::run()
{
	connect(this,SIGNAL(internalStartAddPath(QString)),this,SLOT(internalAddPath(QString)));
	connect(this,SIGNAL(internalStartDoThisPath()),this,SLOT(internalDoThisPath()));
	connect(this,SIGNAL(internalStartSkip()),this,SLOT(internalSkip()));
	connect(this,SIGNAL(internalStartRetry()),this,SLOT(internalRetry()));
	exec();
}

void MkPath::internalDoThisPath()
{
	if(!dir.mkpath(pathList.first()))
	{
		if(stopIt)
			return;
		waitAction=false;
		emit errorOnFolder(pathList.first(),tr("Unable to create the folder"));
		return;
	}
	pathList.removeFirst();
	emit firstFolderFinish();
	checkIfCanDoTheNext();
}

void MkPath::internalAddPath(QString path)
{
	pathList << path;
	checkIfCanDoTheNext();
}

void MkPath::checkIfCanDoTheNext()
{
	if(!waitAction && !stopIt && pathList.size()>0)
		emit internalStartDoThisPath();
}

void MkPath::internalSkip()
{
	waitAction=false;
	pathList.removeFirst();
	emit firstFolderFinish();
	checkIfCanDoTheNext();
}

void MkPath::internalRetry()
{
	waitAction=false;
	checkIfCanDoTheNext();
}

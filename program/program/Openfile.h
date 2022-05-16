#pragma once

#include <QObject>

class Openfile : public QObject
{
	Q_OBJECT

public:
	Openfile(QObject *parent);
	~Openfile();
};

#pragma once

#include <QString>
#include <QUuid>

class EmployeeModel {
public:
	EmployeeModel();
	~EmployeeModel();

	void assign(const EmployeeModel);
	void operator=(const EmployeeModel);

	QUuid id;
	QString name;
	QString surname;
	QString phoneNumHome;
	QString phoneNumWork;
	QString emailWork;
	QString emailHome;
	QString office;
	bool isDoctoral;
	double harnessType; // typ úvazku
	int workPointsNum;
};
#pragma once

#include <QString>
#include <QList>
#include <QUuid>

class GroupModel
{
public:
	GroupModel();
	~GroupModel();

	void assign(const GroupModel);
	void operator=(const GroupModel);

	QString shortcut;
	QString name;
	bool isCombined;
	int year;
	int semester;
	int studentsNum;
	int studyForm;

	QList<QUuid> subjects;
};
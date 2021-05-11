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

	QUuid id;
	QString shortcut;
	QString name;
	bool isCombined;
	int year;
    bool isWinterSemester;
	int studentsNum;
	int studyForm;

	QList<QUuid> subjects;
};

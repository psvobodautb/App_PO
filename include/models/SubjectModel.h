#pragma once

#include <QString>
#include <QUuid>

class SubjectModel {
public:
	SubjectModel();
	~SubjectModel();

	void assign(const SubjectModel);
	void operator=(const SubjectModel);

    QUuid id;
	QString shortcut;
	QString name;
	int lecturesNum;
    int seminarsNum;
    int excercisesNum;
    int weeksNum;
    bool isEnglish;
    bool isCombined;
    int studyYear;
    bool isWinterSemester;
    int ending;
    int groupSize;
    int credits;
};


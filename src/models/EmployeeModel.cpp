#include "include/models/EmployeeModel.h"

EmployeeModel::EmployeeModel()
{
}

EmployeeModel::~EmployeeModel()
{
}

void EmployeeModel::assign(const EmployeeModel m )
{
	id = m.id;
	name = m.name;
	surname = m.surname;
	phoneNumHome = m.phoneNumHome;
	phoneNumWork = m.phoneNumWork;
	emailWork = m.emailWork;
	emailHome = m.emailHome;
	office = m.office;
	isDoctoral = m.isDoctoral;
	harnessType = m.harnessType;
	workPointsNum = m.workPointsNum;
}

void EmployeeModel::operator=(const EmployeeModel m)
{
	assign(m);
}

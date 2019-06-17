#ifndef EXAMPROP_H
#define EXAMPROP_H

#include <QDialog>
#include "MySQLSimpleDriver.hpp"

using Query = std::shared_ptr<querryResult>;

namespace Ui {
class ExamProp;
}

class ExamProp : public QDialog
{
	Q_OBJECT

public:

	static const QString pracQuer;
	static const QString regioQuert;

    explicit ExamProp(size_t id, Query prac, Query regio, QWidget *parent = nullptr);
	~ExamProp();

private slots:
	void on_pushButton_clicked();

private:
	Ui::ExamProp *ui;
};

#endif // EXAMPROP_H

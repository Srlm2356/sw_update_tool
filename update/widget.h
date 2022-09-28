#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QProcess>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QPushButton* pbComputers;
    QPushButton* pbDisk;
    QPushButton* pbStart;
    QPushButton* pbClose;

    QComboBox* boxComputer;
    QComboBox* boxDisk;

private:
    Ui::Widget *ui;

    QProcess* process;
    QLineEdit* command;
    QTextEdit* display;

    QString listGetversion, listGetmedia, listUpdate;


public slots:
    void slotClose()
    {
        close();
    }

    void slot_boxDisk_activated()
    {
        if(boxDisk->currentIndex() != -1)
            pbDisk->setEnabled(true);
        if((boxDisk->currentIndex() != -1) & (boxComputer->currentIndex() != -1))
            pbStart->setEnabled(true);
    }

    void slot_boxComputer_activated()
    {
        if(boxComputer->currentIndex() != -1)
            pbComputers->setEnabled(true);
        if((boxComputer->currentIndex() != -1) & (boxDisk->currentIndex() != -1))
            pbStart->setEnabled(true);
    }

    void slotDataOnStdout()
    {
        display->append(process->readAllStandardOutput());
    }

    void slot_pushButtonComputers()
    {
        display->clear();
        QString strCommand = "";
        strCommand = getListVersion();
        process->start(strCommand);

        display->append("Computer: " + boxComputer->currentText() + "\n");
        display->append("Program: " + strCommand + "\n");
    }

    void slot_pushButtonDisk()
    {
        display->clear();
        QString strCommand = "";
        strCommand = getListGetmedia();
        process->start(strCommand);

        display->append("Computer: " + boxComputer->currentText() + "\n");
        display->append("Media: " + boxDisk->currentText() + "\n");
        display->append("Program: "+ strCommand + "\n");
    }

    void slot_pushButtonUpdate()
    {
        display->clear();
        QString strCommand = "";
        strCommand = getListUpdate();
        process->start(strCommand);

        display->append("Computer: " + boxComputer->currentText() + "\n");
        display->append("Media: " + boxDisk->currentText() + "\n");
        display->append("Program: "+ strCommand + "\n");
    }

public:
    QString directory_name();
    QString getListVersion();
    QString getListGetmedia();
    QString getListUpdate();
};
#endif // WIDGET_H

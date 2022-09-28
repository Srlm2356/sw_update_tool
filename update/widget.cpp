#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDir>
#include <QtXml/QDomDocument>
#include <QMessageBox>

//QString listGetversion, listGetmedia, listUpdate;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    if(qApp->arguments().size() == 6){
        int argX = qApp->arguments().at(3).toInt();
        int argY = qApp->arguments().at(5).toInt();
        setGeometry(argX, argY, width(), height());
    }
    if(qApp->arguments().size() == 5){
        int argX = qApp->arguments().at(2).toInt();
        int argY = qApp->arguments().at(4).toInt();
        setGeometry(argX, argY, width(), height());
    }

    ui->setupUi(this);

    process = new QProcess(this);
    display = new QTextEdit; // Окно вывода
    display->setFixedHeight(90);

    QLabel* lableC = new QLabel(QString::fromUtf8("Выбор компьютера"));
    QLabel* lableD = new QLabel(QString::fromUtf8("Выбор носителя информации"));

    pbComputers = new QPushButton(QString::fromUtf8("Запрос текущей версии"));
    pbDisk = new QPushButton(QString::fromUtf8("Опрос носителя"));
    pbStart = new QPushButton(QString::fromUtf8("Обновить"));
    pbClose = new QPushButton(QString::fromUtf8("Закрыть"));

    boxDisk = new QComboBox;
    boxComputer =  new QComboBox;

    pbComputers->setEnabled(false);
    pbDisk->setEnabled(false);
    pbStart->setEnabled(false);

    connect(process, SIGNAL(readyReadStandardOutput()), SLOT(slotDataOnStdout()));

    connect(pbClose, SIGNAL(clicked()), SLOT(slotClose())); //Закрыть приложение
    connect(boxDisk, SIGNAL(activated(int)), SLOT(slot_boxDisk_activated())); // Проверка заполненности Box
    connect(boxComputer, SIGNAL(activated(int)), SLOT(slot_boxComputer_activated())); // Проверка заполненности Box

    connect(pbComputers, SIGNAL(clicked()), SLOT(slot_pushButtonComputers())); // Запустить процесс с комп
    connect(pbDisk, SIGNAL(clicked()), SLOT(slot_pushButtonDisk())); // Запустить процесс с диск
    connect(pbStart, SIGNAL(clicked()), SLOT(slot_pushButtonUpdate())); // Запустить процесс с диск+комп

     QHBoxLayout* hbLayout = new QHBoxLayout; //Формат горизонтальный
     hbLayout->addWidget(pbStart);
     //hbLayout->addSpacerItem(spacer);
     hbLayout->addWidget(pbClose);

     QVBoxLayout* vbLayout = new QVBoxLayout; // Формат вертикальный
     vbLayout->addWidget(lableC);
     vbLayout->addWidget(boxComputer);
     vbLayout->addWidget(pbComputers);
     vbLayout->addWidget(display);
     vbLayout->addWidget(lableD);
     vbLayout->addWidget(boxDisk);
     vbLayout->addWidget(pbDisk);
     vbLayout->addLayout(hbLayout);
     setLayout(vbLayout);

     QString fileName = directory_name();

     QFile file(fileName);
     QDomDocument doc;

     if(!file.open(QIODevice::ReadOnly)){
         QMessageBox::warning(this,
                              QString::fromUtf8("Ошибка файла"),
                              QString::fromUtf8("Не удалось открыть файл ") + fileName,
                              QMessageBox::Ok);
         return;
     }
     else{
         if(doc.setContent(&file)){
             QDomElement node = doc.documentElement();
             QDomNode domNode = node.firstChild();

             while(!domNode.isNull()){
                 QDomElement domElement = domNode.toElement();

                 if(domElement.tagName() == "Computers"){
                     QDomNode n = domElement.firstChild();
                     while(!n.isNull()){
                         QDomElement e = n.toElement();
                         if(e.tagName() == "hostname"){
                             boxComputer->addItem(e.text());
                         }
                         n = n.nextSibling();
                     }
                 }
                 if(domElement.tagName() == "Media"){
                     QDomNode n = domElement.firstChild();
                     while(!n.isNull()){
                         QDomElement e = n.toElement();
                         if(e.tagName() == "medianame"){
                             boxDisk->addItem(e.text());
                         }
                         n = n.nextSibling();
                     }
                 }
                 if(domElement.tagName() == "Getversion"){
                     QDomNode n = domElement.firstChild();
                     while(!n.isNull()){
                         QDomElement e = n.toElement();
                         if(e.tagName() == "getrVersionName"){
                             listGetversion = (e.text());
                         }
                         n = n.nextSibling();
                     }
                 }
                 if(domElement.tagName() == "Getmedia"){
                     QDomNode n = domElement.firstChild();
                     while(!n.isNull()){
                         QDomElement e = n.toElement();
                         if(e.tagName() == "getMediaName"){
                             listGetmedia = (e.text());
                         }
                         n = n.nextSibling();
                     }
                 }
                 if(domElement.tagName() == "Update"){
                     QDomNode n = domElement.firstChild();
                     while(!n.isNull()){
                         QDomElement e = n.toElement();
                         if(e.tagName() == "updateName"){
                             listUpdate = (e.text());
                         }
                         n = n.nextSibling();
                     }
                 }
                 domNode = domNode.nextSibling();
             }
         }
         file.close();
     }

     boxComputer->setCurrentIndex(-1);
     boxDisk->setCurrentIndex(-1);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::directory_name()
{
    QDir dirName;
    QString string;
    QString fileName = "";

    QRegExp maska("*.xml");
    maska.setPatternSyntax(QRegExp::Wildcard);

    if((qApp->arguments().size() == 2) || (qApp->arguments().size() == 6)){
        if(maska.exactMatch(qApp->arguments().at(1)) != 0)
            fileName = qApp->arguments().at(1).toUtf8();
        else
            string = qApp->arguments().at(1).toUtf8();
    }
    else
        string = QDir::currentPath();

    dirName = string;

    if(dirName.exists())
    {
        QStringList list = dirName.entryList(); //Списочек файликов

        if(fileName == ""){
            for(int i=0; i!=list.size(); i++){
                if(maska.exactMatch(list[i]) !=0)
                    fileName = list[i];
            }
        }
        return QString(string + "/" + fileName);
    }
    return fileName;
}

QString Widget::getListVersion()
{
    static QString string1;
    if(string1 == ""){
        listGetversion.replace(QString("$hostname"), QString(boxComputer->currentText()));
        string1 = boxComputer->currentText();
    }
    else{
        listGetversion.replace(QString(string1), QString(boxComputer->currentText()));
        string1 = boxComputer->currentText();
    }
    return listGetversion;
}

QString Widget::getListGetmedia()
{
    static QString string1;
    if(string1 == ""){
        listGetmedia.replace(QString("$medianame"), QString(boxDisk->currentText()));
        string1 = boxDisk->currentText();
    }
    else{
        listGetmedia.replace(QString(string1), QString(boxDisk->currentText()));
        string1 = boxDisk->currentText();
    }
    return listGetmedia;
}

QString Widget::getListUpdate()
{
    static QString string1;
    if(string1 == ""){
        listUpdate.replace(QString("$hostname"), QString(boxComputer->currentText()));
        string1 = boxComputer->currentText();
    }
    else{
        listUpdate.replace(QString(string1), QString(boxComputer->currentText()));
        string1 = boxComputer->currentText();
    }

    static QString string2;
    if(string2 == ""){
        listUpdate.replace(QString("$medianame"), QString(boxDisk->currentText()));
        string2 = boxDisk->currentText();
    }
    else{
        listUpdate.replace(QString(string2), QString(boxDisk->currentText()));
        string2 = boxDisk->currentText();
    }
    return listUpdate;
}

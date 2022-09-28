#include <QCoreApplication>
#include <QtXml/QDomDocument>
#include <QDebug>
#include <QFile>

QDomElement makeElement(QDomDocument domDoc, const QString& name, const QString& attr = QString(), const QString& text = QString())
{
    QDomElement domElement = domDoc.createElement(name);

    if(!attr.isEmpty()){
        QDomAttr domAttr = domDoc.createAttribute("id");
        domAttr.setValue(attr);
        domElement.setAttributeNode(domAttr);
    }

    if(!text.isEmpty()){
        QDomText domText = domDoc.createTextNode(text);
        domElement.appendChild(domText);
    }
    return domElement;
}

QDomElement nameElement(QDomDocument& domDoc, QString nameElements, const QString& name)
{
    static int nNumber = 1;
    static QString checkName;

    if(checkName == nameElements){
            nNumber++;
            checkName = nameElements;
    }
    else {
        nNumber = 1;
        checkName = nameElements;
    }

    QDomElement domElement = makeElement(domDoc, nameElements, QString().setNum(nNumber));
    domElement.appendChild(makeElement(domDoc, "name", "", name));

    return domElement;
}

int main(int argc, char *argv[])
{
    if(argc <= 1){
        qDebug() << "The name of the configuration file isn't specified ";
        exit(1);
    }
    else{
        qDebug() << "Successfully";
        //return -1;
    }
    QString nameFile;
    nameFile = argv[1];

    QDomDocument doc;;

    QDomElement domElement = doc.createElement("App");
    QDomElement computers = doc.createElement("Computers");
    QDomElement media = doc.createElement("Media");
    QDomElement getversion = doc.createElement("Getversion");
    QDomElement getmedia = doc.createElement("Getmedia");
    QDomElement update = doc.createElement("Update");


    doc.appendChild(domElement);
    domElement.appendChild(doc.createComment("This section for choosing $hostname"));
    domElement.appendChild(computers);
    domElement.appendChild(doc.createComment("This section for choosing $medianame"));
    domElement.appendChild(media);
    domElement.appendChild(doc.createComment("This section for getting current version state"));
    domElement.appendChild(getversion);
    domElement.appendChild(doc.createComment("this section for getting media info"));
    domElement.appendChild(getmedia);
    domElement.appendChild(doc.createComment("This section for starting SW update"));
    domElement.appendChild(update);

    computers.appendChild(nameElement(doc, "hostname", "cpc11lt1"));
    computers.appendChild(nameElement(doc, "hostname", "cpc12lt1"));
    computers.appendChild(nameElement(doc, "hostname", "cpc13lt1"));

    media.appendChild(nameElement(doc, "medianame", "usb-flash"));
    media.appendChild(nameElement(doc, "medianame", "sd-flash"));

    getversion.appendChild(nameElement(doc, "getrVersionName", "ssh pcs1@$hostname check_sum"));

    getmedia.appendChild(nameElement(doc, "getMediaName", "et_media_info.scr $medianame"));

    update.appendChild(nameElement(doc, "updateName", "remote_sw_install.scr $hostname root pcs1 $medianame  /tmp"));

    QFile file(nameFile+".xml");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << doc.toString();
        file.close();
    }
    return 0;
}

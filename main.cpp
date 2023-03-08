#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleSheetFile(":/styleSheet.qss");
    styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheetStream(&styleSheetFile);
    QString styleSheet = styleSheetStream.readAll();

    a.setStyleSheet(styleSheet);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "WordListMaker_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}

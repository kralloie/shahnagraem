#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QFileDialog>
#include <QFile>
#include <QCryptographicHash>
#include <QHBoxLayout>
#include <QTextStream>
#include <QPalette>
#include <unordered_map>
#include <QMessageBox>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:

        void on_openFileButton_clicked();

        void on_openTargetFileButton_clicked();

        void on_generateButton_clicked();

        void on_openHashList_clicked();

        void on_openWordList_clicked();

        void on_hashInput_editingFinished();

        void on_findButton_clicked();

        void on_inputText_textChanged();

        void on_inputModeBox_currentIndexChanged(int index);

    private:
        Ui::MainWindow *ui;

        //Wordlist generator variables.
        QString wordListPath_Gen;
        QString targetFilePath_Gen;

        //Find coincidence function variables.
        QString hashListPath_Find;
        QString wordListPath_Find;
        QFile wordListFile_Find;
        QFile hashListFile_Find;
        bool wordListOpen = false;
        bool hashListOpen = false;
        QMessageBox messageBox;
        QString hash;
        QMap<QString,QCryptographicHash::Algorithm> methodMap;

};
#endif // MAINWINDOW_H

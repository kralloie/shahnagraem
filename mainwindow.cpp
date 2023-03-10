#include "mainwindow.h"
#include "ui_mainwindow.h"
QString sha224(QString str)
{
    QByteArray myData = str.toUtf8();
    QByteArray myHash = QCryptographicHash::hash(myData, QCryptographicHash::Sha224);
    QString result = myHash.toHex();
    return result;
}

QString sha512(QString str)
{
    QByteArray myData = str.toUtf8();
    QByteArray myHash = QCryptographicHash::hash(myData, QCryptographicHash::Sha512);
    QString result = myHash.toHex();
    return result;
}

QString sha384(QString str)
{
    QByteArray myData = str.toUtf8();
    QByteArray myHash = QCryptographicHash::hash(myData, QCryptographicHash::Sha384);
    QString result = myHash.toHex();
    return result;
}

QString sha1(QString str)
{
    QByteArray myData = str.toUtf8();
    QByteArray myHash = QCryptographicHash::hash(myData, QCryptographicHash::Sha1);
    QString result = myHash.toHex();
    return result;
}

QString sha256(QString str)
{
    QByteArray myData = str.toUtf8();
    QByteArray myHash = QCryptographicHash::hash(myData, QCryptographicHash::Sha256);
    QString result = myHash.toHex();
    return result;
}

QString md5(QString str)
{
    QByteArray myData = str.toUtf8();
    QByteArray myHash = QCryptographicHash::hash(myData, QCryptographicHash::Md5);
    QString result = myHash.toHex();
    return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QColor baseColor(QStringLiteral("#32383D"));
    QColor complementColor(QStringLiteral("#202529"));
    QColor disabledColor(QStringLiteral("#4C555C"));
    QColor highlightColor(QStringLiteral("#009BFF"));
    QColor linkColor(QStringLiteral("#009BFF"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Active, QPalette::Button, disabledColor.darker());
    darkPalette.setColor(QPalette::AlternateBase, complementColor);
    darkPalette.setColor(QPalette::Base, baseColor);
    darkPalette.setColor(QPalette::Button, complementColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::Disabled, QPalette::Light, complementColor);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, disabledColor);
    darkPalette.setColor(QPalette::Highlight, highlightColor);
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Link, linkColor);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::ToolTipBase, highlightColor);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Window, complementColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    QApplication::setPalette(darkPalette);

    ui->outputText->setReadOnly(true);

    ui->inputModeBox->addItem("File");
    ui->inputModeBox->addItem("Text");

    ui->hashBox->addItem("SHA1");
    ui->hashBox->addItem("MD5");
    ui->hashBox->addItem("SHA256");
    ui->hashBox->addItem("SHA384");
    ui->hashBox->addItem("SHA512");
    ui->hashBox->addItem("SHA224");

    methodMap.emplace("MD5",&::md5);
    methodMap.emplace("SHA256",&::sha256);
    methodMap.emplace("SHA1",&::sha1);
    methodMap.emplace("SHA384",&::sha384);
    methodMap.emplace("SHA512",&::sha512);
    methodMap.emplace("SHA224",&::sha224);


    this->setWindowFlags(Qt::Window);
    this->setFixedSize(790,620);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Open File","","*.txt");
    wordListPath_Gen = filePath;
    ui->wordListPathlb->setText(filePath);
}


void MainWindow::on_openTargetFileButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"Open File","","*.txt");
    targetFilePath_Gen = filePath;
    ui->targetPathlb->setText(filePath);
}


void MainWindow::on_generateButton_clicked()
{
    QFile targetFile(targetFilePath_Gen);
    QFile wordListFileBase(wordListPath_Gen);
    targetFile.open(QIODevice::ReadWrite | QIODevice::Text);
    wordListFileBase.open(QIODevice::ReadWrite | QIODevice::Text);
    targetFile.resize(0);
    QTextStream wordListStream(&wordListFileBase);
    QTextStream targetFileStream(&targetFile);
    hashFunc = methodMap[ui->hashBox->currentText()];

    bool targetMissing = !targetFile.isOpen();
    bool wordlistMissing = !wordListFileBase.isOpen();

    if(targetMissing || wordlistMissing)
    {
        QString errorMessage = (targetMissing && wordlistMissing) ? "Word list file and Target file are missing!" :(targetMissing) ? "Target file is missing!" : "Word list is missing!";
        QMessageBox::warning(this,"File(s) missing",errorMessage);
        return;
    }


    while (!wordListStream.atEnd())
    {
        QString line = wordListStream.readLine();
        if(!line.isEmpty())
        {
            QString data = hashFunc(line);
            targetFileStream << data << '\n';
        }
    }

    targetFile.close();
    wordListFileBase.close();
    QMessageBox::information(this,"Success","The word list has been generated and stored successfully.");
}


void MainWindow::on_openHashList_clicked()
{
    hashListPath_Find = QFileDialog::getOpenFileName(this,"Open File","","*.txt");
    hashListFile_Find.setFileName(hashListPath_Find);
    hashListFile_Find.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
    hashListOpen = hashListFile_Find.isOpen();
    ui->hashListPathlb->setText(hashListPath_Find);
}


void MainWindow::on_openWordList_clicked()
{
    wordListPath_Find = QFileDialog::getOpenFileName(this,"Open File","","*.txt");
    wordListFile_Find.setFileName(wordListPath_Find);
    wordListFile_Find.open(QIODevice::ReadOnly | QIODevice::Text);
    wordListOpen = wordListFile_Find.isOpen();
    ui->wordListPathlb_2->setText(wordListPath_Find);
}


void MainWindow::on_hashInput_editingFinished()
{
    hash = ui->hashInput->text();
}

void MainWindow::on_findButton_clicked()
{
    if(!hashListOpen || !wordListOpen)
    {
        QString errorMessage = (!hashListOpen && !wordListOpen) ? "Word list and Hash list are missing!" :(!wordListOpen) ? "Word list is missing!" : "Hash List is missing!";
        QMessageBox::warning(this,"List(s) missing",errorMessage);
        return;
    }
    else if(hash.isEmpty())
    {
        QMessageBox::warning(this,"Parameter missing","Please input your hash for the comparison.");
        return;
    }

    QTextStream hashStream(&hashListFile_Find);
    QTextStream wordListStream(&wordListFile_Find);

    while(!hashStream.atEnd() && !wordListStream.atEnd())
    {
        QString hashLine = hashStream.readLine();
        QString wordLine = wordListStream.readLine();
        if(hash == hashLine)
        {
            QString popmsg = "The hash matches with the word: " + wordLine;
            QMessageBox::information(this,"\\^o^/",popmsg);
            return;
        }
    }

    QMessageBox::critical(this,"＞︿＜","No coincidence found.");
}

void MainWindow::on_inputText_textChanged()
{
    ui->outputText->setPlainText("");
    hashFunc = methodMap[ui->hashBox->currentText()];
    QString input = ui->inputText->toPlainText();
    QStringList inputSplitted = input.split('\n');
    for(QString& str : inputSplitted)
    {
        if(!str.isEmpty())
        {
            ui->outputText->insertPlainText(hashFunc(str) + '\n');
        }
    }
}


void MainWindow::on_inputModeBox_currentIndexChanged(int index)
{
    ui->frame_5->setVisible(index);
    ui->frame_1->setHidden(index);
}


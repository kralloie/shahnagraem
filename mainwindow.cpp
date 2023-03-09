#include "mainwindow.h"
#include "ui_mainwindow.h"
QString sha224(std::string str)
{
    unsigned char hash[SHA224_DIGEST_LENGTH];
    std::string encryptedText;

    SHA224((unsigned char*)str.c_str(), str.length(), hash);

    char hexHash[SHA224_DIGEST_LENGTH * 2 + 1];

    for (int i = 0; i < SHA224_DIGEST_LENGTH; i++) {
        sprintf(hexHash + i * 2, "%02x", hash[i]);
    }

    encryptedText = std::string(hexHash);

    return QString::fromStdString(encryptedText);
}

QString sha512(std::string str)
{
    unsigned char hash[SHA512_DIGEST_LENGTH];
    std::string encryptedText;

    SHA512((unsigned char*)str.c_str(), str.length(), hash);

    char hexHash[SHA512_DIGEST_LENGTH * 2 + 1];

    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        sprintf(hexHash + i * 2, "%02x", hash[i]);
    }

    encryptedText = std::string(hexHash);

    return QString::fromStdString(encryptedText);
}

QString sha384(std::string str)
{
    unsigned char hash[SHA384_DIGEST_LENGTH];
    std::string encryptedText;

    SHA384((unsigned char*)str.c_str(), str.length(), hash);

    char hexHash[SHA384_DIGEST_LENGTH * 2 + 1];

    for (int i = 0; i < SHA384_DIGEST_LENGTH; i++) {
        sprintf(hexHash + i * 2, "%02x", hash[i]);
    }

    encryptedText = std::string(hexHash);

    return QString::fromStdString(encryptedText);
}

QString sha160(std::string str)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    std::string encryptedText;

    SHA1((unsigned char*)str.c_str(), str.length(), hash);

    char hexHash[SHA_DIGEST_LENGTH * 2 + 1];

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hexHash + i * 2, "%02x", hash[i]);
    }

    encryptedText = std::string(hexHash);

    return QString::fromStdString(encryptedText);
}

QString sha256(std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string encryptedText;

    SHA256((unsigned char*)str.c_str(), str.length(), hash);

    char hexHash[SHA256_DIGEST_LENGTH * 2 + 1];

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hexHash + i * 2, "%02x", hash[i]);
    }

    encryptedText = std::string(hexHash);

    return QString::fromStdString(encryptedText);
}

QString md5(std::string str)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);
    EVP_DigestUpdate(mdctx, str.c_str(), str.size());
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;
    EVP_DigestFinal_ex(mdctx, hash, &hashLen);


    std::stringstream StrStream;
    StrStream << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < hashLen; ++i) {
        StrStream << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }

    EVP_MD_CTX_free(mdctx);

    return QString::fromStdString(StrStream.str());
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

    ui->comboBox->addItem("SHA160");
    ui->comboBox->addItem("MD5");
    ui->comboBox->addItem("SHA256");
    ui->comboBox->addItem("SHA384");
    ui->comboBox->addItem("SHA512");
    ui->comboBox->addItem("SHA224");

    methodMap.emplace("MD5",&::md5);
    methodMap.emplace("SHA256",&::sha256);
    methodMap.emplace("SHA160",&::sha160);
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
    hashFunc = methodMap[ui->comboBox->currentText()];

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
            QString data = hashFunc(line.toStdString());
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




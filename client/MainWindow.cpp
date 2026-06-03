#include "MainWindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    network = new QNetworkAccessManager(this);

    connect(network,
            &QNetworkAccessManager::finished,
            this,
            &MainWindow::handleReply);

    QWidget *central = new QWidget(this);

    QVBoxLayout *mainLayout =
        new QVBoxLayout();

    //
    // AUTH
    //

    QGroupBox *authBox =
        new QGroupBox("Authorization");

    QVBoxLayout *authLayout =
        new QVBoxLayout();

    loginEdit =
        new QLineEdit();

    passwordEdit =
        new QLineEdit();

    passwordEdit->setEchoMode(
        QLineEdit::Password);

    registerButton =
        new QPushButton("Register");

    loginButton =
        new QPushButton("Login");

    authLayout->addWidget(
        new QLabel("Login"));

    authLayout->addWidget(loginEdit);

    authLayout->addWidget(
        new QLabel("Password"));

    authLayout->addWidget(passwordEdit);

    authLayout->addWidget(registerButton);
    authLayout->addWidget(loginButton);

    authBox->setLayout(authLayout);

    //
    // MD5
    //

    QGroupBox *md5Box =
        new QGroupBox("MD5");

    QVBoxLayout *md5Layout =
        new QVBoxLayout();

    md5Text =
        new QLineEdit();

    md5Button =
        new QPushButton("Calculate MD5");

    md5Layout->addWidget(md5Text);
    md5Layout->addWidget(md5Button);

    md5Box->setLayout(md5Layout);

    //
    // VIGENERE
    //

    QGroupBox *vigBox =
        new QGroupBox("Vigenere");

    QVBoxLayout *vigLayout =
        new QVBoxLayout();

    vigenereText =
        new QLineEdit();

    vigenereKey =
        new QLineEdit();

    vigenereButton =
        new QPushButton("Encrypt");

    vigLayout->addWidget(
        new QLabel("Text"));

    vigLayout->addWidget(vigenereText);

    vigLayout->addWidget(
        new QLabel("Key"));

    vigLayout->addWidget(vigenereKey);

    vigLayout->addWidget(vigenereButton);

    vigBox->setLayout(vigLayout);

    //
    // SECANT
    //

    QGroupBox *secantBox =
        new QGroupBox("Secant");

    QVBoxLayout *secantLayout =
        new QVBoxLayout();

    secantA =
        new QLineEdit();

    secantB =
        new QLineEdit();

    secantButton =
        new QPushButton("Solve");

    secantLayout->addWidget(
        new QLabel("A"));

    secantLayout->addWidget(secantA);

    secantLayout->addWidget(
        new QLabel("B"));

    secantLayout->addWidget(secantB);

    secantLayout->addWidget(secantButton);

    secantBox->setLayout(secantLayout);

    //
    // GRAPH
    //

    QGroupBox *graphBox =
        new QGroupBox("Graph Cycle");

    QVBoxLayout *graphLayout =
        new QVBoxLayout();

    graphInput =
        new QLineEdit();

    graphButton =
        new QPushButton("Check Cycle");

    graphLayout->addWidget(graphInput);
    graphLayout->addWidget(graphButton);

    graphBox->setLayout(graphLayout);

    //
    // OUTPUT
    //

    output =
        new QTextEdit();

    output->setReadOnly(true);

    //
    // MAIN
    //

    mainLayout->addWidget(authBox);
    mainLayout->addWidget(md5Box);
    mainLayout->addWidget(vigBox);
    mainLayout->addWidget(secantBox);
    mainLayout->addWidget(graphBox);
    mainLayout->addWidget(output);

    central->setLayout(mainLayout);

    setCentralWidget(central);

    //
    // SIGNALS
    //

    connect(registerButton,
            &QPushButton::clicked,
            this,
            &MainWindow::registerUser);

    connect(loginButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loginUser);

    connect(md5Button,
            &QPushButton::clicked,
            this,
            &MainWindow::md5Request);

    connect(vigenereButton,
            &QPushButton::clicked,
            this,
            &MainWindow::vigenereRequest);

    connect(secantButton,
            &QPushButton::clicked,
            this,
            &MainWindow::secantRequest);

    connect(graphButton,
            &QPushButton::clicked,
            this,
            &MainWindow::graphRequest);
}

void MainWindow::registerUser()
{
    QNetworkRequest request(
        QUrl("http://localhost:8080/register"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QString json =
        QString("{\"user\":\"%1\",\"password\":\"%2\"}")
            .arg(loginEdit->text())
            .arg(passwordEdit->text());

    network->post(request,
                  json.toUtf8());
}

void MainWindow::loginUser()
{
    QNetworkRequest request(
        QUrl("http://localhost:8080/login"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QString json =
        QString("{\"user\":\"%1\",\"password\":\"%2\"}")
            .arg(loginEdit->text())
            .arg(passwordEdit->text());

    network->post(request,
                  json.toUtf8());
}

void MainWindow::md5Request()
{
    QNetworkRequest request(
        QUrl("http://localhost:8080/md5"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QString json =
        QString("{\"text\":\"%1\"}")
            .arg(md5Text->text());

    network->post(request,
                  json.toUtf8());
}

void MainWindow::vigenereRequest()
{
    QNetworkRequest request(
        QUrl("http://localhost:8080/vigenere"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QString json =
        QString("{\"text\":\"%1\",\"key\":\"%2\"}")
            .arg(vigenereText->text())
            .arg(vigenereKey->text());

    network->post(request,
                  json.toUtf8());
}

void MainWindow::secantRequest()
{
    QNetworkRequest request(
        QUrl("http://localhost:8080/secant"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QString json =
        QString("{\"a\":\"%1\",\"b\":\"%2\"}")
            .arg(secantA->text())
            .arg(secantB->text());

    network->post(request,
                  json.toUtf8());
}

void MainWindow::graphRequest()
{
    QNetworkRequest request(
        QUrl("http://localhost:8080/graph-cycle"));

    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/json");

    QString json =
        QString("{\"graph\":\"%1\"}")
            .arg(graphInput->text());

    network->post(request,
                  json.toUtf8());
}

void MainWindow::handleReply(
    QNetworkReply *reply)
{
    QByteArray data =
        reply->readAll();

    output->append(
        QString::fromUtf8(data));

    reply->deleteLater();
}
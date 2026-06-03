#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QNetworkAccessManager *network;

    // AUTH

    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;

    QPushButton *registerButton;
    QPushButton *loginButton;

    // MD5

    QLineEdit *md5Text;
    QPushButton *md5Button;

    // VIGENERE

    QLineEdit *vigenereText;
    QLineEdit *vigenereKey;
    QPushButton *vigenereButton;

    // SECANT

    QLineEdit *secantA;
    QLineEdit *secantB;
    QPushButton *secantButton;

    // GRAPH

    QLineEdit *graphInput;
    QPushButton *graphButton;

    // OUTPUT

    QTextEdit *output;

private slots:

    void registerUser();
    void loginUser();

    void md5Request();
    void vigenereRequest();
    void secantRequest();
    void graphRequest();

    void handleReply(QNetworkReply *reply);
};

#endif
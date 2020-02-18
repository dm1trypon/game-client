#ifndef WIDGET_H
#define WIDGET_H

#include <manager.h>

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

class GUI : public QWidget
{
    Q_OBJECT

public:
    GUI(QWidget *parent = 0);
    ~GUI();

    void init();
private slots:
    void onConnect();
    void onError(const QString &error);
    void onTcpServerConnected();
    void onUdpServerConnected();
private:
    Manager m_manager;

    QGridLayout *m_mainLayout = nullptr;
    QLineEdit *m_leNickname = nullptr;
    QLineEdit *m_leHost = nullptr;
    QLineEdit *m_leTcpPort = nullptr;
    QLineEdit *m_leUdpPort = nullptr;
    QComboBox *m_cbResolution = nullptr;
    QLabel *m_lNickname = nullptr;
    QLabel *m_lResolution = nullptr;
    QLabel *m_lHost = nullptr;
    QLabel *m_lTCPPort = nullptr;
    QLabel *m_lUDPPort = nullptr;
    QPushButton *m_bConnect = nullptr;
    QGroupBox *m_gbTCP = nullptr;
    QGroupBox *m_gbUDP = nullptr;
};

#endif // WIDGET_H

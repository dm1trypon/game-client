#ifndef WIDGET_H
#define WIDGET_H

#include "manager.h"
#include "gamegui.h"

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
    GUI(QWidget *parent = nullptr);
    ~GUI();

    void init();
private slots:
    void onConnect();
    void onError(const QString &error);
    void onTcpServerConnected();
    void onUdpServerConnected();

private:
    QStringList m_gameResolution;

    Manager m_manager;
    GameGUI *m_gameGUI = nullptr;

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
    void showGameGUI();
};

#endif // WIDGET_H

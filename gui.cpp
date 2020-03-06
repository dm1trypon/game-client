#include "gui.h"

#include <QMessageBox>

GUI::GUI(QWidget *parent)
    : QWidget(parent)
{
    init();
    connect(m_bConnect, &QPushButton::clicked, this, &GUI::onConnect);
    connect(&m_manager, &Manager::error, this, &GUI::onError);
    connect(&m_manager, &Manager::connectedTcp, this, &GUI::onTcpServerConnected);
    connect(&m_manager, &Manager::connectedUdp, this, &GUI::onUdpServerConnected);
}

GUI::~GUI(){}

void GUI::onError(const QString &error) {
    QMessageBox::warning(this, "Warning!", error);
    m_bConnect->setEnabled(true);
}

void GUI::onTcpServerConnected() {
    const QString &resolution = m_cbResolution->currentText();
    m_gameResolution = resolution.split("x");
    m_manager.toVerifyTcp(m_leNickname->text(), m_gameResolution);
}

void GUI::onUdpServerConnected() {
    m_manager.toVerifyUdp(m_leNickname->text());
    showGameGUI();
}

void GUI::showGameGUI() {
    m_gameGUI = new GameGUI;
    connect(&m_manager, &Manager::nextFrame, m_gameGUI, &GameGUI::nextFrame);
    connect(m_gameGUI, &GameGUI::keyboard, &m_manager, &Manager::toKeyboard);
    connect(m_gameGUI, &GameGUI::mouseClick, &m_manager, &Manager::onMouseClick);
    connect(m_gameGUI, &GameGUI::mouseMove, &m_manager, &Manager::onMouseMove);
    m_gameGUI->resize(m_gameResolution.first().toInt(), m_gameResolution.last().toInt());
    m_gameGUI->show();
}

void GUI::onConnect() {
    const QString &host = m_leHost->text();
    const QString &udpPort = m_leUdpPort->text();
    const QString &tcpPort = m_leTcpPort->text();

    if (host.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Host is empty!");
        return;
    }

    if (udpPort.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "UDP port is empty!");
        return;
    }

    if (tcpPort.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "TCP port is empty!");
        return;
    }

    m_manager.onConnect(host, static_cast<quint16>(udpPort.toInt()), static_cast<quint16>(tcpPort.toInt()));

    m_bConnect->setDisabled(true);
}

void GUI::init() {
    m_mainLayout = new QGridLayout;

    m_leNickname = new QLineEdit();
    m_leNickname->setMaxLength(10);

    m_leHost = new QLineEdit();

    m_leTcpPort = new QLineEdit();
    m_leTcpPort->setMaxLength(5);
    m_leTcpPort->setValidator(new QIntValidator(10000, 60000, this));

    m_leUdpPort = new QLineEdit();
    m_leUdpPort->setMaxLength(5);
    m_leUdpPort->setValidator(new QIntValidator(10000, 60000, this));

    m_cbResolution = new QComboBox();

    const QStringList resolutions = {
        "800x600",
        "1024x600",
        "1024x768",
        "1280x720",
        "1360x768",
        "1600x900",
        "1920x1080"
    };

    m_cbResolution->addItems(resolutions);

    m_lNickname = new QLabel("Nickname:");
    m_lResolution = new QLabel("Resolution:");
    m_lHost = new QLabel("Host:");
    m_lTCPPort = new QLabel("TCP Port:");
    m_lUDPPort = new QLabel("UDP Port:");
    m_bConnect = new QPushButton("Connect");

    m_mainLayout->addWidget(m_lNickname, 1, 1, 1, 3);
    m_mainLayout->addWidget(m_leNickname, 2, 1, 1, 3);
    m_mainLayout->addWidget(m_lHost, 3, 1, 1, 1);
    m_mainLayout->addWidget(m_leHost, 4, 1, 1, 1);
    m_mainLayout->addWidget(m_lTCPPort, 3, 2, 1, 1);
    m_mainLayout->addWidget(m_leTcpPort, 4, 2, 1, 1);
    m_mainLayout->addWidget(m_lUDPPort, 3, 3, 1, 1);
    m_mainLayout->addWidget(m_leUdpPort, 4, 3, 1, 1);
    m_mainLayout->addWidget(m_lResolution, 5, 1, 1, 3);
    m_mainLayout->addWidget(m_cbResolution, 6, 1, 1, 3);
    m_mainLayout->addWidget(m_bConnect, 7, 1, 1, 3);

    setLayout(m_mainLayout);
}

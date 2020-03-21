#ifndef LesFenetres_QT
#define LesFenetres_QT

#include "client.hpp"
#include "mainWindow.hpp"
#include "maccroWindow.hpp"
#include "windowQT.hpp"

#include "../UI/src/Menu_LoginQt_ui.hpp"
#include "../UI/src/Menu_EnterQt_ui.hpp"
#include "../UI/src/Menu_RegisterQt_ui.hpp"
#include "../UI/src/MenuQt_ui.hpp"
#include "../UI/src/Salon_HoteQt_ui.hpp"
#include "../UI/src/Salon_InviteeQt_ui.hpp"
#include "../UI/src/DeconexionQt_ui.hpp"
#include "../UI/src/DetailsQt_ui.hpp"
#include "../UI/src/Modifier_Equipe_LombricQt_ui.hpp"
#include "../UI/src/Parametre_PartieQt_ui.hpp"
#include "../UI/src/AmisQt_ui.hpp"


#include <QPushButton>
#include <QLabel>
#include <string>
#include <vector>

class MainWindow;

class MenuQT: public WindowQT{
    Q_OBJECT
private:
    Ui::MenuWidget *page;
public:
    MenuQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~MenuQT();
};

class Menu_EnterQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Menu_EnterWidget *page;
public:
    Menu_EnterQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Menu_EnterQT();
};

class AmisQT: public WindowQT{
    Q_OBJECT
private:
    Ui::AmisWidget *page;
    stringTable friendsList;
    std::string chooseFriend;
    std::vector<chat_r> chooseConvo;
public:
    AmisQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~AmisQT();
private slots:
    void changeFriend(QString);
    void update();
};

class DetailsQT: public WindowQT{
    Q_OBJECT
private:
    Ui::DetailsWidget *page;
public:
    DetailsQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~DetailsQT();
};

class DeconexionQT: public WindowQT{
    Q_OBJECT
private:
    Ui::DeconexionWidget *page;
public:
    DeconexionQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~DeconexionQT();
};

class Menu_LoginQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Menu_LoginWidget *page;
public:
    Menu_LoginQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Menu_LoginQT();
private slots:
    void connection();
};

class Menu_RegisterQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Menu_RegisterWidget *page;
public:
    Menu_RegisterQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Menu_RegisterQT();
};

class Parametre_PartieQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Parametre_PartieWidget *page;
public:
    Parametre_PartieQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Parametre_PartieQT();
};

class Salon_HoteQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Salon_HoteWidget *page;
public:
    Salon_HoteQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Salon_HoteQT();
};

class Salon_InviteeQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Salon_InviteeWidget *page;
public:
    Salon_InviteeQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Salon_InviteeQT();
};

class Modifier_EquipeQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Modifier_EquipeWidget *page;
public:
    Modifier_EquipeQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Modifier_EquipeQT();
};

#endif

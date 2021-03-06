#ifndef LesFenetres_QT
#define LesFenetres_QT

#include "client.hpp"
#include "mainWindow.hpp"
#include "maccroWindow.hpp"
#include "windowQT.hpp"

#include "../UI/src/Menu_LoginQt_ui.hpp"
#include "../UI/src/Menu_EnterQt_ui.hpp"
#include "../UI/src/MenuQt_ui.hpp"
#include "../UI/src/Salon_HoteQt_ui.hpp"
#include "../UI/src/Salon_InviteeQt_ui.hpp"
#include "../UI/src/DeconexionQt_ui.hpp"
#include "../UI/src/DetailsQt_ui.hpp"
#include "../UI/src/Modifier_Equipe_LombricQt_ui.hpp"
#include "../UI/src/Parametre_PartieQt_ui.hpp"
#include "../UI/src/AmisQt_ui.hpp"
#include "../UI/src/End_GameQt_ui.hpp"
#include "../UI/src/InvitationQt_ui.hpp"
#include "../UI/src/ReplayQt_ui.hpp"
#include "../UI/src/Wait_ScreenQt_ui.hpp"


#include <QPushButton>
#include <QLabel>
#include <string>
#include <vector>

class MainWindow;

class MenuQT: public WindowQT{
    Q_OBJECT
private:
    Ui::MenuWidget *page;
    std::vector<Equipe*> teamsVector;
    bool invitationNotifIsDraw;
    bool chatNotifIsDraw;
public:
    MenuQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~MenuQT();
private slots:
    void deconnection();
    void update_invit();
    void handle_notif_invit();
    void handle_notif_amis();
};

class EndGame: public WindowQT{
    Q_OBJECT
private:
    Ui::End_GameWidget *page;
    std::vector<Equipe*> teams;
public:
    EndGame(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~EndGame();
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
    std::vector<std::string> friendsList;
    std::string chooseFriend;
    std::vector<chat_r> chooseConvo;
    QListWidgetItem * friend_clicked;
public:
    AmisQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~AmisQT();
private slots:
    void changeFriend(QString);
    void addFriend();
    void sendMessage();
    void update();
    void handle_friend(QListWidgetItem * item);
    void delete_friend();
};

class DetailsQT: public WindowQT{
    Q_OBJECT
private:
    Ui::DetailsWidget *page;
    std::string chooseHistory;
    playerRank rank;
    std::vector<gameHistory> historique;
public:
    DetailsQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~DetailsQT();
private slots:
    void askHistory();
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
    bool isConnection;
public:
    Menu_LoginQT(int id, MainWindow *parent, Client* cli, bool isCon);//isConnection a false si c'est une inscription
    void initWindow() override;
    bool JustSpace(string str);
    virtual ~Menu_LoginQT();
private slots:
    void connection();
};

class Parametre_PartieQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Parametre_PartieWidget *page;
public:
    Parametre_PartieQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~Parametre_PartieQT();
private slots:
    void setPara();
    void changeMax(int);
};

class SalonQT: public WindowQT{
    Q_OBJECT
private:
    int id_screen;
    Ui::Salon_HoteWidget *page;
    std::vector<std::string> friendsList;
    infoRoom_s infoPartie;
public:
    SalonQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~SalonQT();
private slots:
    void sendGameInvit();
    void change_equipe();
    void play();
    void update_para();
    void leave_room();
};

class Modifier_EquipeQT: public WindowQT{
    Q_OBJECT
private:
    Ui::Modifier_EquipeWidget *page;
    std::vector<std::string> OldLombricName;
public:
    Modifier_EquipeQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    bool JustSpace_inLomb(string str);
    virtual ~Modifier_EquipeQT();
private slots:
    void ChangeNameLombric();
};

class InvitationQT: public WindowQT{
    Q_OBJECT
private:
    Ui::InvitationWidget *page;
    int row_invit;
    QListWidgetItem* item_clicked;
    //std::string name_invit;
public:
    InvitationQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~InvitationQT();
private slots:
    void handle_invit(QListWidgetItem * item);
    void delete_invit();
    void accept_invit();
    //void update_invit();
};

class ReplayQT: public WindowQT{
    Q_OBJECT
private:
    Ui::ReplayWidget *page;
    std::vector<std::string> vectorReplays;
    QListWidgetItem* item_clicked;
    int row_replay;
public:
    ReplayQT(int id, MainWindow *parent, Client* cli);
    void initWindow() override;
    virtual ~ReplayQT();
private slots:
    void handle_double_clicked(QListWidgetItem * item);
    void go_replay();
};

class Wait_ScreenQT: public WindowQT{
  Q_OBJECT
private:
  Ui::Wait_ScreenWidget *page;
public:
  Wait_ScreenQT(int id, MainWindow *parent, Client* cli);
  void initWindow() override;
  virtual ~Wait_ScreenQT();
private slots:
  void CheckMatchIsFind();
};


#endif

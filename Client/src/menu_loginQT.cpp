#include "../includes/LesFenetres_QT.hpp"

Menu_LoginQT::Menu_LoginQT(int id, MainWindow *parent, Client* cli, bool isCon):
WindowQT(id, parent, client),
isConnection(isCon){
    

    page = new Ui::Menu_LoginWidget;
    page->setupUi(this);

    signalMapper->setMapping(page->Return_Menu_LoginToolButton, INIT_SCREEN);
    connect(page->Return_Menu_LoginToolButton, SIGNAL(clicked()), signalMapper, SLOT(map()));

    if(isCon){
        page->Connect_UserToolButton->setText("Se connecter");
    }else{
        page->Connect_UserToolButton->setText("S'inscrire");
    }

    connect(page->Connect_UserToolButton, SIGNAL(clicked()), this, SLOT(connection()));
}


void Menu_LoginQT::initWindow(){
    //parent->setStyleSheet("background-image: url(:/wallpaper/UI/Resources/cropped-1920-1080-521477.jpg);");

    page->PasswordlineEdit->clear();
    page->PseudolineEdit->clear();
}

void Menu_LoginQT::connection(){
    std::string username = page->PseudolineEdit->text().toStdString();
    std::string password = page->PasswordlineEdit->text().toStdString();
    bool username_isSpace = false;
    bool password_isSpace = false;

    //on vérifie si les sting ne contiennent que des espaces vides
    username_isSpace = JustSpace(username);
    password_isSpace = JustSpace(password);


    bool con = client->connection(username, password, isConnection);
    if(con && !username_isSpace && !password_isSpace){
        parent->setUsername(username);
        parent->setPage(MAIN_MENU_SCREEN);
    }else{
        parent->setPage(WARNING_WRONG_DATA_IN_LOGIN_SCREEN);
    }
}

bool Menu_LoginQT::JustSpace(std::string text){

    bool isSpace = false;
    text.erase(remove(text.begin(), text.end(), ' '), text.end());
    if (!text.size())
    {
      isSpace = true;
    }
    return isSpace;
}

Menu_LoginQT::~Menu_LoginQT(){
    delete page;
}

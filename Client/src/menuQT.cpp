#include "../includes/LesFenetres_QT.hpp"

#include "../UI/src/MenuQt_ui.hpp"

MenuQT::MenuQT(int id, MainWindow *parent, Client* cli):
WindowQT(id, parent, client){
  Ui::MenuWidget page;
  /*page.setupUi(this);

  signalMapper->setMapping(page.Create_party_toolButton, 1);
  connect(page.Create_party_toolButton, SIGNAL(clicked()), signalMapper, SLOT(map()));*/
}
#include "../includes/partie_serv.hpp"

#define min(_a,_b) _a<_b?_a:_b
#define DELTA_T 50 //granularité du temps en millisecondes

Partie::Partie():gameInfo(nullptr){
    srand (time(NULL));
}

Partie::~Partie(){
    if(gameInfo)deleteinfoPartie_s(gameInfo);
}

void Partie::setParam(Map* m, std::vector<Sprite*> listLomb){
    gameInfo = new infoPartie_s{m, listLomb, std::vector<Arme*>(),nullptr, 0, std::vector<Equipe*>()};
    gameInfo->armesVector.push_back(new LanceMissile("Lance 'o'", 1, 25, -25, 4));
    gameInfo->armesVector.push_back(new BatteBaseball("Batte", 2, 20, -25));
    gameInfo->armesVector.push_back(new PassTour("PassTour", 0, 0, 0));
}

std::vector<std::string> Partie::useWeapon(std::string tir){
    std::vector<std::string> res;

    Tir weaponUsed;
    weaponUsed.ParseFromString(tir);

    blockDeleted.Clear();

    Arme* currentWeapon = gameInfo->armesVector[weaponUsed.id_arme()];
    currentWeapon->setForce(weaponUsed.force());
    currentWeapon->setAngle(weaponUsed.angle());
    currentWeapon->shoot(gameInfo, 0);

    update();

    res.push_back(blockDeleted.SerializeAsString());

    Degats_lombric d;
    for(auto sp = gameInfo->spriteVector.begin();sp!=gameInfo->spriteVector.end();++sp ){
        if((*sp)->getId()){
            Lombric_c* lomb = dynamic_cast<Lombric_c*>(*sp);
            Lombric* upLomb = d.add_lomb_upt();
            upLomb->set_id_lomb(lomb->getId());
            int pos[2];
            lomb->getPos(pos);
            upLomb->set_pos_x(pos[0]);
            upLomb->set_pos_y(pos[1]);
            upLomb->set_vie(lomb->getLife());
        }
    }

    res.push_back(d.SerializeAsString());
    return res;//pour le moment du moins
}

bool Partie::updateSprites(int t){
    int newPos[2];
    int oldPos[2];
    int life;

    bool isMovement = false;//mis à vrai s'il y a un seul sprite en mouvement

    auto s = gameInfo->spriteVector.begin();
    while(s != gameInfo->spriteVector.end()){
        int id = (*s)->getId();
        if(id)life = dynamic_cast<Lombric_c*>(*s)->getLife();
        (*s)->getPos(oldPos);
        bool alive = (*s)->update(gameInfo, t);
        (*s)->getPos(newPos);

        //si la vie du lombric a changé -> fin de tour
        if(id && dynamic_cast<Lombric_c*>(*s)->getLife() != life)endTour = true;

        isMovement |= (*s)->isInMovement();//un suel lombric en mouvement -> isMovement = true
        if(!alive){//le sprite doit mourir, on le supprime
            isMovement = true;

            std::vector<int> deletedBlock = (*s)->deathMove(gameInfo,t);
            addDeletedBlock(deletedBlock);


            std::cout << "Sprite is dead, his id: " << id << std::endl;
            //if(!id){
              delete *s;
            //}
            *s = nullptr;
            gameInfo->spriteVector.erase(s);
        }
        else{
          ++s;
        }
    }
    return isMovement;
}

bool Partie::moveCurrentLombric(std::string s){
    Lomb_pos lp;
    lp.ParseFromString(s);
    Lombric_c* lomb = dynamic_cast<Lombric_c*>(findById(gameInfo->spriteVector, lp.id_lomb()));
    if(!lomb)return false;

    int oldPos[2];
    lomb->getPos(oldPos);
    int newPos[2];
    newPos[0] = lp.pos_x();
    newPos[1] = lp.pos_y();

    if(oldPos[0] != newPos[0]-1 && oldPos[0] != newPos[0]-1)return false;
    if(oldPos[1] != newPos[1] && oldPos[1]-1 != newPos[1])return false;

    lomb->setPos(newPos);

    update();

    return true;
}

void Partie::addDeletedBlock(std::vector<int> v){
    for(auto i=v.begin();i!=v.end();++i){
        blockDeleted.add_coord(*i);
    }
}

bool Partie::isLombAlive(int id){
    Lombric_c* lomb =  dynamic_cast<Lombric_c*>(findById(gameInfo->spriteVector, id));
    if(!lomb)return false;
    return lomb->getLife()>0;
}


void Partie::setCurrentLomb(int id){
    endTour = false;
    gameInfo->currentWorms = dynamic_cast<Lombric_c*>(findById(gameInfo->spriteVector, id));
}

bool Partie::isTourFinish(){return endTour;}

void Partie::setAnimationTime(int t){
    animationTime = t;
}

void Partie::waitAnimationTime(){
    usleep(animationTime*1000);//en millisecondes
    animationTime = 0;
}

void Partie::update(){
    int t = 0;
    bool run = true;
    while (run){
        //run tant que il y'a des psites en mouvement
        run = updateSprites(t);//update la positions des sprites à cahque itérations
        t += 25;
    }
    setAnimationTime(t);
}

void Partie::addHealthBox(HealthBox* box){
    gameInfo->spriteVector.push_back(box);
}

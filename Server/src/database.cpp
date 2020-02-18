#include <iostream>
#include "../includes/database.hpp"

DataBase::DataBase(std::string db_path):m_is_open(false){
    m_rc = sqlite3_open(db_path.c_str(), &m_db);
    if(m_rc){
        std::cout << "Can't open database: " << sqlite3_errmsg(m_db) << std::endl;
        m_is_open = false;
    }
    else{
        std::cout << "Database opened" << std::endl << std::endl;
        m_is_open = true;
    }
}

DataBase::~DataBase(){
    sqlite3_close(m_db);
}

// Getters
bool DataBase::is_opened() const{
    return m_is_open;
}

bool DataBase::catch_error(){
    if(m_rc != SQLITE_OK){
       std::cout << "SQL error: " << m_zErrMsg << std::endl;
       sqlite3_free(m_zErrMsg);
       return false;
    }
    std::cout << "Request successfull" << std::endl;
    return true;
}

int DataBase::callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", static_cast<const char*>(data));

   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }

   printf("\n");
   return 0;
}

bool DataBase::get_las() const{
    return m_rc;
}

std::string DataBase::get_last_out() const{
    return "";
}


void DataBase::get_user(std::string username){
    std::cout << username << std::endl;

    m_sql_request = "SELECT * from users";

   /* Execute SQL statement */
   m_rc = sqlite3_exec(m_db, m_sql_request.c_str(), callback, nullptr, &m_zErrMsg);
}

void DataBase::get_passwd(std::string username){
    std::cout << "Getting password of " << username << std::endl;
}

bool DataBase::verify_user(std::string username, std::string password) const{
    std::cout << username << std::endl;
    std::cout << password << std::endl;

    return false;
}

void DataBase::register_user(std::string username, std::string password){
    std::cout << username << std::endl;
    std::cout << password << std::endl;
}

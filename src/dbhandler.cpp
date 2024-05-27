#include "dbhandler.h"

#include <QMessageBox>

DBHandler* DBHandler::s_instance = nullptr;

DBHandler::DBHandler() {
    connect2db();
}

DBHandler::~DBHandler()
{

}

DBHandler *DBHandler::instance()
{
    if(!s_instance){
        s_instance = new DBHandler;
    }

    return s_instance;
}

bool DBHandler::login(QString name, QString pwd, int &id)
{
    bool r = false;

    if(m_db.isOpen()){
        QString sql = QString("select id from users where name='%1' and pwd='%2';")
                          .arg(name,pwd);
        if(m_query.exec(sql)){
            while(m_query.next()){
                id = m_query.value("id").toInt();
                r=true;
            }
        }else{
            qDebug()<<"[ERROR] [login] Cannot exec sql."<<m_query.lastError();
        }
    }else{
        qDebug()<<"[ERROR] [login] Please connect to db first."<<m_db.lastError();
    }

    return r;
}

bool DBHandler::regis(QString name, QString pwd)
{
    bool r=false;
    int id = 0;
    if(!login(name,pwd,id)){
        QString sql = QString("insert into users (id,name,pwd) VALUES (%1,'%2','%3');")
                          .arg(id+1).arg(name,pwd);
        if(m_query.exec(sql)){
            qDebug()<<"[Info] Add one new user.";
            r=true;
        }
    }else{
        qDebug()<<"[ERROR] User exists.";
    }

    return r;
}

void DBHandler::connect2db()
{
    if(!m_db.isValid()){
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("D:\\itpmps.sqlite3");
        if(!m_db.open()){
            qDebug()<<"[ERROR] Cannot connect to db."<<m_db.lastError();
        }else{
            qDebug()<<"[Info] Connect to db.";
        }

        m_query = QSqlQuery(m_db);
    }
}
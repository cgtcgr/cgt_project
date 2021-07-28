#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
class sqlManager :public QObject
{
    Q_OBJECT
public:
    static sqlManager*GetInstance();
    sqlManager()
    {
        m_database = QSqlDatabase::addDatabase("QSQLITE");
        m_database.setDatabaseName("MyDataBase.db");
        openDatabase();
    }

    ~sqlManager()
    {
        m_database.close();
    }

    bool openDatabase()
    {

        bool isOpen = false;
        isOpen = m_database.open();
        qDebug()<<"DataBase is Open ?"<<isOpen;
        qDebug()<<"Open file: "<<m_database.databaseName()<<" sucessfully!";
        return isOpen;
    }

    bool createPatrolData()//id ,tasktime , taskPointName , T ,  T, startTime（hour ,min）,result;
    {
        //创建表格
        QSqlQuery query(m_database);
        if(!query.exec("create table taskResult(id INTEGER PRIMARY KEY autoincrement, time nvarchar(50), taskPointName text, startTime int, upT int,downT int,result bool)"))
        {
            //qDebug() << "Error: Fail to create table."<< sql_query.lastError();
        }
        if(!query.exec("create table taskInfo(id INTEGER PRIMARY KEY autoincrement, taskPointName text, devpos int, cameraLeftPos int,cameraRightPos int)"))
        {
            //qDebug() << "Error: Fail to create table."<< sql_query.lastError();
        }
        return true;
    }

    /*
        插入函数.
        构造SQL插入语句.
        taskInfo
    */
    bool insertData(const QString& table, QStringList& names, QStringList& values)
    {
        if (names.size() != values.size())
        {
            return false;
        }
        //openDatabase();  //打开数据库;
        QSqlQuery query(m_database);//QSqlDatabase::database()
        QString sql = QString("insert into %1 (").arg(table);
        int i;
        for (i=0; i < names.size(); i++)
        {
            sql = sql + names.value(i);
            if (i != names.size() - 1)
            {
                sql += QString(",");
            }
            else
            {
                sql += QString(") ");
            }
        }
        sql +=  QString("values (");
        for (i = 0; i < values.size(); i++)
        {
            sql +=  QString("'%1'").arg(values.value(i));
            if (i != values.size()-1)
            {
                sql += QString(",");
            }
        }
        sql += QString(")");
        //INSERT INTO Teachers(Name) Values('张三');
        //sql= "INSERT INTO new_test(Port),Values('2011');";
        qDebug()<<sql;
        bool ok=query.exec(sql);
        qDebug()<<"Insert data:"<<ok;
        //m_database.close();
        if (ok)
        {
            return true;
        }
        else
        {
            //m_database.close();
            return false;
        }
    }
    /*
        修改函数.
        构造SQL修改语句.
    */
    bool updateData(const QString &table, QStringList &names, QStringList &values, QString &expression)
    {
        if (names.size() != values.size())
        {
            return false;
        }
        //UPDATE 表名称 SET 列名称 = 新值 WHERE 列名称 = 某值
        QSqlQuery query(m_database);
        QString sql = QString("update ")+table+QString(" set ");
        for (int i = 0; i < names.size(); i++)
        {
            sql +=  names.value(i);
            sql +=  QString(" = '");
            sql +=  values.value(i);
            sql +=  QString("'");
            if (i != names.size()-1)
            {
                sql += QString(" ,");
            }
        }
        sql +=  QString(" where %1").arg(expression);
        if (query.exec(sql))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /*
        删除函数.
        构造SQL删除语句.
    */
    bool del(const QString &table, QString &expression)
    {
        //DELETE FROM 表名称 WHERE 列名称 = 值
        QSqlQuery query(m_database);
        QString sql = QString("delete from %1 ").arg(table) + QString(" where %1").arg(expression);
        if (query.exec(sql))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void getValues(const QString &table ,int index, QStringList &values)
    {
        QSqlQuery query(m_database);
        QString sql = QString("select * from %1 ;").arg(table);
        qDebug()<<sql;
        bool ok=query.exec(sql);
        qDebug()<<ok;

        while (query.next())
        {
            for(int i = 0; i< index ;i++)
            {
                values << query.value(i).toString();
            }
        }
        qDebug()<<values;
        //m_database.close();
    }

    void getValuesWhereTime(const QString &table, QStringList &values, QString &start, QString &end, QString &name)
    {
        QSqlQuery query(m_database);
        //QString sql = QString("delete from taskInfo ");
        QString sql = QString("select * from taskInfo ")+ QString(" where time BETWEEN '%1' and '%2' AND taskPointName= '%3'").arg(start).arg(end).arg(name);
        qDebug()<<sql;
        bool ok=query.exec(sql);
        qDebug()<<ok;

        while (query.next())
        {
            for(int i = 0; i< 7 ;i++)
            {
                values << query.value(i).toString();
            }
        }
        qDebug()<<values;
        //m_database.close();
    }
private:
    QSqlDatabase m_database;

};

#endif // SQLMANAGER_H

/*//建立并打开数据库
    QSqlDatabase database;
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("MyDataBase.db");
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database." ;
    }

    //创建表格
    QSqlQuery sql_query;
    if(!sql_query.exec("create table student(id int primary key, name text, age int)"))
    {
        qDebug() << "Error: Fail to create table."<< sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }

    //插入数据
    if(!sql_query.exec("INSERT INTO student VALUES(1, \"Wang\", 23)"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Wang!";
    }
    if(!sql_query.exec("INSERT INTO student VALUES(2, \"Li\", 23)"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "inserted Li!";
    }

    //修改数据
    sql_query.exec("update student set name = \"QT\" where id = 1");
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "updated!";
    }

    //查询数据
    sql_query.exec("select * from student");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            int id = sql_query.value(0).toInt();
            QString name = sql_query.value(1).toString();
            int age = sql_query.value(2).toInt();
            qDebug()<<QString("id:%1    name:%2    age:%3").arg(id).arg(name).arg(age);
        }
    }

    //删除数据
    sql_query.exec("delete from student where id = 1");
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"deleted!";
    }

    //删除表格
    sql_query.exec("drop table student");
    if(sql_query.exec())
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "table cleared";
    }

    //关闭数据库
    database.close();
    */

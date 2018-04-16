#ifndef MAPLISTMODELTEST_H
#define MAPLISTMODELTEST_H

#include <QAbstractListModel>

class Item{

public:

    explicit Item(bool isEmg ,bool skipGrade ,  QString name , QString number, int numEmg, int numDevices);

    bool m_IsEmg;
    bool m_SkipGrade;           //true：当区域只有一栋楼，直接跳过建筑信息表，到楼层表

    QString m_Name;
    QString m_Number;
    int m_NumEmg;
    int m_NumDevices;
};

class MapListModelTest : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapListModelTest(QObject *parent = 0);

    bool addItem(const Item &item);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    enum RoleNames{RoleName_IsEmg = Qt::UserRole+1 ,
                   RoleName_SkipGrade ,
                   RoleName_Name ,
                   RoleName_Number ,
                   RoleName_NumEmg ,
                   RoleName_NumDevices};

signals:

public slots:

protected:
    QHash<int , QByteArray>roleNames() const;


private:
    QList<Item>m_ItemList;

};

#endif // MAPLISTMODELTEST_H

#include "maplistmodeltest.h"

Item::Item(bool isEmg, bool skipGrade,QString name, QString number , int numEmg , int numDevices)
{
    m_IsEmg = isEmg;
    m_SkipGrade = skipGrade;
    m_Name = name;
    m_Number = number;
    m_NumEmg = numEmg;
    m_NumDevices = numDevices;
}

MapListModelTest::MapListModelTest(QObject *parent) :
    QAbstractListModel(parent)
{
}
bool MapListModelTest::addItem(const Item &item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_ItemList << item;
    endInsertRows();
    return true;
}
int MapListModelTest::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_ItemList.count();
}
QVariant MapListModelTest::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_ItemList.count())
        return QVariant();

    const Item &item = m_ItemList[index.row()];
    if (role == RoleName_IsEmg)
        return item.m_IsEmg;
    else if(role == RoleName_SkipGrade)
        return item.m_SkipGrade;
    else if (role == RoleName_Name)
        return item.m_Name;
    else if (role == RoleName_Number)
        return item.m_Number;
    else if(role == RoleName_NumEmg)
        return item.m_NumEmg;
    else if(role == RoleName_NumDevices)
        return item.m_NumDevices;

    return QVariant();
}
bool MapListModelTest::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || index.row() < 0 || index.row() >= m_ItemList.size())
        return false;

    Item &item = m_ItemList[index.row()];

    bool ok;
    switch (role) {
    case RoleName_IsEmg:
        item.m_IsEmg = value.toBool();
        break;
    case RoleName_SkipGrade:
        item.m_SkipGrade = value.toBool();
        break;
    case RoleName_Name:
        item.m_Name = value.toString();
        break;
    case RoleName_Number:
        item.m_Number = value.toString();
        break;
    case RoleName_NumEmg:
        item.m_NumEmg = value.toInt(&ok);
        break;
    case RoleName_NumDevices:
        item.m_NumDevices = value.toInt(&ok);
        break;
    }

    emit dataChanged(index ,index);
    return true;

}

QHash<int , QByteArray>MapListModelTest::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RoleName_IsEmg] = "isemg";
    roles[RoleName_Name] = "buildname";
    roles[RoleName_Number] = "buildnumber";
    roles[RoleName_NumEmg] = "numemg";
    roles[RoleName_NumDevices] = "numdevices";
    return roles;
}

#ifndef _C_CHATSERVER_H_INCLUDE_
#define _C_CHATSERVER_H_INCLUDE_

#include <QObject>
#include <QHash>
#include <QVector>

#include "QsLog.h"
#include "sqlitedataprovider.h"
#include "common.h"
#include "networkframemanager.h"
#include "cwebsocketserver.h"

#define IDD_CHATSERVER_OPER_SUCCESSED        1000         // 操作成功
#define IDD_CHATSERVER_OPER_FAILED           1001         // 操作失败
#define IDD_CHATSERVER_OPER_EXIST            1002         // 已经存在
#define IDD_CHATSERVER_OPER_NOEXIST          1003         // 不存在
#define IDD_CHATSERVER_OPER_FRIEND_NOEXIST   1004         // 好友不存在
#define IDD_CHATSERVER_OPER_OFFLINE          1005         // 离线
#define IDD_CHATSERVER_OPER_GROUP_NOEXIST    1006         // 群不存在
#define IDD_CHATSERVER_OPER_GROUP_USEREXIST  1007         // 用户已经存在群中
#define IDD_CHATSERVER_OPER_GROUP_USERNOEXIST 1008        // 用户不在群中

#define IDD_CHATSERVER_USER                  2000         // 用户操作
#define IDD_CHATSERVER_USER_REGISTER         2001         // 用户注册
#define IDD_CHATSERVER_USER_UPDATEINFO       2002         // 修改用户信息
#define IDD_CHATSERVER_USER_CANCEL           2003         // 注销用户
#define IDD_CHATSERVER_USER_ADD_FRIEND       2004         // 加好友
#define IDD_CHATSERVER_USER_DEL_FRIEND       2005         // 删除好友
#define IDD_CHATSERVER_USER_LOGIN            2006         // 用户登录
#define IDD_CHATSERVER_USER_GETONLINEUSERS   2007         // 得到当前所有在线用户信息
#define IDD_CHATSERVER_USER_GETFRIENDS       2008         // 得到好友
#define IDD_CHATSERVER_USER_CHAT             2009         // 用户聊天
#define IDD_CHATSERVER_USER_GETCHATRECORD    2010         // 得到聊天记录

#define IDD_CHATSERVER_GROUP                 3000         // 群操作
#define IDD_CHATSERVER_GROUP_CREATE          3001         // 建立新群
#define IDD_CHATSERVER_GROUP_DELETE          3002         // 删除群
#define IDD_CHATSERVER_GROUP_JOIN            3003         // 加入群
#define IDD_CHATSERVER_GROUP_LEAVE           3004         // 离开群
#define IDD_CHATSERVER_GROUP_CHAT            3005         // 群聊天
#define IDD_CHATSERVER_GROUP_GETCHATRECORD   3006         // 得到群聊天记录

class CWebSocketServer;

struct tagGroupInfo
{
    tagGroupInfo()
        : id(0),creatorId(0) {}
    tagGroupInfo(int pid,QString pname,int pcreatorid,QString pintroduce)
        : id(pid),name(pname),creatorId(pcreatorid),introduce(pintroduce) {}

    /// 查找指定ID的成员是否存在
    inline bool isExistMember(int userid)
    {
        QVector<int>::iterator iter = std::find(members.begin(),
                                            members.end(),
                                            userid);
        if(iter == members.end())
            return false;

        return true;
    }

    /// 删除指定ID的成员
    inline bool deleteMember(int userid)
    {
        QVector<int>::iterator iter = std::find(members.begin(),
                                            members.end(),
                                            userid);
        if(iter == members.end())
            return false;

        members.erase(iter);

        return true;
    }

    /// 得到群成员转字符串
    inline QString getMemberString(void)
    {
        QString returnStr;

        if(!members.isEmpty())
        {
            for(int i=0;i<members.size();i++)
            {
                returnStr += QString::asprintf("%d,",members[i]);
            }

            returnStr = returnStr.mid(0,returnStr.length()-1);
        }

        return returnStr;
    }

    int id;                // 群ID
    QString name;          // 群名称
    int creatorId;         // 建立者ID
    QString introduce;     // 群描述
    QVector<int> members;  // 群成员
};

struct tagUserInfo
{
    tagUserInfo()
        : conn(NULL),id(0) {}
    tagUserInfo(int iid,QString n,QString p)
        : id(iid),name(n),password(p),conn(NULL) {}

    /// 查找指定ID的朋友是否存在
    inline bool isExistFriend(int userid)
    {
        QVector<int>::iterator iter = std::find(friends.begin(),
                                            friends.end(),
                                            userid);
        if(iter == friends.end())
            return false;

        return true;
    }

    /// 删除指定ID的朋友
    inline bool deleteFriend(int userid)
    {
        QVector<int>::iterator iter = std::find(friends.begin(),
                                            friends.end(),
                                            userid);
        if(iter == friends.end())
            return false;

        friends.erase(iter);

        return true;
    }

    int id;                   // 用户ID
    QString name;             // 用户名
    QString password;         // 用户密码
    QVector<int> friends;     // 用户的朋友
    QWebSocket *conn;         // 连接
};

class cchatserver : public QObject , public NetworkFrameManager
{
    Q_OBJECT

public:
    explicit cchatserver(QObject *parent = nullptr);
    ~cchatserver();

    /// 启动服务器
    bool startServer(int port);
    /// 关闭服务器
    void closeServer(bool isprintlog=true);
    /// 检测服务器是否启动
    bool isListening(void);

    /// 打开指定的数据库
    bool openDB(QString dbpath);
    /// 得到当前服务器信息
    QString getCurrentServerInfo(void);

signals:
    /// 日志消息
    void showlog(QString msg);

private:
    /// 打印用户日志
    void printLog(QsLogging::Level type,QString msg);
    /// 得到当前时间
    QString getCurrentDate(void);
    /// 检测指定ID的用户是否在线
    bool isUserOnline(int userId);
    /// 根据用户ID得到用户数据
    tagUserInfo* getUserInfo(int userId);
    /// 得到指定ID的群信息
    tagGroupInfo* getGroupInfo(int groupId);
    /// 解散指定ID的群
    bool deletegroup(int groupId);
    /// 得到所有的群
    void getAllGroup(void);
    /// 检测指定客户端是否在线
    bool inClientOnline(QWebSocket *conn);

protected:
    /// 处理网络字符串消息
    virtual void OnProcessNetText(QWebSocket *conn,QString mes);
    /// 处理一个新的连接到达
    virtual void OnProcessConnectedNetMes(QWebSocket *conn);
    /// 处理一个连接关闭
    virtual void OnProcessDisconnectedNetMes(QWebSocket *conn);

private:
    /// 处理用户注册消息
    void onProcessNetUserRegister(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户修改信息消息
    void onProcessNetUserUpdateInfo(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户注销消息
    void onProcessNetUserCancel(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户登录消息
    void onProcessNetUserLogin(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户得到当前所有在线用户信息
    void onProcessNetUserGetAllonlineUsers(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户加好友消息
    void onProcessNetUserAddFriends(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户删除好友消息
    void onProcessNetUserDeleteFriends(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户得到好友消息
    void onProcessNetUserGetFriends(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户聊天消息
    void onProcessNetUserChat(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户获取聊天记录消息
    void onProcessNetUserGetChatRecord(QWebSocket *conn,QJsonObject &msgObj);

    /// 处理建立新群消息
    void onProcessNetUserCreateGroup(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理解散群消息
    void onProcessNetUserDeleteGroup(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理加入群消息
    void onProcessNetUserJoinGroup(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理离开群消息
    void onProcessNetUserLeaveGroup(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户聊天消息
    void onProcessNetUserGroupChat(QWebSocket *conn,QJsonObject &msgObj);
    /// 处理用户获取聊天记录消息
    void onProcessNetUserGroupGetChatRecord(QWebSocket *conn,QJsonObject &msgObj);

private:
    CWebSocketServer                 m_MainWebSocketServer;                         /**< websocket服务器 */
    SqliteDataProvider               m_SqliteDataProvider;                          /**< 数据库操作 */
    QHash<QWebSocket*,tagUserInfo>   m_onlineUsers;                                 /**< 在线用户列表 */
    QHash<QString,tagGroupInfo>      m_allgrounps;                                  /**< 管理所有的群 */
};

#endif // CCHATSERVER_H

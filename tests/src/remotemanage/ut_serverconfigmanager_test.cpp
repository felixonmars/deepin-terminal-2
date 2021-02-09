#include "ut_serverconfigmanager_test.h"

#include "serverconfigmanager.h"
#include "service.h"
#include "mainwindow.h"
#include "serverconfigoptdlg.h"
#include "utils.h"

//Google GTest 相关头文件
#include <gtest/gtest.h>

//Qt单元测试相关头文件
#include <QTest>
#include <QtGui>

UT_ServerConfigManager_Test::UT_ServerConfigManager_Test()
{
}

void UT_ServerConfigManager_Test::SetUp()
{
}

void UT_ServerConfigManager_Test::TearDown()
{
}

int UT_ServerConfigManager_Test::getServerConfigCount()
{
    QList<ServerConfig *> serverConfigList;
    QMap<QString, QList<ServerConfig *>> severConfigs = ServerConfigManager::instance()->getServerConfigs();
    for (QMap<QString, QList<ServerConfig *>>::iterator iter = severConfigs.begin(); iter != severConfigs.end(); ++iter) {
        QList<ServerConfig *> value = iter.value();
        for (int i = 0; i < value.size(); ++i) {
            serverConfigList.append(value.at(i));
        }
    }

    qDebug() << serverConfigList.count();
    return serverConfigList.count();
}

#ifdef UT_SERVERCONFIGMANAGER_TEST
TEST_F(UT_ServerConfigManager_Test, ServerConfigManagerTest)
{
    ServerConfigManager *serverConfigManager = ServerConfigManager::instance();
    serverConfigManager->initServerConfig();

    int serverConfigCount = getServerConfigCount();
    qDebug() << serverConfigCount << endl;

    QString groupName = QString("group_01");

    qsrand(static_cast<uint>(time(nullptr)));
    ServerConfig *config = new ServerConfig();
    config->m_serverName = QString("new_server_%1").arg(Utils::getRandString());
    config->m_address = QString("192.168.10.%1").arg(qrand() % 255);
    config->m_userName = QString("zhangsan");
    config->m_password = QString("123");
    config->m_privateKey = QString("");
    config->m_port = QString("");
    config->m_group = groupName;
    config->m_path = QString("");
    config->m_command = QString("");
    config->m_encoding = QString("");
    config->m_backspaceKey = QString("");
    config->m_deleteKey = QString("");

    serverConfigManager->saveServerConfig(config);

    serverConfigManager->getServerCount(config->m_group);

    ServerConfig *currConfig = serverConfigManager->getServerConfig(config->m_serverName);

    qsrand(static_cast<uint>(time(nullptr)));
    ServerConfig *newConfig = new ServerConfig();
    newConfig->m_serverName = QString("new_server_%1").arg(Utils::getRandString());
    newConfig->m_address = QString("192.168.10.%1").arg(qrand() % 255);
    newConfig->m_userName = QString("uos");
    newConfig->m_password = QString("123456");
    newConfig->m_privateKey = QString("");
    newConfig->m_port = QString("");
    newConfig->m_group = groupName;
    newConfig->m_path = QString("");
    newConfig->m_command = QString("");
    newConfig->m_encoding = QString("");
    newConfig->m_backspaceKey = QString("");
    newConfig->m_deleteKey = QString("");
    serverConfigManager->modifyServerConfig(newConfig, currConfig);

    serverConfigManager->delServerConfig(newConfig);
}

/*******************************************************************************
 1. @函数:    initManager
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-25
 4. @说明:    初始化Manager
*******************************************************************************/
TEST_F(UT_ServerConfigManager_Test, initManager)
{
    // 将现在已有的instance删除
    delete ServerConfigManager::m_instance;
    ServerConfigManager::m_instance = nullptr;

    // 初始化数据
    // 先将数据存入配置文件
    ServerConfig *groupConfig = new ServerConfig;
    groupConfig->m_serverName = "group_item";
    groupConfig->m_address = "127.0.0.1";
    groupConfig->m_group = "group2020";
    groupConfig->m_userName = "dzw";
    groupConfig->m_port = "22";
    ServerConfig *config = new ServerConfig;
    config->m_serverName = "1988";
    config->m_address = "127.0.0.1";
    config->m_group = "";
    config->m_userName = "dzw";
    config->m_port = "22";
    // 保存数据
    ServerConfigManager::instance()->saveServerConfig(groupConfig);
    ServerConfigManager::instance()->saveServerConfig(config);

    //初始化数据
    ServerConfigManager::instance()->initServerConfig();

    // 删除数据
    ServerConfigManager::instance()->delServerConfig(groupConfig);
    ServerConfigManager::instance()->delServerConfig(config);
}

/*******************************************************************************
 1. @函数:    removeDialog
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-25
 4. @说明:    远程管理弹窗操作
*******************************************************************************/
TEST_F(UT_ServerConfigManager_Test, removeDialog)
{
    // 初始化数据
    ServerConfig config;
    config.m_serverName = "1988";
    config.m_address = "127.0.0.1";
    config.m_group = "";
    config.m_userName = "dzw";
    config.m_port = "22";

    //初始化弹窗
    ServerConfigOptDlg *dlg = new ServerConfigOptDlg(ServerConfigOptDlg::SCT_MODIFY, &config, nullptr);
    dlg->show();

    // 将弹窗记录
    ServerConfigManager::instance()->setModifyDialog(config.m_serverName, dlg);

    // 删除弹窗
    ServerConfigManager::instance()->removeDialog(dlg);
    bool isContain = ServerConfigManager::instance()->m_serverConfigDialogMap.contains(config.m_serverName);
    EXPECT_EQ(isContain, false);
}

/*******************************************************************************
 1. @函数:    closeAllDialog
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-12-25
 4. @说明:    关闭同类弹窗
*******************************************************************************/
TEST_F(UT_ServerConfigManager_Test, closeAllDialog)
{
    // 初始化数据
    ServerConfig config;
    config.m_serverName = "1988";
    config.m_address = "127.0.0.1";
    config.m_group = "";
    config.m_userName = "dzw";
    config.m_port = "22";

    // 打开多个同一种弹窗
    ServerConfigOptDlg *dlg1 = new ServerConfigOptDlg(ServerConfigOptDlg::SCT_MODIFY, &config, nullptr);
    ServerConfigOptDlg *dlg2 = new ServerConfigOptDlg(ServerConfigOptDlg::SCT_MODIFY, &config, nullptr);
    dlg1->show();
    dlg2->show();

    // 将弹窗记录
    ServerConfigManager::instance()->setModifyDialog(config.m_serverName, dlg1);
    ServerConfigManager::instance()->setModifyDialog(config.m_serverName, dlg2);
    // 同一类弹窗
    int typeCount = ServerConfigManager::instance()->m_serverConfigDialogMap.count();
    EXPECT_EQ(typeCount, 1);
    // 弹窗数量
    int count = ServerConfigManager::instance()->m_serverConfigDialogMap[config.m_serverName].count();
    EXPECT_EQ(count, 2);

    // 将弹窗全部全部拒绝 => 拒绝后信号槽会自动删除弹窗
    ServerConfigManager::instance()->closeAllDialog(config.m_serverName);
}

#endif

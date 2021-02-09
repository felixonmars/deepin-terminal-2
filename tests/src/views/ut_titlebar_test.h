
#ifndef UT_TITLEBAR_TEST_H
#define UT_TITLEBAR_TEST_H

#include "ut_defines.h"
#include "termproperties.h"
#include <gtest/gtest.h>

class MainWindow;
class UT_TitleBar_Test : public ::testing::Test
{
public:
    UT_TitleBar_Test();
    ~UT_TitleBar_Test();

private:
    //普通窗口
    MainWindow *m_normalWindow = nullptr;

    TermProperties m_normalTermProperty;
};

#endif // UT_TITLEBAR_TEST_H


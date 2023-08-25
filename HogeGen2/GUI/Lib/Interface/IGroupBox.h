#pragma once

#include <QGroupBox>

namespace HogeGen2 {

class IGroupBox {
public:
    virtual QGroupBox *GetGroupBox();
};

}
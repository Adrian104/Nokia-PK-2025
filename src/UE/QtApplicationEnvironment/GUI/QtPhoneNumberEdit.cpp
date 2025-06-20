#include "QtPhoneNumberEdit.hpp"

#include <QPalette>
#include <QFont>
#include <QIntValidator>

namespace ue
{

QtPhoneNumberEdit::QtPhoneNumberEdit()
{
    setFont(QFont( "Arial Narrow", 16));
    setValidator( new QIntValidator(PhoneNumber::MIN_VALUE, PhoneNumber::MAX_VALUE, this));
    setMaxLength(PhoneNumber::DIGITS);
    setStyleSheet("background-color:rgba( 255, 255, 255, 0% );");
}

QtPhoneNumberEdit::~QtPhoneNumberEdit()
{

}

PhoneNumber QtPhoneNumberEdit::getPhoneNumber() const
{
    bool ok = false;
    auto str = text();
    bool localOk = false;
    decltype(PhoneNumber::value) val = str.toUInt(&localOk, 10);
    //Ensure no overflow happened in string->uint8_t, by casting back uint8_t->string
    ok = localOk && str == QString::number(val);
    if (ok)
        return PhoneNumber{val};
    return PhoneNumber{};
}

void QtPhoneNumberEdit::enable()
{
    setEnabled(true);
    setStyleSheet("QLineEdit {background-color: #ccfb5d;}");
    setFocus();
}

void QtPhoneNumberEdit::disable()
{
    setEnabled(false);
    setStyleSheet("QLineEdit {background-color: #ababab;}");
}

}

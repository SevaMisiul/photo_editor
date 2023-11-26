#include "qcolor_validator.h"

QColorValidator::QColorValidator(QObject *parent)
    : QIntValidator{parent}
{}

QValidator::State QColorValidator::validate(QString &arg1, int &arg2) const
{
    QValidator::State state = QIntValidator::validate(arg1, arg2);
    if (state == QColorValidator::Invalid || arg1.length() == 0 || arg1[0] == '-' ||arg1.toInt() < 0 || arg1.toInt() > 255) {
        return QValidator::Invalid;
    }
    return QValidator::Acceptable;
}
